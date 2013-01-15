#ifndef TFitParticleEtThetaPhiEMomFix_hh
#define TFitParticleEtThetaPhiEMomFix_hh

#include "../interface/TAbsFitParticle.h"
#include "TLorentzVector.h"
#include "TMatrixD.h"
#include <math.h>

class TFitParticleEtThetaPhiEMomFix: public TAbsFitParticle {

public :

  TFitParticleEtThetaPhiEMomFix();
  TFitParticleEtThetaPhiEMomFix( const TFitParticleEtThetaPhiEMomFix& fitParticle );
  TFitParticleEtThetaPhiEMomFix(TLorentzVector* pini, const TMatrixD* theCovMatrix);
  TFitParticleEtThetaPhiEMomFix(const TString &name, const TString &title, 
	       TLorentzVector* pini,
	       const TMatrixD* theCovMatrix);
  virtual ~TFitParticleEtThetaPhiEMomFix();
  virtual TAbsFitParticle* clone( TString newname = "" ) const;

  // returns derivative dP/dy with P=(p,E) and y=(et, theta, phi) 
  // the free parameters of the fit. The columns of the matrix contain 
  // (dP/d(et), dP/d(theta), dP/d(phi)).
  virtual TMatrixD* getDerivative();
  virtual TMatrixD* transform(const TLorentzVector& vec);
  virtual void setIni4Vec(const TLorentzVector* pini);
  virtual TLorentzVector* calc4Vec( const TMatrixD* params );

protected :

  void init(TLorentzVector* pini, const TMatrixD* theCovMatrix);

//  ClassDef(TFitParticleEtThetaPhiEMomFix, 1)    // Particle with spherical 4vector parametrization and free energy

private:
  
};

#endif

