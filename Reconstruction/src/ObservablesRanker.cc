#include "../interface/ObservablesRanker.h"
#include "../interface/Observables.h"
#include "TCut.h"
#include "math.h"


using namespace std;
//using namespace TopTree;

ObservablesRanker::ObservablesRanker ()
{;
}

ObservablesRanker::ObservablesRanker (const Observables & obs)
{
  if (obs.Variables ().size () > 0) {
    for (unsigned int i = 0; i < obs.Variables ().size (); i++) {
      variables_.push_back (obs.Variables ()[i].first);
    }
  }

}


ObservablesRanker::ObservablesRanker (TTree * treeSignal, TTree * treeBkg, string & merged_file_, string & np_file_, string & sm_file_, bool &tbl_overlap, bool &tbl_correlation, bool &tbl_overlap_sorted_cleaned , bool & DoCorrs,vector <float> cor_cut_)
{

  ComputeOverlap (merged_file_, np_file_, sm_file_);

  
  tree_Signal = treeSignal;
  tree_Bkg = treeBkg;
	if (DoCorrs){
	DoOnlyCorrelation(tbl_correlation);
	SaveCorrelanceObservablesList(sm_file_);
	}
for (unsigned int i=0;i<cor_cut_.size();i++){
/*	overlaps_.clear();
	overlaps_sorted_.clear();
	overlaps_sorted_cleaned_.clear();
	overlaps_sorted_pro_cleaned_.clear();
	overlaps_sorted_cleaned_name_.clear();
	overlaps_sorted_cleaned_value_.clear();
*/

  //correlations
	correlations_.clear();
	correlations_sorted_.clear();
	correlation_.clear();
	correlation_sorted_.clear();
	correlation_with_name_.clear();
	correlation_with_name_sorted_.clear();
        all_correlation_table_.clear();
	high_correlation_position_.clear();
	high_correlation_matrix_.clear();
	correlation_sorted_cleaned_.clear();
	correlations_sorted_cleaned_.clear();

 	DoCorrelation(tbl_correlation,cor_cut_[i]);
  	PrintOverlapTable (tbl_overlap);
  	PrintCorrelationTable (tbl_correlation, cor_cut_[i]);
  	PrintOverlapSortedCleanedTable(tbl_overlap_sorted_cleaned);
        SaveBestObservablesList(merged_file_,cor_cut_[i]);
  //Read(merged_file_);
}

}



ObservablesRanker::ObservablesRanker (string & merged_file_, string & np_file_, string & sm_file_, bool &tbl_overlap, bool &tbl_correlation, bool &tbl_overlap_sorted_cleaned, float & cor_cut)
{


  ComputeOverlap (merged_file_, np_file_, sm_file_);
   
  //for (int i=0;i<cor_cut_.size();i++){
  DoCorrelation(tbl_correlation,cor_cut);
  PrintOverlapTable (tbl_overlap);
  PrintCorrelationTable (tbl_correlation, cor_cut);
  PrintOverlapSortedCleanedTable(tbl_overlap_sorted_cleaned);
  //}
}



ObservablesRanker::~ObservablesRanker ()
{
  delete tree_Signal;;
  delete tree_Bkg;
}



void
ObservablesRanker::SaveCorrelanceObservablesList(string & fout){
  TFile * target;
  string file = fout+"_tree.root";
  target = TFile::Open(file.c_str(),"update");
  target->cd();
  
  TTree *Tree_Cor = new TTree("CorrelanceVector","List of Correlance for Observables");
  
  Tree_Cor->Branch("all_correlation_table_",&all_correlation_table_);
//  for (unsigned int i=0;i<all_correlation_table_.size();i++){
//cout<<" all------------ "<<all_correlation_table_[i].first<<"   "<<fout<<endl;}
  
  Tree_Cor->Fill();
  target->Write();
  target->Close();
  delete target;
}

void
ObservablesRanker::SaveBestObservablesList(string & fout, float & cor){
  TFile * target;
  target = TFile::Open(fout.c_str(),"update");
  target->cd();
  char tree_name[50];
  sprintf(tree_name,"ListVector with Cor of %f",cor);
  TTree *t = new TTree(tree_name,"List of Sorted and Cleaned Observables");
  t->Branch("overlaps_sorted_cleaned_",&overlaps_sorted_cleaned_);
  t->Fill();
  target->Write();
  target->Close();
  delete target;
}
	    
