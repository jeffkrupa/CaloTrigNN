#ifndef PF_HH
#define PF_HH

float fPt = 0., fEnergy = 0., fRho = 0., fX = 0., fY = 0., fZ = 0., fGenE = 0., fGain = 0., fInPedAvg = 0., fEraw = 0., fIeta = 0., fIphi = 0., fDepth = 0., fPhi = 0., fEta = 0., fEcalE = 0.; 

float fdepthFrac0 = 0., fdepthFrac1 = 0., fdepthFrac2 = 0., fdepthFrac3 = 0., fdepthFrac4 = 0., fdepthFrac5 = 0., fdepthFrac6 = 0.; 
int fEvent = 0, fLV = 0; 

void loop(std::string lName);
void PF();
#endif 
