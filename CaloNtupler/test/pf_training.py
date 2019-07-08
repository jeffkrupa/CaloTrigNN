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

from keras.optimizers import Adam, Nadam, SGD
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
    parser.add_option('--infile',  action='store', type='string', dest='infile',default='/eos/uscms/store/user/jkrupa/pf_studies/newMinBiaspu_gen0_dR2/oldfn/Apr2.rootskimmed', help='infile dir')
    parser.add_option('--new',     action='store_true',           dest='new',   default=False,     		  help='if sig and bkg files are new')
    parser.add_option('--inputvars',  action='store', type='string', dest='lVars',default=['eta','phi','depthFrac0','depthFrac1','depthFrac2','depthFrac3','depthFrac4','depthFrac5','depthFrac6'], help='input variables')
    parser.add_option('--train_pt_cut', dest="train_pt_cut", type=float,default=1.0, help='pT requirement on training sample')
    parser.add_option('--weights',     action='store_true',           dest='use_weights',           default=False,            help='weight PU and LV pT spectra')

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

def make_roc_plot(fpr, tpr, auc, pT_min, pT_max, eta_min, eta_max,options):
        plt.figure()
        plt.plot([0, 1], [0, 1], 'k--')
        plt.plot(fpr, tpr, label='auc = {:.3f}'.format(auc))
        plt.xlabel('False positive rate')
        plt.ylabel('True positive rate')
        if options.use_weights:
          plt.title('%.1f <= pT <= %.1f, %.1f <= eta <= %.1f, Training pT > %.1f, PU weighted'%(pT_min, pT_max, eta_min, eta_max,options.train_pt_cut))
        else:
          plt.title('%.1f <= pT <= %.1f, %.1f <= eta <= %.1f, Training pT > %.1f'%(pT_min, pT_max, eta_min, eta_max,options.train_pt_cut))
 
        plt.legend(loc='best')
        #plt.savefig("plots/roc_%.1f_pt_%.1f_%.1f_eta_%.1f.png"%(pT_min, pT_max, eta_min, eta_max))
        plt.savefig("plots/roc_%.1f_pt_%.1f_%.1f_eta_%.1f.pdf"%(pT_min, pT_max, eta_min, eta_max))

def make_roc_curve(y_pred, y_test, eta_test, pt_test, pT_min, pT_max, eta_min, eta_max,options):

        print 'Make roc curve for kinematic region %.1f <= pT <= %.1f, %.1f <= eta <= %.1f'%(pT_min,pT_max,eta_min,eta_max)

        y_pred_tmp = []
        y_test_tmp = []


        #y_pred.reshape((pt_test.shape[0],1))
        #y_test.reshape((pt_test.shape[0],1))
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
        make_roc_plot(fpr, tpr, auc_val, pT_min, pT_max, eta_min, eta_max,options)
        print 'AUC = ', auc_val

def plot_th1(lTH1):
    print 'making TH1 plot'
    c0 = rt.TCanvas("c0","c0",800,600)
    l0 = rt.TLegend(0.7,0.75,0.9,0.9)
    for i0, TH1 in enumerate(lTH1):
      TH1.Draw("hist same")
      TH1.SetLineColor(i0+1)
      TH1.SetStats(0)
      l0.AddEntry(TH1,TH1.GetName())
      c0.Update()

    l0.Draw()
    c0.Draw()
    c0.SaveAs("plots/training_pT_distribution.pdf")
    del c0; 

