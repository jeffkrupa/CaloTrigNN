#ifndef CALOTRIGNN_DATAFORMATS_TEVENTINFO_HH
#define CALOTRIGNN_DATAFORMATS_TEVENTINFO_HH

#include <TObject.h>

namespace baconhep 
{
  class TEventInfo : public TObject
  {
    public:
      TEventInfo():
      runNum(0), evtNum(0), lumiSec(0),
      nPU(0), nPUm(0), nPUp(0),
      nPUmean(0), nPUmeanm(0), nPUmeanp(0),
      pvx(0), pvy(0), pvz(0)
      {}
      ~TEventInfo(){}

      unsigned int  runNum, evtNum, lumiSec;                   // run number, event number, lumi section in data
      unsigned int  nPU, nPUm, nPUp;                           // number of reconstructed pile-up vertices
      float         nPUmean, nPUmeanm, nPUmeanp;               // expected number of reconstructed pile-up vertices
      float         pvx, pvy, pvz;                             // best primary vertex

    ClassDef(TEventInfo,4)
  };
}
#endif

