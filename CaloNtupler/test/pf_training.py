#!/usr/bin/env python

import numpy as np
import h5py
import pandas as pd
import ROOT as rt
import sys 
import keras
from optparse import OptionParser
import os 
from ROOT import TMVA, TFile, TTree, TCut, gROOT, gApplication
from os.path import isfile
from rootpy.tree import Cut
from keras import backend as K
import tensorflow as tf

from keras.optimizers import Adam, Nadam
from keras.models import Sequential, Model
from keras.optimizers import SGD
from keras.layers import Input, Activation, Dense, Convolution2D, MaxPooling2D, Dropout, Flatten
from keras.utils import np_utils
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from callbacks import all_callbacks 
from keras.callbacks import EarlyStopping
from keras.callbacks import ModelCheckpoint
import matplotlib.pyplot as plt
from rootpy.plotting import Hist, Hist2D, Hist3D, HistStack, Legend, Canvas
from rootpy.extern.six.moves import range
from rootpy.interactive import wait
from sklearn.metrics import roc_curve
from sklearn.metrics import auc

np.random.seed(7)
dir_path = os.path.dirname(os.path.realpath(__file__))

def parser():
    parser = OptionParser()
    parser.add_option('--tag',     action='store', type='string', dest='tag',  		default='AK8v42017',      help='samples tag')
    parser.add_option('--inc',     action='store_true',           dest='inc',   	default=False,            help='make inclusive ROC')
    parser.add_option('--makeroc',     action='store_true',           dest='makeroc',   default=True,             help='make ROC')
    parser.add_option('--infile',  action='store', type='string', dest='infile',default='/eos/user/j/jekrupa/pf_studies/40pu_era2018_skimmed', help='infile dir')
    parser.add_option('--new',     action='store_true',           dest='new',   default=False,     		  help='if sig and bkg files are new')
    parser.add_option('--inputvars',  action='store', type='string', dest='lVars',default='depthFrac0,depthFrac1,depthFrac2,depthFrac3,depthFrac4,depthFrac5,depthFrac6,phi,eta,ecalE', help='input variables')

    (options,args) = parser.parse_args()
    return options

def freeze_session(session, keep_var_names=None, output_names=None, clear_devices=True):

    #from https://www.dlology.com/blog/how-to-convert-trained-keras-model-to-tensorflow-and-make-prediction/
    from tensorflow.python.framework.graph_util import convert_variables_to_constants
    graph = session.graph
    with graph.as_default():
        freeze_var_names = list(set(v.op.name for v in tf.global_variables()).difference(keep_var_names or []))
        output_names = output_names or []
        output_names += [v.op.name for v in tf.global_variables()]
        # Graph -> GraphDef ProtoBuf
        input_graph_def = graph.as_graph_def()
        if clear_devices:
            for node in input_graph_def.node:
                node.device = ""
        frozen_graph = convert_variables_to_constants(session, input_graph_def,
                                                      output_names, freeze_var_names)
        return frozen_graph

def print_model_to_json(model, outfile_name):
    outfile = open(outfile_name,'wb')
    jsonString = model.to_json()
    import json
    with outfile:
        obj = json.loads(jsonString)
        json.dump(obj, outfile, sort_keys=True,indent=4, separators=(',', ': '))
        outfile.write('\n')

def make_roc_plot(fpr, tpr, auc, pT_min, pT_max, eta_min, eta_max):
        plt.figure()
        plt.plot([0, 1], [0, 1], 'k--')
        plt.plot(fpr, tpr, label='auc = {:.3f}'.format(auc))
        plt.xlabel('False positive rate')
        plt.ylabel('True positive rate')
        plt.title('ROC curve: %.1f <= pT <= %.1f, %.1f <= eta <= %.1f'%(pT_min, pT_max, eta_min, eta_max))
        plt.legend(loc='best')
        plt.savefig("plots/roc_%.1f_pt_%.1f_%.1f_eta_%.1f.png"%(pT_min, pT_max, eta_min, eta_max))
        plt.savefig("plots/roc_%.1f_pt_%.1f_%.1f_eta_%.1f.pdf"%(pT_min, pT_max, eta_min, eta_max))

