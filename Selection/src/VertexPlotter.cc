#include "../interface/VertexPlotter.h"


VertexPlotter::VertexPlotter (string label)
{
  label_ = label;
  //plots for Primary Vertex
  h_PVertex_isValid_ = new TH1F ( ("h_PVertex_isValid_"+label_).c_str(), "PV: isValid",2,-0.5,1.5);
  h_PVertex_isFake_ = new TH1F ( ("h_PVertex_isFake_"+label_).c_str(), "PV: isFake",2,-0.5,1.5);
  h_PVertex_chi2_ = new TH1F ( ("h_PVertex_chi2_"+label_).c_str(), "PV: #chi^{2}",100,0,100);
  h_PVertex_ndof_ = new TH1F ( ("h_PVertex_ndof_"+label_).c_str(), "PV: ndof",100,-0.5,99.5);
  h_PVertex_tracksSize_ = new TH1F ( ("h_PVertex_tracksSize_"+label_).c_str(), "PV: track size",100,-0.5,99.5);
  h_PVertex_absZ_ = new TH1F ( ("h_PVertex_absZ_"+label_).c_str(), "PV: |z|",50,0,25);
  h_PVertex_rho_ = new TH1F ( ("h_PVertex_rho_"+label_).c_str(), "PV: rho",100,0,5);
  //plots for all vertices
  h_AllVertex_mult_ = new TH1F ( ("h_AllVertex_mult_"+label_).c_str(), "Nof vertex",20,-0.5,19.5);
  h_AllVertex_isValid_ = new TH1F ( ("h_AllVertex_isValid_"+label_).c_str(), "All vertex: isValid",2,-0.5,1.5);
  h_AllVertex_isFake_ = new TH1F ( ("h_AllVertex_isFake_"+label_).c_str(), "All vertex: isFake",2,-0.5,1.5);
  h_AllVertex_chi2_ = new TH1F ( ("h_AllVertex_chi2_"+label_).c_str(), "All vertex: #chi^{2}",100,0,100);
  h_AllVertex_ndof_ = new TH1F ( ("h_AllVertex_ndof_"+label_).c_str(), "All vertex: ndof",100,-0.5,99.5);
  h_AllVertex_tracksSize_ = new TH1F ( ("h_AllVertex_tracksSize_"+label_).c_str(), "All vertex: track size",100,-0.5,99.5);
  h_AllVertex_absZ_ = new TH1F ( ("h_AllVertex_absZ_"+label_).c_str(), "All vertex: |z|",50,0,25);
  h_AllVertex_rho_ = new TH1F ( ("h_AllVertex_rho_"+label_).c_str(), "All vertex: rho",100,0,5);
}

VertexPlotter::VertexPlotter (const VertexPlotter & mp)
{
  //plots for Primary Vertex
  h_PVertex_isValid_ = mp.h_PVertex_isValid_;
  h_PVertex_isFake_ = mp.h_PVertex_isFake_;
  h_PVertex_chi2_ = mp.h_PVertex_chi2_;
  h_PVertex_ndof_ = mp.h_PVertex_ndof_;
  h_PVertex_tracksSize_ = mp.h_PVertex_tracksSize_;
  h_PVertex_absZ_ = mp.h_PVertex_absZ_;
  h_PVertex_rho_ = mp.h_PVertex_rho_;
  //plots for all vertices
  h_AllVertex_mult_ = mp.h_AllVertex_mult_;
  h_AllVertex_isValid_ = mp.h_AllVertex_isValid_;
  h_AllVertex_isFake_ = mp.h_AllVertex_isFake_;
  h_AllVertex_chi2_ = mp.h_AllVertex_chi2_;
  h_AllVertex_ndof_ = mp.h_AllVertex_ndof_;
  h_AllVertex_tracksSize_ = mp.h_AllVertex_tracksSize_;
  h_AllVertex_absZ_ = mp.h_AllVertex_absZ_;
  h_AllVertex_rho_ = mp.h_AllVertex_rho_;
}

VertexPlotter::~VertexPlotter ()
{
  //plots for Primary Vertex
  h_PVertex_isValid_->Delete();
  h_PVertex_isFake_->Delete();
  h_PVertex_chi2_->Delete();
  h_PVertex_ndof_->Delete();
  h_PVertex_tracksSize_->Delete();
  h_PVertex_absZ_->Delete();
  h_PVertex_rho_->Delete();
  //plots for all vertices
  h_AllVertex_mult_->Delete();
  h_AllVertex_isValid_->Delete();
  h_AllVertex_isFake_->Delete();
  h_AllVertex_chi2_->Delete();
  h_AllVertex_ndof_->Delete();
  h_AllVertex_tracksSize_->Delete();
  h_AllVertex_absZ_->Delete();
  h_AllVertex_rho_->Delete();
}

void
VertexPlotter::Fill (vector < TRootVertex > vertex)
{
  if (vertex.size () > 0) {
    float Rho = sqrt (vertex[0].x () * vertex[0].x () + vertex[0].y () * vertex[0].y ());
    h_PVertex_isValid_->Fill (vertex[0].isValid ());
    h_PVertex_isFake_->Fill (vertex[0].isFake ());
    h_PVertex_chi2_->Fill (vertex[0].chi2 ());
    h_PVertex_ndof_->Fill (vertex[0].ndof ());
    h_PVertex_tracksSize_->Fill (vertex[0].tracksSize ());
    h_PVertex_absZ_->Fill (abs (vertex[0].z ()));
    h_PVertex_rho_->Fill (Rho);
  }
  h_AllVertex_mult_->Fill (vertex.size ());
  for (unsigned int i = 0; i < vertex.size (); i++) {
    float Rho = sqrt (vertex[i].x () * vertex[i].x () + vertex[i].y () * vertex[i].y ());
    h_AllVertex_isValid_->Fill (vertex[i].isValid ());
    h_AllVertex_isFake_->Fill (vertex[i].isFake ());
    h_AllVertex_chi2_->Fill (vertex[i].chi2 ());
    h_AllVertex_ndof_->Fill (vertex[i].ndof ());
    h_AllVertex_tracksSize_->Fill (vertex[i].tracksSize ());
    h_AllVertex_absZ_->Fill (abs (vertex[i].z ()));
    h_AllVertex_rho_->Fill (Rho);
  }
}


void
VertexPlotter::Write (TFile * fout)
{
  string dirname ("VertexPlots");
  if(fout->Get(dirname.c_str())==0)
    fout->mkdir(dirname.c_str());
  ((TDirectory *) (fout->Get (dirname.c_str ())))->mkdir (label_.c_str ());
  char subdirName[200];
  sprintf (subdirName, "%s/%s", dirname.c_str (), label_.c_str ());
  fout->cd (subdirName);
  //plots for Primary Vertex
  h_PVertex_isValid_->Write();
  h_PVertex_isFake_->Write();
  h_PVertex_chi2_->Write();
  h_PVertex_ndof_->Write();
  h_PVertex_tracksSize_->Write();
  h_PVertex_absZ_->Write();
  h_PVertex_rho_->Write();
  //plots for all vertices
  h_AllVertex_mult_->Write();
  h_AllVertex_isValid_->Write();
  h_AllVertex_isFake_->Write();
  h_AllVertex_chi2_->Write();
  h_AllVertex_ndof_->Write();
  h_AllVertex_tracksSize_->Write();
  h_AllVertex_absZ_->Write();
  h_AllVertex_rho_->Write();
}
