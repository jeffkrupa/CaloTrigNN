#ifndef CALOTPNN_DATAFORMATS_TRHPART_HH
#define CALOTPNN_DATAFORMATS_TRHPART_HH

#include <TObject.h>


namespace baconhep
{
  class TRHPart : public TObject
  {
    public:
      TRHPart():
        //eta(0), phi(0),
        energy(0),
	timeFalling(0), time(0),
	x(0),y(0),z(0), eta(0), phi(0), RHClusterDist(0),
	hit(0),particleMatch(0),
        nRHClusterMatches(0), RHClusterMatch(0)
    {}
    ~TRHPart(){}
    //float eta, phi;
    float energy;
    float timeFalling,time; 
    float x,y,z, eta, phi, RHClusterDist;
    int hit, particleMatch;
    int nRHClusterMatches, RHClusterMatch;
    ClassDef(TRHPart,4)
  };
}
#endif
