#ifndef CALOTRIGANA_DATAFORMATS_LINKDEF_H
#define CALOTRIGANA_DATAFORMATS_LINKDEF_H
#include "CaloTrigNN/DataFormats/interface/TGenParticle.hh"
#include "CaloTrigNN/DataFormats/interface/THcalDep.hh"
#include "CaloTrigNN/DataFormats/interface/TRHPart.hh"
#include "CaloTrigNN/DataFormats/interface/TPFPart.hh"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace baconhep;

#pragma link C++ class baconhep::TGenParticle+;
#pragma link C++ class baconhep::THcalDep+;
#pragma link C++ class baconhep::TRHPart+;
#pragma link C++ class baconhep::TPFPart+;
#endif