def get_weights(y_train, pt_train,options):

    print 'getting weights...'
    Nbins = 100

    LV = rt.TH1D("LV", "LV", Nbins, options.train_pt_cut, 100.)
    PU = rt.TH1D("PU", "PU", Nbins, options.train_pt_cut, 100.)
    W  = rt.TH1D("W",  "W",  Nbins, options.train_pt_cut, 100.)

    y_train  = y_train .values
    pt_train = pt_train.values

    for y in y_train:
        if y == 1:  LV.Fill(pt_train[i0])
        else:       PU.Fill(pt_train[i0])

    LV.Scale(1./LV.Integral())
    PU.Scale(1./PU.Integral())

    plot_th1([LV,PU])

    #make weight in each pT cat
    for i0 in range(LV.GetNbinsX()):
        if (PU.GetBinContent(i0+1) == 0.) or (LV.GetBinContent(i0+1) == 0.): 
            W.SetBinContent(i0+1, 1)
        else: W.SetBinContent(i0+1, float(LV.GetBinContent(i0+1)) / float(PU.GetBinContent(i0+1)))

        #make sure weight isn't too big or too small
        weightUpper = 5.0
        weightLower = 0.2
        if  W.GetBinContent(i0+1) > weightUpper:  W.SetBinContent(i0+1) == weightUpper 
        if  W.GetBinContent(i0+1) < weightLower:  W.SetBinContent(i0+1) == WeightLower
        

    W.Print("all")
    #apply weights on training PU samples
    candWeights = np.zeros(len(y_train),dtype=float)
    for y in y_train:
        if y == 1: candWeights[i0] == 1.				       #don't weight LV
        else:      candWeights[i0] = W.GetBinContent(W.FindBin(pt_train[i0]))  #weight PU

    return candWeights    

