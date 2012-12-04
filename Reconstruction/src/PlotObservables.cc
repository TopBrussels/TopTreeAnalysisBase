#include "../interface/PlotObservables.h"
PlotObservables::PlotObservables ()
{
  label_.clear ();
  h_variables.clear ();
  h_variables2D.clear ();
  tree = new TTree ("OBS", "list of observables");

  tr = new TTree ("OBS", "list of observables");

}

PlotObservables::PlotObservables (const vector < string > &vec, const vector < pair < float, float > >&range)
{
  (*this) = PlotObservables ();
  label_ = vec;
  NofVar_ = vec.size ();
  var_ = new float[vec.size ()];

  for (unsigned int i = 0; i < vec.size (); i++)
    {
      h_variables.
	push_back (TH1F
		   (vec[i].c_str (), vec[i].c_str (), 200, range[i].first,
		    range[i].second));
      variable_string_.push_back (vec[i].c_str ());
      var_[i] = -8888.;


    }



}


PlotObservables::PlotObservables (const PlotObservables & pobs)
{
  h_variables = pobs.h_variables;
  h_variables2D = pobs.h_variables2D;
  label_ = pobs.label_;


}


PlotObservables::~PlotObservables ()
{
}


void
PlotObservables::Fill (const Observables & obs, float & weight,bool &fill_)
{


if (fill_){
  for (unsigned int i = 0; i < h_variables.size (); i++)
    {
      h_variables[i].Fill ((obs.Variables ()[i]).second);

      //PlotContent_.push_back( pair< string,float > ( (obs.Variables ()[i]).first , (obs.Variables ()[i]).second) );
      //cout<<"  "<<i<<"  "<<PlotContent_[i].first<<"  "<<obs.Variables()[i].first<<"  "<<PlotContent_[i].second<<"  "<<(obs.Variables()[i]).second<<endl;

    }
}

}

void
PlotObservables::Normalized ()
{
  for (unsigned int i = 0; i < h_variables.size (); i++)
    if (h_variables[i].Integral () > 0)
      h_variables[i].Scale (1 / h_variables[i].Integral ());
}


void
PlotObservables::Write(TString  & setname,  string & decision,const vector < pair < string,float > >&PlotContent_, int NBins, int evtsPerBin,float & weight,bool normalized)
{
if (decision=="Bins") void WriteNBins(TString  & setname,  const vector < pair < string,float > >&PlotContent_, int NBins,float & weight,bool normalized);
else if (decision=="Events") void  WriteEvtsPerBin(TString  & setname,  const vector < pair < string,float > >&PlotContent_, int evtsPerBin,float & weight,bool normalized);

}


