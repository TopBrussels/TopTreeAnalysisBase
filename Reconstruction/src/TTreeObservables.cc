#include "../interface/TTreeObservables.h"
using namespace std;

TTreeObservables::TTreeObservables ()
{
  label_.clear ();
  tree = new TTree ("OBS", "list of observables");
}

TTreeObservables::TTreeObservables (string & setname)
{
  label_.clear ();
  string filename = setname + "_tree.root";
  f1 = new TFile (filename.c_str (), "recreate"); //FIRST 'creating' file... //caveat... is it really created first?
  tree = new TTree ("OBS", "list of observables"); //THEN 'declaring' TTree...
}

TTreeObservables::TTreeObservables (const vector < string > &vec, string & setname)
{  
  (*this) = TTreeObservables (setname);
  label_ = vec;
  NofVar_ = vec.size ();
  var_ = new float[vec.size ()];
  for (unsigned int i = 0; i < vec.size (); i++)
    {
      var_[i] = -8888.;
      char arg[100];
      //string name = vec[i].c_str()+"_"+ds;
      sprintf (arg, "%s/F", vec[i].c_str ());
      tree->Branch (vec[i].c_str (), &var_[i], arg);
      // cout << " creating the TTree. " << vec[i].c_str () << "  "<<NofVar_<<endl;
    }
}





TTreeObservables::TTreeObservables (const TTreeObservables & pobs)
{
  tree = pobs.tree;
  label_ = pobs.label_;
}


TTreeObservables::~TTreeObservables ()
{
}


void
TTreeObservables::FillTtree ()
{
  tree->Fill ();


}
void
TTreeObservables::Fill (const Observables & obs,bool bfill)
{
  // if((int) obs.Variables().size()!= NofVar_) {cerr<<"Warning: TTreeObservables ... size are not correct in the Fill() method"<<" NofVar "<<NofVar_<<" Variables "<<obs.Variables().size()<<endl; return;}
  for (int i = 0; i < NofVar_; i++)
    {
      var_[i] = (obs.Variables ()[i]).second;

      
      //       cout<<" NOW JUST FILLED THE "<<obs.Variables()[i].second<<"   "<<obs.Variables()[i].first<<"  "<<i<<"  "<<obs.Variables().size()<<endl;

    }
if (bfill)
  tree->Fill ();

  //cout<<" DONE WITH FILLING THE TREE "<<endl;

}




void
TTreeObservables::Write (TFile * fout, bool normalized)
{

  cout<<"  NOW JUST WROTE THE TTree "<<endl;
  fout->cd ();
  tree->Write ();
  fout->Close();
  
}


void
TTreeObservables::Write (bool normalized)
{
  f1->cd ();
  tree->Write ();
  f1->Close ();

}