void
ObservablesRanker::Read(string &fout,float & cor){
  TFile *f = TFile::Open(fout.c_str(),"read");
  TTree *t;
  char tree_name[50];
  sprintf(tree_name,"ListVector with Cor of %f",cor);
  f->GetObject(tree_name,t);
  TBranch *br_list=0;
  vector<pair<string, float> >* myListP=&overlaps_sorted_cleaned_;
  t->SetBranchAddress("overlaps_sorted_cleaned_",&myListP,&br_list);
  Long64_t tentry = t->LoadTree(0);
  br_list->GetEntry(tentry);
  for (unsigned int j=0;j<overlaps_sorted_cleaned_.size();++j){
    cout<<" NOW READ OUT THE VECTOR  "<<overlaps_sorted_cleaned_[j].first<<"    "<<overlaps_sorted_cleaned_[j].second<<endl;
  }
  delete t;
  f->Close();
  delete f;


}

void
ObservablesRanker::ComputeOverlap (string & fout, string & np_file, string & sm_file)
{
  gROOT->SetStyle ("Plain");
  gStyle->SetPalette (1);

  TList *FileList;
  TFile *Target;
  string fileout = fout;
  string file_np = np_file + ".root";
  string file_sm = sm_file + ".root";
  FileList = new TList ();
  FileList->Add (TFile::Open (file_np.c_str ()));
  FileList->Add (TFile::Open (file_sm.c_str ()));

  //TFile *Target = new TFile (fileout.c_str (), "RECREATE");
  Target = TFile::Open (fout.c_str (), "RECREATE");

  // more than 2 source files: not tested
  string np_title = np_file;
  Impose (Target, FileList, np_title);
  delete FileList;
  delete Target;


}