def make_roc_curve(self, y_pred, y_test, eta_test, pt_test, pT_min, pT_max, eta_min, eta_max):

        print 'Make roc curve for kinematic region %.1f <= pT <= %.1f, %.1f <= eta <= %.1f'%(pT_min,pT_max,eta_min,eta_max)

        y_pred_tmp = []
        y_test_tmp = []


        y_pred.reshape((pt_test.shape[0],1))
        y_test.reshape((pt_test.shape[0],1))
        y_test   = y_test.values.reshape((y_test.shape[0],1))
        pt_test  = pt_test.values.reshape((pt_test.shape[0],1))
        eta_test = eta_test.values.reshape((eta_test.shape[0],1))
        assert(len(y_test) == len(pt_test))
        assert(len(eta_test) == len(y_pred))

        for i0 in range(len(y_pred)):
          if (pt_test[i0][0] > pT_min and pt_test[i0][0] < pT_max and (abs(eta_test[i0][0]) > eta_min) and (abs(eta_test[i0][0]) < eta_max)):
            y_pred_tmp.append(y_pred[i0])
            y_test_tmp.append(y_test[i0][0])

        fpr, tpr, thresholds = roc_curve(y_test_tmp, y_pred_tmp)
        auc_val = auc(fpr, tpr)
        make_roc_plot(fpr, tpr, auc_val, pT_min, pT_max, eta_min, eta_max)
        print 'AUC = ', auc_val

class TMVA_Analysis():
    def __init__(self,options):
        TMVA.Tools.Instance()
        gROOT.LoadMacro( "./TMVAGui.C" )
        self._lOutput     = TFile.Open('TMVA.root', 'RECREATE')
        self._lFactory    = TMVA.Factory('TMVAClassification', self._lOutput,'!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification')
        self._lDataLoader = TMVA.DataLoader("dataset")

        for i0 in options.inputvars:
            self._lDataLoader.AddVariable(i0,'F')

        #define signal and background tree based on PU
        self._lInputFile  = TFile.Open(options.infile+'.root')
        self._lInputTree  = self._lInputFile.Get("Events")
        self._lSigFile    = TFile.Open("sig.root","READ")
        self._lBkgFile    = TFile.Open("bkg.root","READ")

        if options.new:
           self._lSigFile       = TFile.Open("sig.root","RECREATE")
           self._lBkgFile       = TFile.Open("bkg.root","RECREATE")
           self._lSigFile.cd()
           self._lSigTree       = self._lInputTree.CopyTree("PU==0")
           self._lBkgFile.cd()
           self._lBkgTree       = self._lInputTree.CopyTree("PU==1")
           self._lBkgTree.Write()
           self._lSigFile.Close(); self._lBkgFile.Close();
        else:
           self._lInputFile  = TFile.Open(options.infile+'.root')
           self._lInputTree  = self._lInputFile.Get("Events")
           self._lSigFile    = TFile.Open("sig.root","READ")
           self._lBkgFile    = TFile.Open("bkg.root","READ")
           self._lSigTree    = self._lSigFile.Get("Events")
           self._lBkgTree    = self._lBkgFile.Get("Events")

        self._lDataLoader.AddSignalTree    (self._lSigTree,1.0)
        self._lDataLoader.AddBackgroundTree(self._lBkgTree,1.0)

        self._lDataLoader.PrepareTrainingAndTestTree(TCut(""),TCut(""),"SplitMode=Random:NormMode=NumEvents:!V") 

        self._lFactory.BookMethod( self._lDataLoader, TMVA.Types.kLikelihood, "Likelihood","H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50")
        #self._lFactory.BookMethod( self._lDataLoader, TMVA.Types.kBDT, "BDTG", "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.50:nCuts=20:MaxDepth=2")
        self._lFactory.Print("v")

    def TMVA(self):
        self._lFactory.TrainAllMethods()
        self._lFactory.TestAllMethods()
        self._lFactory.EvaluateAllMethods()
        self._lOutput.Close()

        print "TMVA classification is done, open GUI"
        TMVA.TMVAGui("TMVA.root") 
        gApplication.Run()      

