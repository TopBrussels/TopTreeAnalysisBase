// Classname: TFitParticleEtThetaPhiEMomFix
//
//________________________________________________________________
// 
// TFitParticleEtThetaPhiEMomFix::
// --------------------
//
// Particle with a special parametrization useful in hadron 
// colliders (3 free parameters (Et, Eta, Phi). The parametrization is 
// chosen as follows:
//
// p = (EtCosPhi, EtSinPhi, Et/Tan(Theta)
// E =  Et/Sin(Theta)
// E/p is kept constant to the initial value

using namespace std;

#include <iostream>
#include "../interface/TFitParticleEtThetaPhiEMomFix.h"
#include "TMath.h"

//ClassImp(TFitParticleEtThetaPhiEMomFix)

//----------------
// Constructor --
//----------------
TFitParticleEtThetaPhiEMomFix::TFitParticleEtThetaPhiEMomFix()
  :TAbsFitParticle()  
{
  init(0, 0);
}

TFitParticleEtThetaPhiEMomFix::TFitParticleEtThetaPhiEMomFix( const TFitParticleEtThetaPhiEMomFix& fitParticle )
  :TAbsFitParticle( fitParticle.GetName(), fitParticle.GetTitle() )
{

  _nPar = fitParticle._nPar;
  _u1 = fitParticle._u1;
  _u2 = fitParticle._u2;
  _u3 = fitParticle._u3;
  _covMatrix.ResizeTo(  fitParticle._covMatrix );
  _covMatrix = fitParticle._covMatrix;
  _iniparameters.ResizeTo( fitParticle._iniparameters );
  _iniparameters = fitParticle._iniparameters;
  _parameters.ResizeTo( fitParticle._parameters );
  _parameters = fitParticle._parameters;
  _pini = fitParticle._pini;
  _pcurr = fitParticle._pcurr;

}

TFitParticleEtThetaPhiEMomFix::TFitParticleEtThetaPhiEMomFix(TLorentzVector* pini, const TMatrixD* theCovMatrix)
  :TAbsFitParticle()  
{
  init(pini, theCovMatrix);
}

TFitParticleEtThetaPhiEMomFix::TFitParticleEtThetaPhiEMomFix(const TString &name, const TString &title, 
			   TLorentzVector* pini, const TMatrixD* theCovMatrix)
  :TAbsFitParticle(name, title)  
{
  init(pini, theCovMatrix);
}

TAbsFitParticle* TFitParticleEtThetaPhiEMomFix::clone( TString newname ) const {
  // Returns a copy of itself
  
  TAbsFitParticle* myclone = new TFitParticleEtThetaPhiEMomFix( *this );
  if ( newname.Length() > 0 ) myclone->SetName(newname);
  return myclone;

}

//--------------
// Destructor --
//--------------
TFitParticleEtThetaPhiEMomFix::~TFitParticleEtThetaPhiEMomFix() {

}

//--------------
// Operations --
//--------------
void TFitParticleEtThetaPhiEMomFix::init(TLorentzVector* pini, const TMatrixD* theCovMatrix ) {

  _nPar = 3;
  setIni4Vec(pini);
  setCovMatrix(theCovMatrix);

}

TLorentzVector* TFitParticleEtThetaPhiEMomFix::calc4Vec( const TMatrixD* params ) {
  // Calculates a 4vector corresponding to the given
  // parameter values

  if (params == 0) {
    return 0;
  }

  if ( params->GetNcols() != 1 || params->GetNrows() !=_nPar ) {
    cerr << "WrongMatrixSize" << GetName() << "::calc4Vec - Parameter matrix has wrong size." << endl;
    return 0;
  }

  Double_t et = (*params)(0,0);
  Double_t theta = (*params)(1,0);
  Double_t phi = (*params)(2,0);
	
	Double_t PoverE = sqrt(pow(_pini.X(),2) + pow(_pini.Y(),2) + pow(_pini.Z(),2))/_pini.E();

  Double_t X = PoverE*et*TMath::Cos(phi);
  Double_t Y = PoverE*et*TMath::Sin(phi);
  Double_t Z = PoverE*et/TMath::Tan(theta);
  Double_t E = et/TMath::Sin(theta);
		
  TLorentzVector* vec = new TLorentzVector( X, Y, Z, E );
  return vec;

}

