#ifndef PF2_HH
#define PF2_HH

std::vector<float> vpt;
std::vector<float> veta;
std::vector<float> vphi;
std::vector<float> vpftype;
std::vector<float> vdepth0;
std::vector<float> vdepth1;
std::vector<float> vdepth2;
std::vector<float> vdepth3;
std::vector<float> vdepth4;
std::vector<float> vdepth5;
std::vector<float> vdepth6;
std::vector<float> vgenE;


float fPt = 0., fEnergy = 0., fPhi = 0., fEta = 0.; 

float fdepthFrac0 = 0., fdepthFrac1 = 0., fdepthFrac2 = 0., fdepthFrac3 = 0., fdepthFrac4 = 0., fdepthFrac5 = 0., fdepthFrac6 = 0.; 
int fLV = 0, fNum = 0; 

void loop(std::string lName);
void PF();
#endif 