void
ObservablesRanker::Impose (TDirectory * target, TList * sourcelist, string & np_title_)
{
  cout << "	" << "========================================================" << endl;
  cout << "	" << "This is a macro to superimpose plots of different root files." << endl;
  cout << "	" << "Only TH1F objects are superimposed." << endl;
  cout << "	" << "Target path: " << target->GetPath () << endl;
  TString path ((char *) strstr (target->GetPath (), ":"));
  path.Remove (0, 2);
  std::vector < pair < string, float > > BinContent_;


overlaps_.clear();
overlaps_sorted_.clear();

  TFile *first_source = (TFile *) sourcelist->First ();
  first_source->cd (path);
  TDirectory *current_sourcedir = gDirectory;
  //gain time, do not add the objects in the list in memory
  Bool_t status = TH1::AddDirectoryStatus ();
  TH1::AddDirectory (kFALSE);

  // loop over all keys in this directory
  TChain *globChain = 0;
  TIter nextkey (current_sourcedir->GetListOfKeys ());
  TKey *key, *oldkey = 0;
  while ((key = (TKey *) nextkey ())) {

    //keep only the highest cycle number for each key
    if (oldkey && !strcmp (oldkey->GetName (), key->GetName ()))
      continue;

    // read object from first source file and create a canvas
    first_source->cd (path);
    TObject *obj = key->ReadObj ();
    TCanvas *c1 = new TCanvas ("c1", obj->GetName (), 500, 500);
    TLegend *legend_c1 = new TLegend (0.65, 0.80, 0.89, 0.70);

    if (obj->IsA ()->InheritsFrom ("TH1F")) {
      // descendant of TH1F -> prepare the histograms to be superimposed

      //      cout << "Modifying histogram " << obj->GetName() << endl;      
      TH1F *h1 = (TH1F *) obj;
      ModifyHist (h1, kRed);

      // loop over all source files and modify the correspondant
      // histogram to the one pointed to by "h1"
      TFile *nextsource = (TFile *) sourcelist->After (first_source);
      while (nextsource) {

	// make sure we are at the correct directory level by cd'ing to path
	nextsource->cd (path);
	TKey *key2 = (TKey *) gDirectory->GetListOfKeys ()->FindObject (h1->GetName ());
	if (key2) {
	  TH1F *h2 = (TH1F *) key2->ReadObj ();
	  ModifyHist (h2, kBlue);



	  double maxh1;
	  double maxh2;
	  CS = 0;
	  TH1F* htest;
	  if (h1->GetNbinsX()>h2->GetNbinsX()) htest=h1;
	  else htest=h2;
	  // overlaps_.clear();overlaps_sorted_.clear();
	  for (Int_t b = 1; b <= htest->GetNbinsX () + 1; b++) {
	    bincontS = h2->GetBinContent (b);
	    bincontB = h1->GetBinContent (b);
	    // cout<<" BIN CONTENT "<<h1->GetName()<<"  "<<bincontS<<"  "<<bincontB<<"  "<<b<<"  "<<h1->GetNbinsX()<<"  "<<h2->GetNbinsX()<<"  "<<h1->GetBinCenter(h1->GetNbinsX()+1)<<"  "<<h2->GetBinCenter(h2->GetNbinsX()+1)<<"  "<<h1->GetBinCenter(h1->GetNbinsX()+1)<<"  "<<h2->GetBinCenter(h2->GetNbinsX()+1)<<endl;

	    if (bincontS <= bincontB)
	      CS += bincontS;
	    else
	      CS += bincontB;
	  }
	
	  //if (CS>0)
	  overlaps_.push_back (pair < string, float >((htest->GetName ()), CS));
	  //if (CS>0)
	  overlaps_sorted_.push_back (pair < string, float >((htest->GetName ()), CS));
	  // overlaps_sorted_cleaned_.push_back(pair< string, float> ((h1->GetName()), CS));
	  maxh1 = h1->GetMaximum (10000.);
	  maxh2 = h2->GetMaximum (10000.);
	  //maxh1 = h1->GetBinCenter(h1->GetNbinsX()+1);
          //maxh2 = h2->GetBinCenter(h2->GetNbinsX()+1);
	  if (maxh1 > maxh2) {
	    h1->Draw ();
	    h2->Draw ("SAME");

	  }
	  else {
	    h2->Draw ();
	    h1->Draw ("SAME");

	  }



	  legend_c1->SetTextFont (70);
	  legend_c1->SetTextSize (0.03);
	  legend_c1->AddEntry (h1, np_title_.c_str (), "L");	// change this aBranch_Bkg_ording to first source file
	  legend_c1->AddEntry (h2, "TTJets", "L");	// change this aBranch_Bkg_ording to second source file
	  legend_c1->Draw ("SAME");
	}

	nextsource = (TFile *) sourcelist->After (nextsource);
      }				// while ( nextsource )
    }
    else if (obj->IsA ()->InheritsFrom ("TTree")) {	// not tested

      // loop over all source files create a chain of Trees "globChain"
      const char *obj_name = obj->GetName ();

      globChain = new TChain (obj_name);
      globChain->Add (first_source->GetName ());
      TFile *nextsource = (TFile *) sourcelist->After (first_source);
      //      const char* file_name = nextsource->GetName();
      // cout << "file name  " << file_name << endl;
      while (nextsource) {

	globChain->Add (nextsource->GetName ());
	nextsource = (TFile *) sourcelist->After (nextsource);
      }

    }
    else if (obj->IsA ()->InheritsFrom ("TDirectory")) {	// not tested
      // it's a subdirectory

      cout << "Found subdirectory " << obj->GetName () << endl;

      // create a new subdir of same name and title in the target file
      target->cd ();
      ////     TDirectory *newdir = target->mkdir (obj->GetName (), obj->GetTitle ());

      // newdir is now the starting point of another round of superimposing
      // newdir still knows its depth within the target file via
      // GetPath(), so we can still figure out where we are in the recursion
      // Impose( newdir, sourcelist );

    }
    else {

      // object is of no type that we know or can handle
      cout << "Unknown object type, name: " << obj->GetName () << ", object type: " << obj->ClassName () << endl;
    }

    // now draw and write the superimposed histograms to the target file
    // note that this will just store the canvas c1 in the current directory level,
    // which is not persistent until the complete directory itself is stored
    // by "target->Write()" below
    if (obj) {
      target->cd ();

      //!!if the object is a tree, it is stored in globChain...     
      if (obj->IsA ()->InheritsFrom ("TTree"))	// not tested
	globChain->Merge (target->GetFile (), 0, "keep");
      else
	c1->Write (key->GetName ());
    }


    delete obj;
    delete c1;
    delete legend_c1;
  }				// while ( ( TKey *key = (TKey*)nextkey() ) )

  // save modifications to target file
  target->SaveSelf (kTRUE);
  TH1F::AddDirectory (status);
  cout << "	" << "========================================================" << endl;
  cout<< " Ended SuperImpose of files.... " <<endl;

  // for (int i=0;i<SValueVariables_.size();i++){
  //cout<<SValueVariables_[i].second<<endl;}


  /*   TObject *obj = key->ReadObj ();
       TCanvas *c1 = new TCanvas ("c1", obj->GetName (), 500, 500);
       TLegend *legend_c1 = new TLegend (0.65, 0.80, 0.89, 0.70);
  */



}




