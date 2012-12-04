#include "../interface/MakeBinning.h"
MakeBinning::MakeBinning ()
{
  label_.clear ();
}

MakeBinning::MakeBinning (const vector < string > &vec, const vector < pair < float, float > >&range)
{
//  (*this) = MakeBinning ();
  label_ = vec;
  NofVar_ = vec.size ();
  var_ = new float[vec.size ()];

  for (unsigned int i = 0; i < vec.size (); i++)
    {
      variable_string_.push_back (vec[i].c_str ());
      // cout<<" for the MakeBinning Class  JUST CREATED THE VARIABLE "<<vec[i].c_str()<<"  "<< range[i].first<<"  "<< range[i].second<<endl;
      var_[i] = -9999.;


    }

}


MakeBinning::~MakeBinning ()
{;}


/*void 
MakeBinning::Fill (string & var, float & val)
{

		      PlotContentAll_.push_back( pair< string,double > (var,val));
	   cout << " Filled the  " << var<< " with a value "<<val<<endl;

}	     	      
*/
void
MakeBinning::Binning (string &decision,vector < pair < string,float > > & PlotContentAll_,int NBins,int EvtsPerBin,float & weight)
{

  vector < double >values_;

  TFile *taxisFile;
  vector < TAxis* > myAxes;
  myAxes.clear();
  char binfile[100];

  if (decision=="Events")	  
  sprintf(binfile,"Binning_%i_EvtsPerBin.root",EvtsPerBin);
  if (decision=="Bins")
  sprintf(binfile,"Binning_%i_Bins.root",NBins);
  
  taxisFile = new TFile (binfile, "recreate");

  variable_string_.clear();
  PlotContentFraction_.clear ();

  sort(PlotContentAll_.begin(),PlotContentAll_.end());
  
  for (unsigned int jj = 0; jj < PlotContentAll_.size (); jj++){
   
	  variable_string_.push_back(PlotContentAll_[jj].first);}
   
  sort (variable_string_.begin (), variable_string_.end ());
  variable_string_.erase (unique (variable_string_.begin (), variable_string_.end ()),variable_string_.end ());


     ////Loop over variables by string and fill the double with all the events
     //
	  

	  for (unsigned int i = 0; i < variable_string_.size (); i++)
    {//////////////start of looping for all variables


	   //cout << "Will try to make the binning for variable  " << variable_string_[i] <<endl;
	   double valsContent[PlotContentAll_.size ()];
   
      for (unsigned int jj = 0; jj < PlotContentAll_.size (); jj++)
	{

	  if (PlotContentAll_[jj].first == variable_string_[i] && PlotContentAll_[jj].second>-9999)
	    {
	      values_.push_back (PlotContentAll_[jj].second);
	      //cout<<"  Checking that they hold the same values  "<<values_[values_.size()-1]<<"  <--- equal  -->  "<<PlotContentAll_[PlotContentAll_.size()-1].second<<endl;
	 //cout << " Filled the  " << variable_string_[i] << " equal to  " << PlotContentAll_[jj].first <<" with value  "<<PlotContentAll_[jj].second<<"  size "<<PlotContentAll_.size()<<"  "<<jj<<endl;
	    }
	}
     
     sort (values_.begin (), values_.end ());
		
  
  if (decision=="Events"){
      for (unsigned int j = 0; j < values_.size (); j++)
	{
		int num = EvtsPerBin/weight;
	   if (j % num == 0)	  	   
	   { PlotContentFraction_.push_back (values_[j]);
	     //cout << " Filled the j% " << variable_string_[i] << " with normalized bin "<< num <<"  at  "<<j<<endl;
	cout<<" for variable "<<variable_string_[i]<<"  Now made a new bin at ....  "<<j % num <<" j "<<j<<" and value  "<<values_[j]<<" num "<<num<<" size  "<<values_.size()<<" evtsperbin  "<<EvtsPerBin<<endl;	   
	   }
	}
            ////now stacking the last value 

  }///end of decision = "Events"
  else if (decision=="Bins"){
	PlotContentFraction_.push_back (values_[0]);////set the first value as where to start the first bin
      for (unsigned int j = 0; j < values_.size (); j++)
	{

	   if ( j% int(values_.size()/NBins)==0  )	{  	    
	 		 PlotContentFraction_.push_back (values_[j]);
	    cout<<" for variable "<<variable_string_[i]<<"  Now made a new bin at ....  "<<j % (values_.size()/NBins)  <<" j "<<j<<" and value "<<values_[j]<<" ratio "<<values_.size()/NBins<<" size  "<<values_.size()<<" nbins  "<<NBins<<endl;	   
	   }
		}	
    }///end of decision = Bins


           PlotContentFraction_.push_back (values_[values_.size ()-1]);
//and now fill a double for the constructor of TH1
    
	   for (unsigned int b = 0; b < PlotContentFraction_.size (); b++)
	{
        
	  valsContent[b] = PlotContentFraction_[b];

	}

/////////////////////////now merging the last bin with the last-1 bin
         // cout<<"  PlotContentFraction_ "<<valsContent[PlotContentFraction_.size()-2]<<"   "<<valsContent[PlotContentFraction_.size()-1]<<endl;
          	
if (decision=="Events"){ 
	  valsContent[PlotContentFraction_.size () - 2] = valsContent[PlotContentFraction_.size () - 1];
}

/////////and now create some left margins for some variables

  	if (variable_string_[i] == "EtaMuon"
	  || variable_string_[i] == "DeltaPhiMetMuon"
	  || variable_string_[i] == "DeltaPhiMetLeadingJet"
	  || variable_string_[i] == "DeltaPhiHadtopLepTop"
	  || variable_string_[i] == "DeltaRLeptonLeadingJet"
	  || variable_string_[i] == "DeltaRHadtopLepTop"
	  || variable_string_[i] == "DeltaRJet1Jet2"
	  || variable_string_[i] == "DeltaRJet1Jet3" ||  variable_string_[i] == "DeltaRBTagJets")

	valsContent[0] = -5;

      else if (variable_string_[i] == "SumDeltaPhiMuJet12")
	valsContent[0] = -10;

      else if (variable_string_[i] == "HZ4jets")
	valsContent[0] = -5000;

      else if (variable_string_[i] == "HTo(HT+HZ)")
	    valsContent[0] = -1000;
      else
	(valsContent[0] = 0);

      

      //sort (PlotContentFraction_.begin (), PlotContentFraction_.end ());

///////// This is for making a fixed axis range for all vars

   
   if (variable_string_[i] == "EtaMuon"
	  || variable_string_[i] == "DeltaPhiMetMuon"
	  || variable_string_[i] == "DeltaPhiMetLeadingJet"
	  || variable_string_[i] == "DeltaPhiHadtopLepTop"
	  || variable_string_[i] == "DeltaRLeptonLeadingJet"
	  || variable_string_[i] == "DeltaRHadtopLepTop"
	  || variable_string_[i] == "DeltaRJet1Jet2"
	  || variable_string_[i] == "DeltaRJet1Jet3" ||  variable_string_[i] == "DeltaRBTagJets")
		valsContent[PlotContentFraction_.size ()] = 15; 

      else if (variable_string_[i] == "SumDeltaPhiMuJet12"
	       || variable_string_[i] == "RelCaloIso"
	       || variable_string_[i] == "RelTrackIso")
	valsContent[PlotContentFraction_.size ()] = 100;

      else if (variable_string_[i] == "HTo(HT+HZ)")
	valsContent[PlotContentFraction_.size ()] = 1000;

      else if (  variable_string_[i] == "ET1oET2" 
	       ||  variable_string_[i] == "METDivHadTopPt"
	       || variable_string_[i] == "ET1oET3"
	       || variable_string_[i] == "ET1oET4"
	       || variable_string_[i] == "ET2oET3")
		valsContent[PlotContentFraction_.size ()] = 200;  


      else if (variable_string_[i] == "AngleHadTopLepton"
	       || variable_string_[i] == "AngleHadTopLepTop")
		valsContent[PlotContentFraction_.size ()] = 10;

  
      else
       	(valsContent[PlotContentFraction_.size ()] = 5000);
   
///////////this is actually where the binning is decided.....one binning per variable
	  //TAxis *myNewAxis = NULL;

	   for (unsigned int b = 0; b < PlotContentFraction_.size (); b++){


//cout<<variable_string_[i]<<"  bin at "<<valsContent[b]<<" last bin at "<<valsContent[PlotContentFraction_.size()]<<endl;}
    }
          int nbinsTH1 =0;
            
		  nbinsTH1 = PlotContentFraction_.size();
		 TAxis * myNewAxis= new TAxis(nbinsTH1, valsContent);
	
	  myNewAxis->SetName (("Binning_" + variable_string_[i] + "_SM").c_str ());
//	  cout<<"  Just created the TAxis name as  "<<myNewAxis->GetName()<<" with a total of "<<nbinsTH1<<"  and decision is  "<<decision<<"  "<<myNewAxis->GetXmin()<<"   "<<myNewAxis->GetXmax()<<"   "<<myNewAxis->GetNbins()<<endl;
	  myAxes.push_back (myNewAxis);
//////////clear and delete !
     

      for (unsigned int l = 0; l <PlotContentFraction_.size (); l++)
	{
	  // cout<<" now will clear ... "<<l<<"  "<<valsContent[l]<<"  "<<PlotContentAll_.size()<<endl;
	  valsContent[l] = 0;
	}

      values_.clear ();
      PlotContentFraction_.clear();

    }/////////////end of loop for all variables

  // Write the TAxis file 

      taxisFile->cd ();

      cout << "Writing TAxis objects in file " << taxisFile->GetName () << endl;
      for (unsigned int i = 0; i < myAxes.size (); i++)
	{
	
		myAxes[i]->Write ();
	}

       taxisFile->Write ();
       myAxes.clear ();
   

      taxisFile->Close ();
      delete taxisFile;


      
}


