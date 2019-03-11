#ifndef PF_HH
#define PF_HH

float fPt = 0., fEnergy = 0., fRho = 0., fX = 0., fY = 0., fZ = 0., fGenE = 0., fGain = 0., fInPedAvg = 0., fEraw = 0., fIeta = 0., fIphi = 0., fDepth = 0., fPhi = 0., fEta = 0., fEcalE = 0.; 

float fdepthFrac0 = 0., fdepthFrac1 = 0., fdepthFrac2 = 0., fdepthFrac3 = 0., fdepthFrac4 = 0., fdepthFrac5 = 0., fdepthFrac6 = 0.; 
int fEvent = 0, fPU = 0; 
float fRaw0 = 0., fRaw1 = 0., fRaw2 = 0., fRaw3 = 0., fRaw4 = 0., fRaw5 = 0., fRaw6 = 0., fRaw7 = 0.;
float fPed0 = 0., fPed1 = 0., fPed2 = 0., fPed3 = 0., fPed4 = 0., fPed5 = 0., fPed6 = 0., fPed7 = 0.;
float fInNoiseADC0 = 0., fInNoiseADC1 = 0., fInNoiseADC2 = 0., fInNoiseADC3 = 0., fInNoiseADC4 = 0., fInNoiseADC5 = 0., fInNoiseADC6 = 0., fInNoiseADC7 = 0.;
float fInPedestal0 = 0., fInPedestal1 = 0., fInPedestal2 = 0., fInPedestal3 = 0., fInPedestal4 = 0., fInPedestal5 = 0., fInPedestal6 = 0., fInPedestal7 = 0.;
float fInNoisePhoto0 = 0., fInNoisePhoto1 = 0., fInNoisePhoto2 = 0., fInNoisePhoto3 = 0., fInNoisePhoto4 = 0., fInNoisePhoto5 = 0., fInNoisePhoto6 = 0., fInNoisePhoto7 = 0;
float fInputTDC0 = 0., fInputTDC1 = 0., fInputTDC2 = 0., fInputTDC3 = 0., fInputTDC4 = 0., fInputTDC5 = 0., fInputTDC6 = 0., fInputTDC7 = 0.;

void loop(std::string lName);
void PF();
#endif 