void
ObservablesRanker::ModifyHist (TH1F * &h, Color_t lcolor)
{
  double temp_integral;

  h->SetLineColor (lcolor);
  temp_integral = h->Integral ();
  //cout << temp_integral << endl;
  if (temp_integral != 0)
    h->Scale (pow (temp_integral, -1));
}


void
ObservablesRanker::PrintOverlapTable (bool & cout_table_over)
{

 
  //sort (overlaps_.begin (), overlaps_.end ());
  sort (overlaps_sorted_.begin (), overlaps_sorted_.end (), sort_pred ());
  // sort (overlaps_sorted_cleaned_.begin (), overlaps_sorted_cleaned_.end (), sort_pred ());
  cout << " OVERLAPS AND CORRELATION SORTED OVERLAPS " << overlaps_sorted_.size () << "  " << correlations_.size () << endl;
  if (cout_table_over){
    cout << "\\begin{table}" << endl;
    cout << "\\centering" << endl;
    cout << "\\begin{tabular}{l|";
    for (unsigned int j = 0; j < overlaps_sorted_.size (); j++)


      cout << "c";
    cout << "}" << endl;
    cout << "\\hline" << endl;
    //fill first line of the table
    cout << "Variables & \t";
    //loop over overlaps
  }


  if (cout_table_over) cout << " Now will print the Sorted Table according to Overlap ascending ...." << endl;
  for (unsigned int i = 0; i < overlaps_sorted_.size (); i++) {
    if (cout_table_over)  cout << overlaps_sorted_[i].first << " &\t";


    for (unsigned int j = 0; j < overlaps_.size (); j++) {
      if (overlaps_[j].first == overlaps_sorted_[i].first) {
	//cout<< " FOUND THE PAIR , it was "<<"\t"<<overlaps_[j].first<<" and now is   "<<"\t"<<overlaps_sorted_[i].first<<" was in place  "<<"\t"<<j<<" and now is in place "<<"\t"<<i<<"\t"<<" with value  "<<overlaps_[j].second<<"  "<<overlaps_sorted_[i].second<<endl;
//	correspondant_.push_back (pair < int, int >(j, i));
//	correspondant_cleaned_.push_back (pair < int, int >(j, i));
      }
    }
  }
  if (cout_table_over){
    cout << " \\\\" << endl;
    cout << "\\hline" << endl;
    cout << "Overlap & \t";
    //loop over overlaps
    for (unsigned int i = 0; i < overlaps_sorted_.size (); i++) {

      cout << overlaps_sorted_[i].second << " &\t";
    }
    cout << " \\\\" << endl;
    cout << "\\hline" << endl;

    cout << "  Finished with overlap table.... " << endl;

  }

}


void
ObservablesRanker::PrintOverlapSortedCleanedTable (bool & cout_table_srt_cln)
{
 
  //sort (overlaps_.begin (), overlaps_.end ());
  sort (overlaps_sorted_cleaned_.begin (), overlaps_sorted_cleaned_.end (), sort_pred ());
  sort (overlaps_sorted_cleaned_.begin (), overlaps_sorted_cleaned_.end (), sort_pred ());
  //  cout << " OVERLAPS AND CORRELATION SORTED AND CLEANED OVERLAPS " << overlaps_sorted_cleaned_.size () << "  " << correlations_.size () << endl;


  if (cout_table_srt_cln){
    cout << "\\begin{table}" << endl;
    cout << "\\centering" << endl;
    cout << "\\begin{tabular}{l|";
    for (unsigned int j = 0; j < overlaps_sorted_cleaned_.size (); j++)


      cout << "c";
    cout << "}" << endl;
    cout << "\\hline" << endl;
    //fill first line of the table
    cout << "Variables & \t";
    //loop over overlaps



    cout << " Now will print the Sorted and Cleaned Table according to Overlap ascending ...." << endl;
    for (unsigned int i = 0; i < overlaps_sorted_cleaned_.size (); i++) {
      cout << overlaps_sorted_cleaned_[i].first << " &\t";
  
    }
    cout << " \\\\" << endl;
    cout << "\\hline" << endl;
    cout << "Overlap & \t";
    //loop over overlaps
    for (unsigned int i = 0; i < overlaps_sorted_cleaned_.size (); i++) {

      cout << overlaps_sorted_cleaned_[i].second << " &\t";
    }
    cout << " \\\\" << endl;
    cout << "\\hline" << endl;

    cout << "  Finished with overlap sorted-cleaned table.... " << endl;

  }

}