void
PlotObservables::WriteEvtsPerBin(TString  & setname,  const vector < pair < string,float > >&PlotContent_, int evtsPerBin,float & weight,bool normalized)
{



vector < double >values_;
  TString file_nobins = setname + "_nobins.root";
  TString file_binned = setname + ".root";

cout <<"  -------->>>>>>>>>>>>>>>> The plots will be for EvtsPerBin  and file "<<setname<<" file _ "<<file_nobins<<"   "<<file_binned<<"   "<<PlotContent_.size()<<endl;
//sprintf(file_nobins,"%s",setname.c_str());
  TFile *root_nobins = new TFile (file_nobins, "recreate");
  TFile *root_bins = new TFile (file_binned, "recreate");

vector < TAxis* > myAxes;
char binfile[100];
sprintf(binfile,"../config/Binning.root");


  sort (variable_string_.begin (), variable_string_.end ());
  variable_string_.erase (unique (variable_string_.begin (), variable_string_.end ()),variable_string_.end ());
  PlotContentFraction_.clear ();

    map < string, TAxis * >mapAxis;
 
    //////////Reading the Binning for sample other than TTJets

     TFile fBinning(binfile,"READ");
     for (vector < string >::iterator iter =  variable_string_.begin (); iter !=  variable_string_.end (); iter++) {
    TAxis *axisSM = NULL;
    char tAxisName[150];
    sprintf (tAxisName, "Binning_%s_SM", iter->c_str ());
    fBinning.GetObject (tAxisName, axisSM);
  
    if (axisSM == NULL)
      {
      cerr<<" The axis is NULL while creating binned histos. Please check that the "<<binfile<<" exists in your local area !!!!!! "<<endl;
      mapAxis.insert (make_pair (*iter, (TAxis *) NULL));
    
      }
    else{
 
      mapAxis.insert (make_pair (*iter, new TAxis (*axisSM))); 
  
        }



     }  
fBinning.Close ();

TH1F *hnew[variable_string_.size()];
      
     ////Loop over variables
  for (unsigned int i = 0; i < variable_string_.size (); i++)
    {
      double valsContent[PlotContent_.size ()];

 TAxis *axis = mapAxis[variable_string_[i]];
    
 //     TAxis *axisrange = mapAxis[variable_string_[i].c_str()];
      PlotContentFraction_.clear ();
      values_.clear ();
   
      for (unsigned int j = 0; j < PlotContent_.size (); j++)
	{

	  if (PlotContent_[j].first == variable_string_[i] && PlotContent_[j].second>-9999)
	    {
	      values_.push_back (PlotContent_[j].second);

	    }


                                                         

	}
     
  
  sort (values_.begin (), values_.end ());

   



    hnew[i] =new TH1F (variable_string_[i].c_str (), variable_string_[i].c_str (),axis->GetNbins(), axis->GetXbins ()->fArray );
  
//	cout<<" and now ploting... "<<setname<<"  "<<variable_string_[i]<<"  bins "<<axis->GetNbins()<<endl;
	for (unsigned int n = 0; n < values_.size (); n++)
	{
	  hnew[i]->Fill (values_[n]);
	}
	
 for (unsigned int n = 0; n < values_.size (); n++)
	{
	 
	  	  if ( values_[n]>hnew[i]->GetBinLowEdge(hnew[i]->GetNbinsX()-1)) {

     
		hnew[i]->SetBinContent(hnew[i]->GetNbinsX()-2, hnew[i]->GetBinContent(hnew[i]->GetNbinsX()-2)+hnew[i]->GetBinContent(hnew[i]->GetNbinsX()-1 ));
		hnew[i]->SetBinContent(hnew[i]->GetNbinsX()-1,0);

//	cout<<" and now is again... "<<setname<<"  "<<variable_string_[i]<<"  value is "<< values_[values_.size()-2]<<"   "<<values_[values_.size()-1]<<"  "<< hnew[i]->GetBinContent(hnew[i]->GetNbinsX()-1)<<"   "<< hnew[i]->GetBinContent(hnew[i]->GetNbinsX())<<endl;

                   }

	      
 	}///end of merging the bin content for every bin if there is value after the last TTJets bin
     
//		hnew[i]->SetBinContent(hnew[i]->GetNbinsX(),0);
		
      values_.clear ();

      for (unsigned int l = 0; l < PlotContentFraction_.size (); l++)
	{
	  // cout<<" now will clear ... "<<l<<"  "<<valsContent[l]<<"  "<<PlotContent_.size()<<endl;
	  valsContent[l] = 0;
	}

      PlotContentFraction_.clear ();



    }				/////////////end of loop in variables


      root_bins->cd ();
      for (unsigned int ii=0;ii<variable_string_.size();ii++){
      if (normalized){
      if (hnew[ii]->Integral () > 0)
      hnew[ii]->Scale (1 / hnew[ii]->Integral ());}
      hnew[ii]->Write();
      }
          


      for (unsigned int ik=0;ik<variable_string_.size();ik++){
      delete hnew[ik];
      }
 
  root_bins->Write ();
  root_bins->Close ();
  root_nobins->cd ();

  if (normalized)
    Normalized ();
  for (unsigned int i = 0; i < h_variables.size (); i++)
    {
      h_variables[i].Write ();
    }
    
  vector<pair<string,float> > BinContentObsPerSample; //has to be float!!! (double problems with writing in ttree...)
  BinContentObsPerSample = PlotContent_;

  //writing bin contents with string (observable name) of the selected events of the sample in the rootfile
  TTree *Tree_BinContentObsPerSample = new TTree("BinContentObsPerSample","List of Bin Contents for Observables");  
  Tree_BinContentObsPerSample->Branch("BinContentObsPerSample",&BinContentObsPerSample);
  Tree_BinContentObsPerSample->Fill();
  Tree_BinContentObsPerSample->Write();

   root_nobins->Close ();

}


