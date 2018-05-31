#ifndef CALOTPNN_DATAFORMATS_THCALDEP_HH
#define CALOTPNN_DATAFORMATS_THCALDEP_HH

#include <TObject.h>
#include <vector>

namespace baconhep
{
  class THcalDep : public TObject {
  public:
    THcalDep():ieta(0),iphi(0),depth(0),genE(0) {}
    ~THcalDep(){}
    std::vector<short>  hitsidx;
    std::vector<bool>   hitssat;
    std::vector<float>  hitsE  ;
    std::vector<float>  EOut ;
    std::vector<float>  EId  ;
    std::vector<unsigned long>   fine1;
    std::vector<unsigned long>   fine2;
    int ieta;
    int iphi;
    int depth;
    float genE;
    std::vector<int>    genEDepth;
    std::vector<float>  genElayer;
    std::vector<float>  genETime;
    ClassDef(THcalDep,1)
  };
}
#endif