void
ObservablesRanker::DoOnlyCorrelation ( bool &cout_table_corr_srt_cln )
{


  TObjArray *Branches_Bkg = tree_Bkg->GetListOfBranches ();


  Int_t Entries_Bkg = Branches_Bkg->GetEntries ();
  

  for (Int_t i = 0; i < Entries_Bkg; i++) {

    TBranch *Branch_Sgnl_ = (TBranch *) Branches_Bkg->At (i);


    char Name_Bkg_[30];

    sprintf (Name_Bkg_, "%s", Branch_Sgnl_->GetName ());


    for (Int_t j = 0; j < Entries_Bkg; j++) {


      TBranch *Branch_Bkg_ = (TBranch *) Branches_Bkg->At (j);


      char Name_Bkg2_[30];


      sprintf (Name_Bkg2_, "%s", Branch_Bkg_->GetName ());

      tree_Bkg->Draw (Form ("%s:%s>>hcorr", Name_Bkg_, Name_Bkg2_, "", "goff"));
      TH2 *hcorr = (TH2 *) gDirectory->Get ("hcorr");
      //      cout << "The correlation from TTJets " << Name_Bkg_ << " with " << Name_Bkg2_ << "   correl factor =   " << '\t' << hcorr->GetCorrelationFactor () << "  " << endl;

      float hCorr = hcorr->GetCorrelationFactor ();
      //   correlation_sorted_.push_back (hCorr);
 all_correlation_table_.push_back(pair <string, float > (Name_Bkg_,hCorr));

    }
  }

}


void
ObservablesRanker::DoCorrelation ( bool &cout_table_corr_srt_cln , float & cor_cut)
{


//  high_correlation_matrix_.clear();

  TObjArray *Branches_Bkg = tree_Bkg->GetListOfBranches ();


  Int_t Entries_Bkg = Branches_Bkg->GetEntries ();
  

  for (Int_t i = 0; i < Entries_Bkg; i++) {

    TBranch *Branch_Sgnl_ = (TBranch *) Branches_Bkg->At (i);


    char Name_Bkg_[30];

    sprintf (Name_Bkg_, "%s", Branch_Sgnl_->GetName ());


    for (Int_t j = 0; j < Entries_Bkg; j++) {


      TBranch *Branch_Bkg_ = (TBranch *) Branches_Bkg->At (j);


      char Name_Bkg2_[30];


      sprintf (Name_Bkg2_, "%s", Branch_Bkg_->GetName ());

      tree_Bkg->Draw (Form ("%s:%s>>hcorr", Name_Bkg_, Name_Bkg2_, "", "goff"));
      TH2 *hcorr = (TH2 *) gDirectory->Get ("hcorr");
      //      cout << "The correlation from TTJets " << Name_Bkg_ << " with " << Name_Bkg2_ << "   correl factor =   " << '\t' << hcorr->GetCorrelationFactor () << "  " << endl;

      float hCorr = hcorr->GetCorrelationFactor ();
      if (fabs (hCorr) > cor_cut && Name_Bkg_ != Name_Bkg2_ && j > i) {
	high_correlation_matrix_.push_back (pair < string, string > (Name_Bkg_,Name_Bkg2_));


      }

    }
  }

}


