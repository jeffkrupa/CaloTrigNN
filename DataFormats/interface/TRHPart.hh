#ifndef CALOTPNN_DATAFORMATS_TRHPART_HH
#define CALOTPNN_DATAFORMATS_TRHPART_HH

#include <TObject.h>


namespace baconhep
{
  class TRHPart : public TObject
  {
    public:
      TRHPart():
        energy(0),
	x(0),y(0),z(0),
	hit(0)
    {}
    ~TRHPart(){}

    float energy;
    float x,y,z;
    int hit;
    ClassDef(TRHPart,4)
  };
}
#endif