void
PlotObservables::WriteNBins (TString & setname, const vector < pair < string,float > >&PlotContent_, int NBins,float & weight,bool normalized)
{

//cout<<"  ------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BINS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<setname<<endl;
  vector < double >values_;

  TString file_nobins = setname + "_nobins.root";
  TString file_binned = setname + ".root";

  //sprintf(file_nobins,"%s",setname.c_str());
  TFile *root_nobins = new TFile (file_nobins, "recreate");
  TFile *root_bins = new TFile (file_binned, "recreate");

vector < TAxis* > myAxes;
char binfile[100];
//if (decision=="Bins")
//sprintf(binfile,"Binning_%i_Bins.root",NBins);
sprintf(binfile,"../config/Binning.root");
//if (decision=="Events")
//sprintf(binfile,"Binning_%i_EvtsPerBin.root",evtsPerBin);


  sort (variable_string_.begin (), variable_string_.end ());
  variable_string_.erase (unique (variable_string_.begin (), variable_string_.end ()),variable_string_.end ());
  PlotContentFraction_.clear ();

    map < string, TAxis * >mapAxis;
 
    //////////Reading the Binning for sample other than TTJets

     TFile fBinning(binfile,"READ");
     for (vector < string >::iterator iter =  variable_string_.begin (); iter !=  variable_string_.end (); iter++) {
    TAxis *axisSM = NULL;
    char tAxisName[150];
    sprintf (tAxisName, "Binning_%s_SM", iter->c_str ());
    fBinning.GetObject (tAxisName, axisSM);
  
    if (axisSM == NULL)
      {
      cerr<<" The axis is NULL while creating binned histos. Please check that the "<<binfile<<" exists in your local area !!!!!! "<<endl;
      mapAxis.insert (make_pair (*iter, (TAxis *) NULL));
    
      }
    else{
 
      mapAxis.insert (make_pair (*iter, new TAxis (*axisSM))); 
  
        }
     }  
      fBinning.Close ();

TH1F *hnew[variable_string_.size()];
      
     ////Loop over variables
  for (unsigned int i = 0; i < variable_string_.size (); i++)
    {
      double valsContent[PlotContent_.size ()];

 TAxis *axis = mapAxis[variable_string_[i]];
    
 //     TAxis *axisrange = mapAxis[variable_string_[i].c_str()];
      PlotContentFraction_.clear ();
      values_.clear ();
   
      for (unsigned int j = 0; j < PlotContent_.size (); j++)
	{

	  if (PlotContent_[j].first == variable_string_[i] && PlotContent_[j].second>-9999)
	    {
	      values_.push_back (PlotContent_[j].second);

	    }
	}
     
     sort (values_.begin (), values_.end ());

   
    hnew[i] =new TH1F (variable_string_[i].c_str (), variable_string_[i].c_str (),axis->GetNbins(), axis->GetXbins ()->fArray );


	for (unsigned int n = 0; n < values_.size (); n++)
	{
	  hnew[i]->Fill (values_[n]);
	}

 for ( int n = 1; n < hnew[i]->GetNbinsX()-1; n++){
        float bin_conn;
	bin_conn=0;
	bin_conn = hnew[i]->GetBinContent(n+1);
	 hnew[i]->SetBinContent(n,bin_conn);

 cout<<"hnew[i]  set new bincontents"<<hnew[i]->GetName()<<" nbins "<<hnew[i]->GetNbinsX()<<" h cont "<<hnew[i]->GetBinContent(n)<<" <-- equal ?   "<<bin_conn<<" n  "<<n<<endl;
 
 }
		
      values_.clear ();

      for (unsigned int l = 0; l < PlotContentFraction_.size (); l++)
	{
	  // cout<<" now will clear ... "<<l<<"  "<<valsContent[l]<<"  "<<PlotContent_.size()<<endl;
	  valsContent[l] = 0;
	}

      PlotContentFraction_.clear ();



    }				/////////////end of loop in variables


      root_bins->cd ();
      for (unsigned int ii=0;ii<variable_string_.size();ii++){
      if (normalized){
      if (hnew[ii]->Integral () > 0)
      hnew[ii]->Scale (1 / hnew[ii]->Integral ());}
      hnew[ii]->Write();
      }
          


      for (unsigned int ik=0;ik<variable_string_.size();ik++){
      delete hnew[ik];
      }
 
  root_bins->Write ();
  root_bins->Close ();
 
  root_nobins->cd ();

  if (normalized)
    Normalized ();
  for (unsigned int i = 0; i < h_variables.size (); i++)
    {
      h_variables[i].Write ();
    }

  vector<pair<string,float> > BinContentObsPerSample; //has to be float!!! (double problems with writing in ttree...)
  BinContentObsPerSample = PlotContent_;

  //writing bin contents with string (observable name) of the selected events of the sample in the rootfile
  TTree *Tree_BinContentObsPerSample = new TTree("BinContentObsPerSample","List of Bin Contents for Observables");  
  Tree_BinContentObsPerSample->Branch("BinContentObsPerSample",&BinContentObsPerSample);
  Tree_BinContentObsPerSample->Fill();
  Tree_BinContentObsPerSample->Write();
  
   root_nobins->Close ();

}









void
PlotObservables::Write (string & setname, bool normalized)
{


  if (normalized)
    Normalized ();
  //char dirname[50];
  

  string file_nobins = setname + ".root";
  //sprintf(file_nobins,"%s",setname.c_str());
  TFile *root_nobins = new TFile (file_nobins.c_str (), "recreate");

  //TDirectory *cddirs;

  root_nobins->cd ();

  // sprintf(dirname,"%s",setname.c_str());
  //cddirs = fout->mkdir(dirname);
  //cddirs=root_nobins->mkdir("Observables");
  //cddirs=root_nobins->cd();
  for (unsigned int i = 0; i < h_variables.size (); i++)
    {
      h_variables[i].Write ();
    }

  root_nobins->Close ();

}
