#ifndef CALOTPNN_DATAFORMATS_TPFPART_HH
#define CALOTPNN_DATAFORMATS_TPFPART_HH

#include <TObject.h>
#include <vector>
#include <array>

namespace baconhep
{
  class TPFPart : public TObject
  {
    public:
      TPFPart():
	pt(0), eta(0), phi(0), m(0),e(0),ecalE(0),hcalE(0),
	q(0),pfType(-1)
    {}
    ~TPFPart(){}

    float pt,eta,phi,m,e,ecalE,hcalE,avgdepth,ecalSum;
    int   q,pfType, genMatch, NGenMatch;
    std::array<double,7> depthE;
    //std::array<double,7> depthgenE;
    //std::array<double,7> depthgenESum;
    //std::array<double,7> depthESum;
    std::array<double,7> depthFrac;
    //std::vector<float> fraction;
    //std::vector<int>   detid;
    //std::vector<int>   ieta;
    //std::vector<int>   iphi;
    //std::vector<float> depth;
    //std::vector<float> gene;

    ClassDef(TPFPart,4)
  };
}
#endif