void TFitParticleEtThetaPhiEMomFix::setIni4Vec(const TLorentzVector* pini) {
  // Set the initial 4vector. Will also set the 
  // inital parameter values 

  if (pini == 0) {

    _u1.SetXYZ(0., 0., 0.);
    _u3.SetXYZ(0., 0., 0.);
    _u2.SetXYZ(0., 0., 0.);
    _pini.SetXYZT(0., 0., 0., 0.);
    _pcurr = _pini;

    _iniparameters.ResizeTo(_nPar,1);
    _iniparameters(0,0) = 0.;
    _iniparameters(1,0) = 0.;
    _iniparameters(2,0) = 0.;
    
    _parameters.ResizeTo(_nPar,1);
    _parameters(0,0) = 0.;
    _parameters(1,0) = 0.;
    _parameters(2,0) = 0.;   
    
  } else {
    
    Double_t et = pini->E()*fabs(sin(pini->Theta()));
    Double_t theta = pini->Theta();
    Double_t phi = pini->Phi();
    
    _pini = (*pini);
    _pcurr = _pini;
    
    _iniparameters.ResizeTo(_nPar,1);
    _iniparameters(0,0) = et;
    _iniparameters(1,0) = theta;
    _iniparameters(2,0) = phi;
    
    _parameters.ResizeTo(_nPar,1);
    _parameters = _iniparameters;

    _u1.SetXYZ( TMath::Cos(phi), TMath::Sin(phi), 0.); // the base vector of Et
    _u2.SetXYZ( TMath::Cos(phi)*TMath::Cos(theta), TMath::Sin(phi)*TMath::Cos(theta), -1.*TMath::Sin(theta) );// the base vector of Eta ( same as the base vector for Theta)
    _u3.SetXYZ( -1.*TMath::Sin(phi), TMath::Cos(phi), 0. );// the base vector of Phi

  }

}

TMatrixD* TFitParticleEtThetaPhiEMomFix::getDerivative() {
  // returns derivative dP/dy with P=(p,E) and y=(et, theta, phi) 
  // the free parameters of the fit. The columns of the matrix contain 
  // (dP/d(et), dP/d(theta), dP/d(phi)).

  TMatrixD* DerivativeMatrix = new TMatrixD(4,3);
  (*DerivativeMatrix) *= 0.;

  Double_t et = _parameters(0,0);
  Double_t theta = _parameters(1,0);
  Double_t phi = _parameters(2,0);

  Double_t CotanTheta = 1./TMath::Tan(theta);

	Double_t PoverE = sqrt(pow(_pini.X(),2) + pow(_pini.Y(),2) + pow(_pini.Z(),2))/_pini.E();

  //1st column: dP/d(et)
  (*DerivativeMatrix)(0,0) = PoverE*TMath::Cos(phi);
  (*DerivativeMatrix)(1,0) = PoverE*TMath::Sin(phi);
  (*DerivativeMatrix)(2,0) = PoverE*CotanTheta;
  (*DerivativeMatrix)(3,0) = 1./TMath::Sin(theta);

  //2nd column: dP/d(theta)
  (*DerivativeMatrix)(0,1) = 0.;
  (*DerivativeMatrix)(1,1) = 0.;
  (*DerivativeMatrix)(2,1) = -1.*PoverE*et*(1.+CotanTheta*CotanTheta);
  (*DerivativeMatrix)(3,1) = -1.*et*CotanTheta/(TMath::Sin(theta));

   //3rd column: dP/d(phi)
  (*DerivativeMatrix)(0,2) = -1.*PoverE*et*TMath::Sin(phi);
  (*DerivativeMatrix)(1,2) = PoverE*et*TMath::Cos(phi);
  (*DerivativeMatrix)(2,2) = 0.;
  (*DerivativeMatrix)(3,2) = 0.;

  return DerivativeMatrix;

}

TMatrixD* TFitParticleEtThetaPhiEMomFix::transform(const TLorentzVector& vec) {
  // Returns the parameters corresponding to the given 
  // 4vector

  // retrieve parameters
  TMatrixD* tparams = new TMatrixD( _nPar, 1 );
  (*tparams)(0,0) = vec.E()*fabs(sin(vec.Theta()));
  (*tparams)(1,0) = vec.Theta();
  (*tparams)(2,0) = vec.Phi();

  return tparams;

}

