#include "../interface/TTreeObservables.h"
using namespace std;

TTreeObservables::TTreeObservables ()
{
  label_.clear ();
  tree = new TTree ("OBS_full", "list of observables");
  tree2 = new TTree ("OBS", "list of observables");
}

TTreeObservables::TTreeObservables (string & setname)
{
  label_.clear ();
  string filename = setname + "_tree.root";
  f1 = new TFile (filename.c_str (), "recreate"); //FIRST 'creating' file... //caveat... is it really created first?
  
  tree = new TTree ("OBS_full", "list of observables"); //THEN 'declaring' TTree...
  tree2 = new TTree ("OBS", "list of observables"); //THEN 'declaring' TTree...
}
TTreeObservables::TTreeObservables (string & setname, bool CREATE)
{
  label_.clear ();
  string filename = setname + "_tree.root";
if (CREATE){  f1 = new TFile (filename.c_str (), "recreate");  }
if (!CREATE) { f1 = new TFile ("dummy.root", "recreate");  }

             tree = new TTree ("OBS_full", "list of observables"); //THEN 'declaring' TTree...
             tree2 = new TTree ("OBS", "list of observables"); //THEN 'declaring' TTree...
}

TTreeObservables::TTreeObservables (const vector < string > &vec, string & setname,bool CREATE)
{  
  (*this) = TTreeObservables (setname,CREATE);
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
      tree2->Branch (vec[i].c_str (), &var_[i], arg);
 
    //  cout << " creating the TTree. " << vec[i].c_str () << "  "<<NofVar_<<endl;
    }

  vector <string > more_vars;

  more_vars.push_back("weight");
  more_vars.push_back("btagL");
  more_vars.push_back("btagM");
  more_vars.push_back("btagT");
  varr_ = new float[more_vars.size ()];
  
  for (unsigned int i = 0; i < more_vars.size (); i++)
    {
      varr_[i] = -8888.;
      char arg[100];
      //string name = vec[i].c_str()+"_"+ds;
      sprintf (arg, "%s/F", more_vars[i].c_str ());
      tree->Branch (more_vars[i].c_str (), &varr_[i], arg);
   //    cout << " creating the TTree. " << more_vars[i].c_str () << "  "<<more_vars.size()<<endl;
    }

  
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
      tree2->Branch (vec[i].c_str (), &var_[i], arg);

      // cout << " creating the TTree. " << vec[i].c_str () << "  "<<NofVar_<<endl;
    }
	}

TTreeObservables::TTreeObservables (const vector < string > &vec)
{
  (*this) = TTreeObservables ();
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
      tree2->Branch (vec[i].c_str (), &var_[i], arg);
      // cout << " creating the TTree. " << vec[i].c_str () << "  "<<NofVar_<<endl;
    }
}





TTreeObservables::TTreeObservables (const TTreeObservables & pobs)
{
  tree = pobs.tree;
  tree2 = pobs.tree;
  label_ = pobs.label_;
}


TTreeObservables::~TTreeObservables ()
{
}