void
ObservablesRanker::PrintCorrelationTable ( bool &cout_table_corr_srt_cln , float & cor_cut)
{



  high_correlation_position_.clear();
  overlaps_sorted_pro_cleaned_.clear();
  
  sort (overlaps_sorted_.begin (), overlaps_sorted_.end (), sort_pred ());

  correlation_with_name_sorted_.clear ();

    char Name_Bkg_[30];
    char Name_Bkg2_[30];
  
    for (unsigned int i = 0; i < high_correlation_matrix_.size(); i++) {


    sprintf (Name_Bkg_, "%s", high_correlation_matrix_[i].first.c_str());

      sprintf (Name_Bkg2_, "%s", high_correlation_matrix_[i].second.c_str());


    //for (unsigned int ii = 0; ii < high_correlation_matrix_.size(); ii++) {
    //if (Name_Bkg_==Name_Bkg2_) cout <<"  WARNINGGGGGGGGGGG NOT UNIQUE RECORD  "<<Name_Bkg_<<"   "<<Name_Bkg2_<<endl;}

      for (unsigned int kk=0;kk<overlaps_sorted_.size();kk++){

	if (overlaps_sorted_[kk].first == Name_Bkg_){ // && high_correlation_matrix_[i].first.c_str() !=high_correlation_matrix_[j].second.c_str()){ 

      		for (unsigned int ll=kk;ll<overlaps_sorted_.size();ll++){

			if (overlaps_sorted_[ll].first == Name_Bkg2_){
	      
	      high_correlation_position_.push_back (pair < string, int >(Name_Bkg2_, ll));

    	//  cout << " Will now remove the " << Name_Bkg2_ <<'\t'<< "  as has high Cor with  " << Name_Bkg_ <<'\t'<<" Var to be kept.. " <<'\t'<< overlaps_sorted_[kk].first <<'\t'<< " with  " <<'\t'<< overlaps_sorted_[kk].second <<" var to be removed..."<< '\t'<< overlaps_sorted_[ll].first <<'\t'<< " with  " <<'\t'<< overlaps_sorted_[ll].second <<" equal  "<<Name_Bkg2_<<endl;
	  

			}
		}
	}
	
	else if (overlaps_sorted_[kk].first == Name_Bkg2_){ // && high_correlation_matrix_[i].first.c_str() !=high_correlation_matrix_[j].second.c_str()){ 

      		for (unsigned int ll=kk;ll<overlaps_sorted_.size();ll++){

			if (overlaps_sorted_[ll].first == Name_Bkg_){
	      
	      high_correlation_position_.push_back (pair < string, int >(Name_Bkg_, ll));
    	 // cout << " Will now remove the " << Name_Bkg_ <<'\t'<< "  as has high Cor with  " << Name_Bkg2_ <<'\t'<<" Var to be kept.. " <<'\t'<< overlaps_sorted_[kk].first <<'\t'<< " with  " <<'\t'<< overlaps_sorted_[kk].second <<" var to be removed..."<< '\t'<< overlaps_sorted_[ll].first <<'\t'<< " with  " <<'\t'<< overlaps_sorted_[ll].second <<" equal  "<<Name_Bkg2_<<endl;

			}
		}
	
	
	}      
	}
	}
  sort (high_correlation_position_.begin (), high_correlation_position_.end (), sort_string_int ());

  
  high_correlation_position_.erase (unique (high_correlation_position_.begin (), high_correlation_position_.end ()), high_correlation_position_.end ());

  overlaps_sorted_pro_cleaned_ = overlaps_sorted_;


  //   for (unsigned int l = 0; l < overlaps_sorted_.size (); l++) {
  //cout<<" Overlaps  "<<overlaps_sorted_[l].first<<"   "<<overlaps_sorted_pro_cleaned_[l].first<<"   "<<overlaps_sorted_[l].second<<"   "<<overlaps_sorted_pro_cleaned_[l].second<<"   "<<endl;   }

  for (unsigned int l = 0; l < high_correlation_position_.size (); l++) {
   
	   
    for (unsigned int m = 0; m < overlaps_sorted_pro_cleaned_.size (); m++) {

      if (high_correlation_position_[l].first==overlaps_sorted_pro_cleaned_[m].first){
//	cout<<"  Will have to move from the list .... "<<"   "<<high_correlation_position_[l].first<<"  "<<overlaps_sorted_pro_cleaned_[m].first<<endl;
	overlaps_sorted_pro_cleaned_.erase(overlaps_sorted_pro_cleaned_.begin() + m);}
    }

  }


/* for (unsigned int m = 0; m < overlaps_sorted_pro_cleaned_.size (); m++) {

    cout<<"  should be ok now... "<<overlaps_sorted_pro_cleaned_[m].first<<endl;
  }
*/


    char names[30];
    char namet[30];

	  
	  for (unsigned int j = 0; j < overlaps_sorted_pro_cleaned_.size (); j++) {


    sprintf (names, "%s", overlaps_sorted_pro_cleaned_[j].first.c_str());

    for (unsigned int jj = 0; jj < overlaps_sorted_pro_cleaned_.size(); jj++) {


      sprintf (namet, "%s", overlaps_sorted_pro_cleaned_[jj].first.c_str());

      tree_Bkg->Draw (Form ("%s:%s>>hcorr", overlaps_sorted_pro_cleaned_[j].first.c_str(), overlaps_sorted_pro_cleaned_[jj].first.c_str(), "", "goff"));
      TH2 *hcorrs = (TH2 *) gDirectory->Get ("hcorr");
      float hCorr=0 ; hCorr= hcorrs->GetCorrelationFactor ();


      correlation_sorted_cleaned_.push_back (hcorrs->GetCorrelationFactor ());
 
 if (hCorr > cor_cut && names!=namet && j!=jj ){ cout<<" ---------------->>>>>>>>>>>>>>>>  THE Correlance Factor is "<<hCorr<< " and should be lower that that..... CHECK!!!!!!!!!!!!!!!! " <<names<<"   and   "<<namet<<"   "<<correlation_sorted_cleaned_.size()<<endl; 
 }
    }
  }



/*  for (unsigned int i = 0; i < correlation_sorted_.size (); i++) {
    correlations_sorted_.push_back (correlation_sorted_);


  }

*/
  for (unsigned int i = 0; i < correlation_sorted_cleaned_.size (); i++) {
    correlations_sorted_cleaned_.push_back (correlation_sorted_cleaned_);


  }


   overlaps_sorted_cleaned_.clear();
   overlaps_sorted_cleaned_ = overlaps_sorted_pro_cleaned_;

  if (cout_table_corr_srt_cln){

    cout << "" << endl;
    cout << "  NOW WILL PRINT SORTED AND CLEANED TABLE  FOR A CORRELATION OF " <<cor_cut<< endl;
    cout << "" << endl;





    for (unsigned int ii = 0; ii < overlaps_sorted_cleaned_.size (); ii++) {


      cout << overlaps_sorted_cleaned_[ii].first << "  " << "&\t";
    }
    cout << "\\\\" << endl;

      int ovv = overlaps_sorted_cleaned_.size ();
    for (unsigned int i = 0; i < overlaps_sorted_cleaned_.size (); i++) {
      cout << overlaps_sorted_cleaned_[i].first << "&\t";


      for (unsigned int k = 0; k < overlaps_sorted_cleaned_.size (); k++) {

	cout << correlation_sorted_cleaned_[k + ovv * i] << "&\t";
      }

      cout << " \\\\" << endl;
    }
    cout << "\\hline" << endl;

  }
  ////

}