class TMVA_Analysis():
    def __init__(self,options):
        TMVA.Tools.Instance()
        TMVA.PyMethodBase.PyInitialize()
        gROOT.LoadMacro( "./TMVAGui.C" )
        self._lOutput     = TFile.Open('TMVA.root', 'RECREATE')
        self._lFactory    = TMVA.Factory('TMVAClassification', self._lOutput,'!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification')
        self._lDataLoader = TMVA.DataLoader("dataset")

        for i0 in options.lVars:
            self._lDataLoader.AddVariable(i0,'F')

        self._lDataLoader.Print("all")
        #define signal and background tree based on LV flag
        if options.new:
           self._lInputFile  = TFile.Open(options.infile+'.root')
           self._lInputTree  = self._lInputFile.Get("Events")
           self._lSigFile       = TFile.Open("sig.root","RECREATE")
           self._lBkgFile       = TFile.Open("bkg.root","RECREATE")
           self._lSigFile.cd()
           self._lSigTree       = self._lInputTree.CopyTree("LV && std::abs(eta) < 3.0 && std::abs(eta) > 1.7 && pt > 5")
           self._lSigTree.Show(53)
           self._lSigTree.Write()
           self._lSigFile.Close() 
           self._lBkgFile.cd()
           self._lBkgTree       = self._lInputTree.CopyTree("!LV")# && std::abs(eta) < 3.0 && std::abs(eta) > 1.7 && pt > 5")
           self._lBkgTree.Write()
           self._lBkgFile.Close()
        
        self._lSigFile    = TFile.Open("sig.root","READ")
        self._lBkgFile    = TFile.Open("bkg.root","READ")
        self._lSigTree    = self._lSigFile.Get("Events")
        self._lBkgTree    = self._lBkgFile.Get("Events")

        self._lDataLoader.AddSignalTree    (self._lSigTree,1.0)
        self._lDataLoader.AddBackgroundTree(self._lBkgTree,1.0)

        nSig = self._lSigTree.GetEntries()
        nBkg = self._lBkgTree.GetEntries()

        nSigTrain = nSig*0.8
        nBkgTrain = nSig*0.8*1.5

        self._lDataLoader.PrepareTrainingAndTestTree(TCut(""),TCut(""),"nTrain_Signal=%i:nTrain_Background=%i:nTest_Signal=%i:nTest_Background=%i:SplitMode=Random:NormMode=NumEvents:!V"%(nSigTrain,nBkgTrain,nSigTrain,nBkgTrain)) 

        Methods =  {
        #'Variable': TMVA.Types.kVariable,
        
        #'Cuts': TMVA.Types.kCuts,
        'Likelihood': TMVA.Types.kLikelihood,
        #'BDT': TMVA.Types.kBDT
        #'PyRandomForest': TMVA.Types.kPyRandomForest,
        #'MaxMethod': TMVA.Types.kMaxMethod
        }
        '''
        'PDERS': TMVA.Types.kPDERS,
        'HMatrix': TMVA.Types.kHMatrix,
        'Fisher': TMVA.Types.kFisher,
        'KNN': TMVA.Types.kKNN,
        'CFMlpANN': TMVA.Types.kCFMlpANN,
        'TMlpANN': TMVA.Types.kTMlpANN,
        'BDT': TMVA.Types.kBDT,
        'DT': TMVA.Types.kDT,
        'RuleFit': TMVA.Types.kRuleFit,
        'SVM': TMVA.Types.kSVM,
        'MLP': TMVA.Types.kMLP,
        'BayesClassifier': TMVA.Types.kBayesClassifier,
        'FDA': TMVA.Types.kFDA,
        'Boost': TMVA.Types.kBoost,
        'PDEFoam': TMVA.Types.kPDEFoam,
        'LD': TMVA.Types.kLD,
        'Plugins': TMVA.Types.kPlugins,
        'Category': TMVA.Types.kCategory,
        'DNN': TMVA.Types.kDNN,
        'PyRandomForest': TMVA.Types.kPyRandomForest,
        'PyAdaBoost': TMVA.Types.kPyAdaBoost,
        'PyGTB': TMVA.Types.kPyGTB,
        'PyKeras': TMVA.Types.kPyKeras,
        'C50': TMVA.Types.kC50,
        'RSNNS': TMVA.Types.kRSNNS,
        'RSVM': TMVA.Types.kRSVM,
        'RXGB': TMVA.Types.kRXGB,
        'MaxMethod': TMVA.Types.kMaxMethod
        '''

        for m,t in Methods.iteritems():
           self._lFactory.BookMethod( self._lDataLoader, t, m, "" )

        self._lFactory.BookMethod(self._lDataLoader, TMVA.Types.kBDT, 'BDT', '!H:!V:NTrees=300:MinNodeSize=2.5%:MaxDepth=3:BoostType=Grad:SeparationType=GiniIndex:nCuts=100:UseBaggedBoost=True:PruneMethod=NoPruning')
        #self._lfactory.bookmethod(self._ldataloader, tmva.types.kbdt, 'BDT2', '!h:!v:ntrees=300:minnodesize=2.5%:maxdepth=4:boosttype=AdaBoost:separationtype=crossentropy:ncuts=100:prunemethod=nopruning')

        #self._lfactory.bookmethod(self._ldataloader, tmva.types.kbdt, 'BDT3', '!h:!v:ntrees=300:minnodesize=2.5%:maxdepth=4:boosttype=AdaBoost:separationtype=GiniIndex:ncuts=100:prunemethod=nopruning')
        #self._lFactory.BookMethod( self._lDataLoader, TMVA.Types.kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" )

        model = Sequential()
        model.add(Dense(len(options.lVars), input_dim=len(options.lVars),activation='tanh'))
        model.add(Dense(30,  activation='tanh'))
        model.add(Dense(20,  activation='relu'))
        model.add(Dense(10,  activation='tanh'))
        model.add(Dense(5,   activation='relu'))
        model.add(Dense(2,   activation='sigmoid'))
 
        model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy',])
        model.save('model.h5')
        model.summary()

        self._lFactory.BookMethod(self._lDataLoader, TMVA.Types.kPyKeras, 'PyKeras', 'H:!V:FilenameModel=model.h5:NumEpochs=10:BatchSize=500')

        frozen_graph = freeze_session(K.get_session(),
                              output_names=[out.op.name for out in model.outputs])
        tf.train.write_graph(frozen_graph, "h5_files", "tf_model.pb", as_text=False)
 

        self._lFactory.Print("v")

    def TMVA(self):
        self._lFactory.TrainAllMethods()
        self._lFactory.TestAllMethods()
        self._lFactory.EvaluateAllMethods()
        self._lOutput.Close()