void MakeBinning::Binning_forTprimeAnalysis(string myxvariable, string myyvariable, map<string,vector<float > > VariableValuesMap, map<string,int> nbinsMap, vector<float> eventweightvector, string outputfilename){
  cout<<"---- Binning for t' analysis (2D) ----"<<endl;
  TFile *taxisFile;
  vector < TAxis* > myAxes;
  myAxes.clear();
  
  taxisFile = new TFile (outputfilename.c_str(), "RECREATE"); //maybe attach postfix with nbins etc...?
  
  //string xvariable = "HT4jetsMuonMET", yvariable = "MassHadTop"; //dimensions of the 2D plane hardcoded, not good
  string xvariable, yvariable;
  xvariable = myxvariable;
  yvariable = myyvariable;
	
  //obtain which variables are in the map, these are the dimensions of the 2D plane. WARNING: old way, not robust (not used anymore)
  vector<string> lstVar;
  for(map<string,vector<float > >::const_iterator it = VariableValuesMap.begin(); it != VariableValuesMap.end(); it++)
  {  
		//warning!! The ordering of the variables in VariableValuesMap is not necessarily the ordering you call to be filled...
     //cout<<"it->first = "<<it->first<<endl;
		 lstVar.push_back(it->first);
  }	
  if(lstVar.size() != 2)  
    cout<<" The map of variables doesn't contain 2 variables, TO BE SOLVED!"<<endl;
  
  cout << " Will try to make the binning for variable " << xvariable <<endl;
  vector<pair<float,float> > xvariables_withweights;
  for (unsigned int j = 0; j < VariableValuesMap[xvariable].size (); j++){
     xvariables_withweights.push_back(make_pair(VariableValuesMap[xvariable][j],eventweightvector[j]));
  }
  //vector<float> dummyvector
////  cout<<"size = "<<xvariables_withweights.size ()<<", eventweightvector = "<<eventweightvector.size ()<<endl;
  sort (xvariables_withweights.begin (), xvariables_withweights.end ()); //sort the vector according to the variable value (first of pair, and low to high (<) is the default operator in this C++ function)	
  unsigned int xarraysize = nbinsMap[xvariable] + 2; //2 'extra' comes from: 1 'artificial' underflowbin (for the plots, should be smaller than or equal to the physical boundary), and 1 more than the numbers of bins because you have to specify the edges...
  double xbins[xarraysize];
  //xbins[0] = xvariables[0];//can be changed later //warning, possible conflicts when not changed?
  xbins[0] = 0;
  unsigned int index = 1, alternator = 0;
  float sum_eventweights_bin = 0, totalsum_eventweights = 0, sum_eventweights_updated = 0;
  for(unsigned int w = 0; w < xvariables_withweights.size (); w++)
  {
     totalsum_eventweights = totalsum_eventweights + xvariables_withweights[w].second;
  }
////  cout<<" totalsum_eventweights = "<<totalsum_eventweights<<", xvariables_withweights.size () = "<<xvariables_withweights.size ()<<endl;


	xbins[index] = xvariables_withweights[0].first;
	index++;
////////  cout<<" for variable "<<xvariable<<", made a new bin at 'event' j = "<<0<<", variable value = "<<xvariables_withweights[0].first<<", sum_eventweights_bin = "<<sum_eventweights_bin<<", sum of weights updated = " << sum_eventweights_updated << ", total sum of weights = "<<totalsum_eventweights<<", nbins = "<<nbinsMap[xvariable]<<", and constructed number of events/bin = "<<totalsum_eventweights/nbinsMap[xvariable]<<endl;	   
  sum_eventweights_bin = 0;
  for (unsigned int j = 0; j < xvariables_withweights.size (); j++)
  {  		
       sum_eventweights_bin = sum_eventweights_bin + xvariables_withweights[j].second;
////////       cout<<" ... sum_eventweights_bin = "<<sum_eventweights_bin<<endl;
       sum_eventweights_updated = sum_eventweights_updated + xvariables_withweights[j].second; //is not resetted, unlike sum_eventweights_bin
		   if ((sum_eventweights_bin / (totalsum_eventweights/nbinsMap[xvariable])) >= 1)
       {
		 	   if(alternator % 2 == 1)
			   { //new! alternating between "exceeding" or "just not exceeding" the 'constructed number of events/bin'; for an odd j, go one step back in the loop to place the bin boundary at the event just *before* the sum_eventweights_bin exceeds the 'constructed number of events/bin'
		  		 sum_eventweights_bin = sum_eventweights_bin - xvariables_withweights[j].second;
					 sum_eventweights_updated = sum_eventweights_updated - xvariables_withweights[j].second;
////////					 cout<<" Changing sum_eventweights_bin to " << sum_eventweights_bin << endl;
			  	 sum_eventweights_updated = sum_eventweights_updated - xvariables_withweights[j].second;
			     j = j - 1;
				 }
			   alternator++;
	 		   //xbins[index] = xvariables_withweights[j].first; //old
	 		   xbins[index] = xvariables_withweights[j].first;
			   index++;
////////   	 	   cout<<" for variable "<<xvariable<<", made a new bin at 'event' j = "<<j<<", variable value = "<<xvariables_withweights[j].first<<", sum_eventweights_bin = "<<sum_eventweights_bin<<", sum of weights updated = " << sum_eventweights_updated << ", total sum of weights = "<<totalsum_eventweights<<", nbins = "<<nbinsMap[xvariable]<<", and constructed number of events/bin = "<<totalsum_eventweights/nbinsMap[xvariable]<<endl;	   
         sum_eventweights_bin = 0;
       }
  }
  
  xbins[xarraysize-1] = xvariables_withweights[xvariables_withweights.size ()-1].first; //by definition this is the last element of the array xbins, and the last element of the vector xvariables

  //remark: what if jet energy scale becomes different (evaluating systematics), and jet energy is shifted below the boundary? -> event just not selected...
  if(xvariable == "HT4jetsMuonMET" || xvariable == "HT")
  {
        xbins[0] = 0;//hardcode left boundaries for some variables, for the plots (not necessarily physical, but should be smaller or equal to the physical boundary)
        xbins[1] = 0; //hardcode left 'physical' boundaries for some variables... how to do this for all variables... should be put in the range of the observable in the observables class??? --> 4 X 35 GeV ((at least) 4 jets of 35 GeV) + 1 X 30 GeV (muon)
  }
	if(xvariable == "MassHadTop" || xvariable == "MTop" ||  xvariable == "Mtop")
  {
        xbins[0] = 0;//hardcode left boundaries for some variables, for the plots (not necessarily physical, but should be smaller or equal to the physical boundary)
        xbins[1] = 0; //hardcode left 'physical' boundaries for some variables... how to do this for all variables... should be put in the range of the observable in the observables class??? --> 4 X 35 GeV ((at least) 4 jets of 35 GeV) + 1 X 30 GeV (muon)
  }
  
  xbins[xarraysize-1] = 2*xbins[xarraysize-2] - xbins[xarraysize-3]; //a choice: in this way the last bin will have equal width as the last but one bin 

  for (unsigned int b = 0; b < xarraysize; b++)
  {        
			cout<<" xbins["<<b<<"] = "<<xbins[b]<<";"<<endl;
  }
  
  int xnbins = 0;
  xnbins = xarraysize - 1; //the last element of xbins should be the upperbound of the 'last bin' actually
  TAxis * myNewAxis= new TAxis(xnbins, xbins);
  myNewAxis->SetName (("Binning_" + xvariable + "_SM").c_str ());
//	  cout<<"  Just created the TAxis name as  "<<myNewAxis->GetName()<<" with a total of "<<xnbins<<", range  "<<myNewAxis->GetXmin()<<"  to "<<myNewAxis->GetXmax()<<", nbins  "<<myNewAxis->GetNbins()<<endl;
  myAxes.push_back (myNewAxis);
  xvariables_withweights.clear();
  
  
  //now making the binnings in the other dimension of the 2D plane, following closely the procedure of before; maybe to be put in a common function of the class
  for(int CurrentxvariableBin = 1;CurrentxvariableBin < nbinsMap[xvariable]+1; CurrentxvariableBin++)
  {
    cout<<"//---> "+xvariable+" bin "<<CurrentxvariableBin<<endl;
    vector<pair<float,float> > yvariables_CurrentxvariableBin_withweights;
    for(unsigned int j = 0; j < VariableValuesMap[xvariable].size(); j++)
    {
       double rightcriterium;
       if(CurrentxvariableBin==nbinsMap[xvariable])
         rightcriterium = xbins[CurrentxvariableBin+1] + 1000000;
       else
         rightcriterium = xbins[CurrentxvariableBin+1];
       
       if(VariableValuesMap[xvariable][j]>=xbins[CurrentxvariableBin] && VariableValuesMap[xvariable][j]<rightcriterium) //then the event is in the 1st bin of the base variable (HT)
       {
          yvariables_CurrentxvariableBin_withweights.push_back(make_pair(VariableValuesMap[yvariable][j],eventweightvector[j]));
       }
  
    }
//    cout<<"  size = "<<yvariables_CurrentxvariableBin_withweights.size()<<endl;
    sort (yvariables_CurrentxvariableBin_withweights.begin (), yvariables_CurrentxvariableBin_withweights.end ());
    unsigned int yarraysize = nbinsMap[yvariable] + 2; //2 'extra' comes from: 1 'artificial' underflowbin (for the plots, should be smaller than or equal to the physical boundary), and 1 more than the numbers of bins because you have to specify the edges...
    double ybins[yarraysize];
    //ybins[0] = VariableValuesMap[yvariable][0];//can be changed later //warning, possible conflicts when not changed?
    ybins[0] = 0;
    index = 1;
    sum_eventweights_bin = 0;
    totalsum_eventweights = 0;
    for(unsigned int w = 0; w < yvariables_CurrentxvariableBin_withweights.size (); w++)
    {
       totalsum_eventweights = totalsum_eventweights + yvariables_CurrentxvariableBin_withweights[w].second;
    }
////    cout<<" totalsum_eventweights = "<<totalsum_eventweights<<", xvariables_withweights.size () = "<<xvariables_withweights.size ()<<endl;
    
		alternator = 0;
		sum_eventweights_bin = 0;
		sum_eventweights_updated = 0;
		ybins[index] = yvariables_CurrentxvariableBin_withweights[0].first;
	  index++;
////////    cout<<" for variable "<<yvariable<<", made a new bin at 'event' j = "<<0<<", variable value = "<<yvariables_CurrentxvariableBin_withweights[0].first<<", sum_eventweights_bin = "<<sum_eventweights_bin <<", total sum of weights = "<<totalsum_eventweights<<", nbins = "<<nbinsMap[yvariable]<<", and constructed number of events/bin = "<<totalsum_eventweights/nbinsMap[yvariable]<<endl;	   
    for (unsigned int j = 0; j < yvariables_CurrentxvariableBin_withweights.size (); j++)
    {
       sum_eventweights_bin = sum_eventweights_bin + yvariables_CurrentxvariableBin_withweights[j].second;
////////       cout<<" ... sum_eventweights_bin = "<<sum_eventweights_bin<<endl;
       sum_eventweights_updated = sum_eventweights_updated + yvariables_CurrentxvariableBin_withweights[j].second; //is not resetted, unlike sum_eventweights_bin		   
			 if ((sum_eventweights_bin / (totalsum_eventweights/nbinsMap[yvariable])) >= 1)
       { 	    			
					if(alternator % 2 == 1)
			   	{ //new! alternating between "exceeding" or "just not exceeding" the 'constructed number of events/bin'; for an odd j, go one step back in the loop to place the bin boundary at the event just *before* the sum_eventweights_bin exceeds the 'constructed number of events/bin'
		  		 	sum_eventweights_bin = sum_eventweights_bin - yvariables_CurrentxvariableBin_withweights[j].second;
					 	sum_eventweights_updated = sum_eventweights_updated - yvariables_CurrentxvariableBin_withweights[j].second;
////////					 	cout<<" Changing sum_eventweights_bin to " << sum_eventweights_bin << endl;
			   	  j = j - 1;
					}
			   	alternator++;
	 		   	ybins[index] = yvariables_CurrentxvariableBin_withweights[j].first;
			   	index++;
////////   	 	   	cout<<" for variable "<<yvariable<<", made a new bin at 'event' j = "<<j<<", variable value = "<<yvariables_CurrentxvariableBin_withweights[j].first<<", sum_eventweights_bin = "<<sum_eventweights_bin<<", sum of weights updated = " << sum_eventweights_updated << ", total sum of weights = "<<totalsum_eventweights<<", nbins = "<<nbinsMap[xvariable]<<", and constructed number of events/bin = "<<totalsum_eventweights/nbinsMap[yvariable]<<endl;	   
         	sum_eventweights_bin = 0;
       }
    }			
    ybins[yarraysize-1] = yvariables_CurrentxvariableBin_withweights[yvariables_CurrentxvariableBin_withweights.size ()-1].first; //by definition this is the last element of the array xbins, and the last element of the vector VariableValuesMap[xvariable]

    if(yvariable == "MassHadTop" || yvariable == "MTop" ||  yvariable == "Mtop")
    {
        ybins[0] = 0;//hardcode left boundaries for some variables, for the plots (not necessarily physical, but should be smaller or equal to the physical boundary)
        ybins[1] = 0; //hardcode left 'physical' boundaries for some variables... how to do this for all variables... should be put in the range of the observable in the observables class??? --> 4 X 35 GeV ((at least) 4 jets of 35 GeV) + 1 X 30 GeV (muon)
    }
		if(yvariable == "HT4jetsMuonMET" || yvariable == "HT")
  	{
        ybins[0] = 0;//hardcode left boundaries for some variables, for the plots (not necessarily physical, but should be smaller or equal to the physical boundary)
        ybins[1] = 0; //hardcode left 'physical' boundaries for some variables... how to do this for all variables... should be put in the range of the observable in the observables class??? --> 4 X 35 GeV ((at least) 4 jets of 35 GeV) + 1 X 30 GeV (muon)
  	}
  
    ybins[yarraysize-1] = 2*ybins[yarraysize-2] - ybins[yarraysize-3]; //a choice: in this way the last bin will have equal width as the last but one bin 

    for (unsigned int b = 0; b < yarraysize; b++)
    {        
				//cout<<" ybins_xbin[\"xbin"<<CurrentxvariableBin<<"\"]"<<"["<<b<<"] = "<<ybins[b]<<";"<<endl;
    		cout<<" ybins_map[\"xbin"<<CurrentxvariableBin<<"\"].push_back("<<ybins[b]<<");"<<endl;
		}
  
    int ynbins = 0;
    ynbins = yarraysize - 1; //the last element of xbins should be the upperbound of the 'last bin' actually
    TAxis * myNewAxis= new TAxis(ynbins, ybins);
    ostringstream CurrentxvariableBin_sstream;
    CurrentxvariableBin_sstream << CurrentxvariableBin;	
    myNewAxis->SetName (("Binning_" + yvariable + "_SM_" + xvariable + "bin" + CurrentxvariableBin_sstream.str()).c_str ());
  //	  cout<<"  Just created the TAxis name as  "<<myNewAxis->GetName()<<" with a total of "<<ynbins<<", range  "<<myNewAxis->GetXmin()<<"  to "<<myNewAxis->GetXmax()<<", nbins  "<<myNewAxis->GetNbins()<<endl;
    myAxes.push_back (myNewAxis);    
    yvariables_CurrentxvariableBin_withweights.clear(); 
  }

  
  // Write the TAxis file 
  taxisFile->cd ();
  cout << "Writing TAxis objects in file " << taxisFile->GetName () << endl;
  for (unsigned int i = 0; i < myAxes.size (); i++)
  {	
      myAxes[i]->Write ();
  }

  taxisFile->Write ();
  myAxes.clear ();   
  taxisFile->Close ();
  delete taxisFile;
}