class DNN_Training():
    def __init__(self,options):
        pass

    def get_data(self):
 
        print 'Build dataset'
        lFile = h5py.File(options.infile+'.h5','r')

        lArr = np.array(lFile.get('Events')[:10000000])

        df = pd.DataFrame(data=lArr)
        y  = df['PU']
        x  = df.drop(columns=['genE','PU','energy','pt'])

        eta = df['eta']
        pt  = df['pt']

        num_vars = len(x.columns)

        msk = np.random.rand(len(df)) < 0.8
        
        y_train, x_train  = y[msk], x[msk]
        y_test, x_test    = y[~msk], x[~msk] 

        #propagate eta, pt for ROC curves
        eta_test, pt_test = eta[~msk],pt[~msk]

	print 'Train on %i PF candidates and use %i for validation.'%(len(y_train),len(y_test))

        return x_train, x_test, y_train, y_test, num_vars, eta_test, pt_test


    def build_model(self,num_vars):
        print 'Defining model...'
        model = Sequential()
        model.add(Dense(num_vars, input_dim=num_vars,activation='relu'))
        model.add(Dense(30, activation='relu'))
        model.add(Dense(20,  activation='relu'))
        model.add(Dense(10,  activation='relu'))
        model.add(Dense(1,  activation='sigmoid'))
        model.compile(optimizer='adam',loss='binary_crossentropy',metrics=['accuracy'])
        return model

    def train(self):
        x_train, x_test, y_train, y_test, num_vars, eta_test, pt_test = self.get_data()


        print x_train[:100]
        print y_train[:100]

        model = self.build_model(num_vars)
        model.summary()

        scaler  = StandardScaler().fit(x_train)
        x_train = scaler.transform(x_train)

        callbacks = all_callbacks(stop_patience=1000, 
                            lr_factor=0.5,
                            lr_patience=10,
                            lr_epsilon=0.000001, 
                            lr_cooldown=2, 
                            lr_minimum=0.0000001,
                            outputDir='/afs/cern.ch/work/j/jekrupa/public/CMSSW_10_0_3/src/CaloTrigNN/CaloNtupler/test/puppi_training/h5_files')

        print 'Fit model...'
        history = model.fit(x_train, y_train, epochs=5, batch_size=50000, callbacks=callbacks.callbacks, validation_split=0.0)

        #https://hackernoon.com/simple-guide-on-how-to-generate-roc-plot-for-keras-classifier-2ecc6c73115a
        y_pred                = model.predict(x_test).ravel()

        #inclusive
        fpr, tpr, thresholds  = roc_curve(y_test, y_pred)

        #kinematic binning
        if options.inc:
          lpT  = [1.,10000.0]
          leta = [1.7,3.0]
        else:
          lpT  = [1.,10.,20.,10000.0]
          leta = [1.7,2.0,2.5,3.0]


        if options.makeroc:
          for i0 in range(len(lpT)-1):
            for i1 in range(len(leta)-1):
              make_roc_curve(y_pred,y_test,eta_test, pt_test, lpT[i0],lpT[i0+1],leta[i1],leta[i1+1])


        frozen_graph = freeze_session(K.get_session(),
                              output_names=[out.op.name for out in model.outputs])
        tf.train.write_graph(frozen_graph, "h5_files", "tf_model.pb", as_text=False)


        print_model_to_json(model,'/afs/cern.ch/work/j/jekrupa/public/pf_studies/fixgit_4/CMSSW_10_0_3/src/CaloTrigNN/CaloNtupler/test/h5_files/model.json')
        model.save_weights('/afs/cern.ch/work/j/jekrupa/public/pf_studies/fixgit_4/CMSSW_10_0_3/src/CaloTrigNN/CaloNtupler/test/h5_files/dense_model_weights.h5')
        json_string = model.to_json()

if __name__ == "__main__":
    options = parser()
    print options
    #pftrain = DNN_Training(options)
    #pftrain.train()

    tmva_ana = TMVA_Analysis(options)
    tmva_ana.TMVA()