class DNN_Training():
    def __init__(self,options):
        rt.gROOT.SetBatch(rt.kTRUE)

    def get_data(self):
 
        print 'Build dataset'
        lFile = h5py.File(options.infile+'.h5','r')

        lArr = np.array(lFile.get('Events')[:])

        df = pd.DataFrame(data=lArr)
        y  = df['LV']
        x  = df.drop(columns=['ecalE','genE','LV','energy','pt'])

        eta = df['eta']
        pt  = df['pt']

        num_vars = len(x.columns)

        msk  = np.random.rand(len(df)) < 0.8
        msk2 = np.zeros(len(df),dtype=bool)

        for i0 in range(len(msk)):
            if (msk[i0] == True) and (pt.iloc[i0] > options.train_pt_cut):
                msk2[i0] = True
      
        #print msk2[0:1000]
        y_train, x_train  = y[msk2], x[msk2]
        y_test, x_test    = y[~msk2], x[~msk2] 
        print x_test

        #propagate eta, pt for ROC curves, weights
        eta_train,pt_train = eta[msk2],pt[msk2]
        eta_test, pt_test  = eta[~msk2],pt[~msk2]

	print 'Train on %i PF candidates (satisfying pT > %f) and use %i for validation.'%(len(y_train),options.train_pt_cut,len(y_test))

        return x_train, x_test, y_train, y_test, num_vars, eta_test, pt_test, eta_train, pt_train


    def build_model(self,num_vars):
        print 'Defining model...'
        model = Sequential()
        model.add(Dense(num_vars, input_dim=num_vars,activation='tanh'))
        model.add(Dense(30, activation='tanh'))
        model.add(Dense(20,  activation='tanh'))
        model.add(Dense(10,  activation='tanh'))
        model.add(Dense(5,  activation='tanh'))
        model.add(Dense(1,  activation='sigmoid'))
        model.compile(optimizer='adam',loss='binary_crossentropy',metrics=['accuracy'])
        return model

    def train(self):
        x_train, x_test, y_train, y_test, num_vars, eta_test, pt_test, eta_train, pt_train = self.get_data()

        Nbatch = 500
        Nepoch = 10
        
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
                            outputDir='/uscms_data/d3/jkrupa/pf_studies/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/test/h5_files')

        print 'Fit model...'
        if options.use_weights:
            weights = get_weights(y_train, pt_train, options)
            history = model.fit(x_train, y_train, epochs=Nepoch, batch_size=Nbatch, callbacks=callbacks.callbacks, validation_split=0.0, sample_weight=weights)
        else:
            history = model.fit(x_train, y_train, epochs=Nepoch, batch_size=Nbatch, callbacks=callbacks.callbacks, validation_split=0.0)


        #https://hackernoon.com/simple-guide-on-how-to-generate-roc-plot-for-keras-classifier-2ecc6c73115a
        y_pred                = model.predict(x_test).ravel()

        #inclusive
        fpr, tpr, thresholds  = roc_curve(y_test, y_pred)

        #kinematic binning
        if options.inc:
          lpT  = [1.,10000.0]
          leta = [1.7,3.0]
        else:
          lpT  = [1.,5.,10.,20.,10000.0]
          leta = [1.7,2.0,2.5,3.0]


        if options.makeroc:
          for i0 in range(len(lpT)-1):
            for i1 in range(len(leta)-1):
              make_roc_curve(y_pred,y_test,eta_test, pt_test, lpT[i0],lpT[i0+1],leta[i1],leta[i1+1],options)


        frozen_graph = freeze_session(K.get_session(),
                              output_names=[out.op.name for out in model.outputs])
        tf.train.write_graph(frozen_graph, "h5_files", "tf_model.pb", as_text=False)


        print_model_to_json(model,'/uscms_data/d3/jkrupa/pf_studies/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/test/h5_files/model.json')
        model.save_weights('/uscms_data/d3/jkrupa/pf_studies/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/test/h5_files/dense_model_weights.h5')
        json_string = model.to_json()

if __name__ == "__main__":
    options = parser()
    print options
    #pftrain = DNN_Training(options)
    #pftrain.train()

    tmva_ana = TMVA_Analysis(options)
    tmva_ana.TMVA()
