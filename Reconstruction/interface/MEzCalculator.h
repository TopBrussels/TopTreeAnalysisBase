#ifndef MEzCalculator_h
#define MEzCalculator_h


#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootJet.h"

using namespace TopTree;

class MEzCalculator {
  
 public:
  /// constructor
  MEzCalculator();
  /// destructor
  ~MEzCalculator();
  /// Set MET
  void SetMET(const TRootMET& MET) { MET_ = MET; } ;
  void SetMET(const TLorentzVector& MET) { MET_ = MET; } ;
  /// Set Muon
  void SetMuon(const TRootMuon& lepton) { lepton_ = lepton; };
  void SetMuon(const TLorentzVector& lepton) { lepton_ = lepton; };
  /// Calculate MEz
  /// options to choose roots from quadratic equation:
  /// type = 0 (defalut): if real roots, pick the one nearest to
  ///                     the lepton Pz except when the Pz so chosen
  ///                     is greater than 300 GeV in which case pick
  ///                     the most central root.
  /// type = 1: if real roots, choose the one closest to the lepton Pz
  ///           if complex roots, use only the real part.
  /// type = 2: if real roots, choose the most central solution.
  ///           if complex roots, use only the real part.
  double Calculate(int type = 0);
  /// check for complex root
  bool IsComplex() const { return isComplex_; };
  
 private:
  
  bool isComplex_;
  //TRootMuon lepton_;
  //TRootMET MET_;  
  TLorentzVector lepton_;
  TLorentzVector MET_;  
};

#endif