void
TTreeObservables::FillTtree ()
{
  tree->Fill ();
  tree2->Fill ();


}
void
TTreeObservables::Fill (const Observables & obs,float weight, int &btagL,  int &btagM, int  & btagT,bool bfill)
{
  // if((int) obs.Variables().size()!= NofVar_) {cerr<<"Warning: TTreeObservables ... size are not correct in the Fill() method"<<" NofVar "<<NofVar_<<" Variables "<<obs.Variables().size()<<endl; return;}
  for (int i = 0; i < NofVar_; i++)
    {
    if ( (obs.Variables ()[i]).second > -9999 )  var_[i] = (obs.Variables ()[i]).second;

         //cout<<" NOW JUST FILLED THE "<<obs.Variables()[i].second<<"   "<<obs.Variables()[i].first<<"  "<<i<<"  "<<obs.Variables().size()<<" weight  "<<weight<<endl;
    }

    varr_[0]=weight;
    varr_[1]=btagL;
    varr_[2]=btagM;
    varr_[3]=btagT;

if (bfill){
  tree->Fill ();
  tree2->Fill ();
}
  //cout<<" DONE WITH FILLING THE TREE "<<endl;

}
void
TTreeObservables::Fill (const Observables & obs,bool bfill)
{
  // if((int) obs.Variables().size()!= NofVar_) {cerr<<"Warning: TTreeObservables ... size are not correct in the Fill() method"<<" NofVar "<<NofVar_<<" Variables "<<obs.Variables().size()<<endl; return;}
  for (int i = 0; i < NofVar_; i++)
    {
      var_[i] = (obs.Variables ()[i]).second;
      
             cout<<" NOW JUST FILLED THE "<<obs.Variables()[i].second<<"   "<<obs.Variables()[i].first<<"  "<<i<<"  "<<obs.Variables().size()<<endl;

    }
if (bfill){
  tree->Fill ();
  tree2->Fill ();
}
  //cout<<" DONE WITH FILLING THE TREE "<<endl;

}



void
TTreeObservables::Fill (const ObservablesEl & obs,float weight, int &btagL,  int &btagM, int  & btagT,bool bfill)
{
  // if((int) obs.Variables().size()!= NofVar_) {cerr<<"Warning: TTreeObservables ... size are not correct in the Fill() method"<<" NofVar "<<NofVar_<<" Variables "<<obs.Variables().size()<<endl; return;}
  for (int i = 0; i < NofVar_; i++)
    {
    if ( (obs.Variables ()[i]).second > -9999 )  var_[i] = (obs.Variables ()[i]).second;

         //cout<<" NOW JUST FILLED THE "<<obs.Variables()[i].second<<"   "<<obs.Variables()[i].first<<"  "<<i<<"  "<<obs.Variables().size()<<" weight  "<<weight<<endl;
    }

    varr_[0]=weight;
    varr_[1]=btagL;
    varr_[2]=btagM;
    varr_[3]=btagT;

if (bfill){
  tree->Fill ();
  tree2->Fill ();
}
  //cout<<" DONE WITH FILLING THE TREE "<<endl;

}
void
TTreeObservables::Fill (const ObservablesEl & obs,bool bfill)
{
  // if((int) obs.Variables().size()!= NofVar_) {cerr<<"Warning: TTreeObservables ... size are not correct in the Fill() method"<<" NofVar "<<NofVar_<<" Variables "<<obs.Variables().size()<<endl; return;}
  for (int i = 0; i < NofVar_; i++)
    {
      var_[i] = (obs.Variables ()[i]).second;
      
             cout<<" NOW JUST FILLED THE "<<obs.Variables()[i].second<<"   "<<obs.Variables()[i].first<<"  "<<i<<"  "<<obs.Variables().size()<<endl;

    }
if (bfill){
  tree->Fill ();
  tree2->Fill ();
}
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
 // tree2->SetBranchStatus("btag*",0);
//  tree2->SetBranchStatus("weight",0);
  //f2->cd ();

  //tree2->Write ();
  //f2->Close ();
  
  
  f1->cd ();
  tree->Write ();

  tree2->SetBranchStatus("btag*",0);
  tree2->SetBranchStatus("weight",0);

  tree2->Write ();
  f1->Close ();


}

void
TTreeObservables::SaveInfo(string & fout, const vector <pair< string, float > >   &vector_){
	TFile * target;
	vector <pair< string, float > >  vectest = vector_;
	string filename = fout + "_tree.root";
	
	cout<<" THE NAME OF THE TREEEEEE IS ==============================   "<<filename.c_str()<<endl;
	target = TFile::Open(filename.c_str(),"update");
	target->cd();
	//char tree_name[50];
	//sprintf(tree_name,"Info");
	TTree *t = new TTree("Event Yields Info","Event Yields Info");
       
	t->Branch("selection_table_",&vectest);
	t->Fill();
	target->Write();
	//target->Close();
	delete target;

	}