float
ObservablesRanker::GetRankedVariableByName (const string label)
{


  //overlaps_sorted_cleaned_name_

  //cout <<" size is " << overlaps_sorted_cleaned_.size ()<<endl;
  for (unsigned int i = 0; i < overlaps_sorted_cleaned_.size (); i++) {
    if (label == overlaps_sorted_cleaned_[i].first) {
      return overlaps_sorted_cleaned_[i].second;
    }
  }
  return -66666.;
}



float
ObservablesRanker::GetRankedVariableByPos (const int position)
{

  return overlaps_sorted_cleaned_[position - 1].second;


  //return -66666.;
}


string ObservablesRanker::GetRankedVariableName (const int position)
{

  return overlaps_sorted_cleaned_[position - 1].first;


  //return -66666.;
}



int
ObservablesRanker::GetRankingofVariable (string label)
{
  for (unsigned int i = 0; i < overlaps_sorted_cleaned_.size (); i++) {
    if (label == overlaps_sorted_cleaned_[i].first) {

      return i + 1;
    }

  }
  return -9999;
}



float
ObservablesRanker::GetCorrelation (string var1_, string var2_)
{




  for (unsigned int i = 0; i < overlaps_sorted_cleaned_name_.size (); i++) {

    int ovv = overlaps_sorted_cleaned_name_.size ();

    if (var1_ == overlaps_sorted_cleaned_name_[i])
      {

	for (unsigned int k = 0; k < overlaps_sorted_cleaned_name_.size (); k++) {


	  if (var2_ == overlaps_sorted_cleaned_name_[k]) {

	    // cout<<"  ADASDASSD "<<endl;
	    return correlation_sorted_cleaned_[k + ovv * i];
	  }
	}
      }
  }

  return -9999;
}
