#include "../interface/PlotTopology.h"


void PlotTopology::InitStruct(histo &h, string name){
	stringstream name_stream;
	string wholename;
	const char* name_char;
	
	name_stream << name << "Multiplicity";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.Multiplicity = new TH1F(name_char,name_char,6,0.,6.);
	h.Multiplicity->GetXaxis()->SetTitle("Multiplicity");
	name_stream.str("");
	
	name_stream << name << "Pt";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.Pt = new TH1F(name_char,name_char,100,0.,1000.);
	h.Pt->GetXaxis()->SetTitle("Pt (GeV/c)");
	name_stream.str("");
	
	name_stream << name << "Eta";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.Eta = new TH1F(name_char,name_char,50,-5.,5.);
	h.Eta->GetXaxis()->SetTitle("#eta");
	name_stream.str("");
	
	name_stream << name << "Phi";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.Phi = new TH1F(name_char,name_char,50,-3.15,3.15);
	h.Phi->GetXaxis()->SetTitle("#phi");
	name_stream.str("");
	
	name_stream << name << "Gamma";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.Gamma = new TH1F(name_char,name_char,50,1.,1200.);
	h.Gamma->GetXaxis()->SetTitle("#Gamma");
	name_stream.str("");
} 

void PlotTopology::InitStruct(histomultibody &h, string name){
	stringstream name_stream;
	string wholename;
	const char* name_char;
	
	name_stream << name << "InvariantMass";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.InvMass = new TH1F(name_char,name_char,100,0.,300.);
	h.InvMass->GetXaxis()->SetTitle("InvariantMass");
	name_stream.str("");
	
	name_stream << name << "DeltaR";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.DeltaR = new TH1F(name_char,name_char,50,0.,5.);
	h.DeltaR->GetXaxis()->SetTitle("#DeltaR");
	name_stream.str("");
	
	name_stream << name << "MinDeltaR";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.MinDeltaR = new TH1F(name_char,name_char,50,0.,5.);
	h.MinDeltaR->GetXaxis()->SetTitle("Min#DeltaR");
	name_stream.str("");
	
	name_stream << name << "MaxDeltaR";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.MaxDeltaR = new TH1F(name_char,name_char,50,0.,15.);
	h.MaxDeltaR->GetXaxis()->SetTitle("Max#DeltaR");
	name_stream.str("");
	
	name_stream << name << "Ht";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.Ht = new TH1F(name_char,name_char,100,0.,2000.);
	h.Ht->GetXaxis()->SetTitle("Ht");
	name_stream.str("");
	
	name_stream << name << "DeltaR_vs_Pt";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.DeltaR_vs_Pt = new TH2F(name_char,name_char,50,0.,5.,50,0.,1500.);
	h.DeltaR_vs_Pt->GetXaxis()->SetTitle("#DeltaR");
	h.DeltaR_vs_Pt->GetYaxis()->SetTitle("Pt (GeV/c)");
	name_stream.str("");
	
	name_stream << name << "DeltaR_vs_Eta";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.DeltaR_vs_Eta = new TH2F(name_char,name_char,50,0.,5.,50,0.,5.);
	h.DeltaR_vs_Eta->GetXaxis()->SetTitle("#DeltaR");
	h.DeltaR_vs_Eta->GetYaxis()->SetTitle("Eta");
	name_stream.str("");
	
	name_stream << name << "DeltaR_vs_Gamma";
	wholename = name_stream.str();
	name_char = wholename.c_str();
	h.DeltaR_vs_Gamma = new TH2F(name_char,name_char,50,0.,5.,10,0.,50.);
	h.DeltaR_vs_Gamma->GetXaxis()->SetTitle("#DeltaR");
	h.DeltaR_vs_Gamma->GetYaxis()->SetTitle("Gamma");
	name_stream.str("");
}
  
PlotTopology::PlotTopology(){
 nEvents = 0;
 nSelectedEvents = 0;
 Efficiency = 1.;

 InitStruct(ElectronHisto,"h_Electron");
 InitStruct(MuonHisto,"h_Muon");
 InitStruct(TauonHisto,"h_Tauon");
 
 InitStruct(ElectronneutrinoHisto,"h_Electronneutrino");
 InitStruct(MuonneutrinoHisto,"h_Muonneutrino");
 InitStruct(TauonneutrinoHisto,"h_Tauonneutrino"); 
 
 InitStruct(GluinoHisto,"h_Gluino");
 
 InitStruct(StopHisto,"h_Stop");
 
 InitStruct(TopHisto,"h_Top");
 
 InitStruct(LeptonsHisto,"h_Leptons");
 InitStruct(TopqqHisto,"h_Topqq");
 InitStruct(Topqb_1Histo,"h_Topqb_1");
 InitStruct(Topqb_2Histo,"h_Topqb_2");
 InitStruct(ToplnuHisto,"h_Toplnu");
 InitStruct(ToplbHisto,"h_Toplb");
 InitStruct(TopnubHisto,"h_Topnub");
 
 // the particles mentioned left of 'vs' in the string correspond to the particles 
 // of which you calculate the variable on the left of 'vs' in the name of the 
 // member of the histomultiplebody struct; the particle on the right of 'vs' in 
 // the string is the particle corresponding to the variable right of  'vs' in the 
 // name of the member of the histomultiplebody struct.
 InitStruct(Topqq_vs_TopHisto,"h_TopqqvsTop_"); 
 InitStruct(Topqq_vs_WHisto,"h_TopqqvsW_");
 
 InitStruct(TopqqbHisto,"h_Topqqb_");
 
 InitStruct(leadingmuqHisto,"h_leadingmuq");
 
 /*
 h_MinDeltaRqqb = new TH1F("h_MinDeltaRqqb","Min#DeltaR(q,#bar{q},b)",50,0.,5.);
 h_MinDeltaRqqb->GetXaxis()->SetTitle("Min#DeltaR(q,#bar{q},b)");
 h_MinDeltaRqqb->GetYaxis()->SetTitle("U.A.");
 h_MinDeltaRqqb_cut = new TH1F("h_MinDeltaRqqb_cut","Min#DeltaR(q,#bar{q},b) - cut",50,0.,5.);
 h_MinDeltaRqqb_cut->GetXaxis()->SetTitle("Min#DeltaR(q,#bar{q},b)");
 h_MinDeltaRqqb_cut->GetYaxis()->SetTitle("U.A.");
 h_MinDeltaRqqb_vs_PtTop = new TH2F("h_MinDeltaRqqb_vs_PtTop","Min#DeltaR(q,#bar{q},b) vs P_{t}^{top}",50,0.,5.,50,0,1500);
 h_MinDeltaRqqb_vs_PtTop->GetXaxis()->SetTitle("Min#DeltaR(q,#bar{q},b)");
 h_MinDeltaRqqb_vs_PtTop->GetYaxis()->SetTitle("P_{t}^{top}");
 h_MinDeltaRqqb_vs_EtaTop = new TH2F("h_MinDeltaRqqb_vs_EtaTop","Min#DeltaR(q,#bar{q},b) vs Eta^{top}",50,0.,5.,50,0,5.);
 h_MinDeltaRqqb_vs_EtaTop->GetXaxis()->SetTitle("Min#DeltaR(q,#bar{q},b)");
 h_MinDeltaRqqb_vs_EtaTop->GetYaxis()->SetTitle("Eta_{t}^{top}");
 h_MinDeltaRqqb_vs_GammaTop = new TH2F("h_MinDeltaRqqb_vs_GammaTop","Min#DeltaR(q,#bar{q},b) vs #Gamma^{top}",50,0.,5.,10,0,20);
 h_MinDeltaRqqb_vs_GammaTop->GetXaxis()->SetTitle("Min#DeltaR(q,#bar{q},b)");
 h_MinDeltaRqqb_vs_GammaTop->GetYaxis()->SetTitle("#Gamma^{top}");
 
 h_MaxDeltaRqqb = new TH1F("h_MaxDeltaRqqb","Max#DeltaR(q,#bar{q},b)",50,0.,5.);
 h_MaxDeltaRqqb->GetXaxis()->SetTitle("Max#DeltaR(q,#bar{q},b)");
 h_MaxDeltaRqqb->GetYaxis()->SetTitle("U.A.");
 h_MaxDeltaRqqb_cut = new TH1F("h_MaxDeltaRqqb_cut","Max#DeltaR(q,#bar{q},b) - cut",50,0.,5.);
 h_MaxDeltaRqqb_cut->GetXaxis()->SetTitle("Max#DeltaR(q,#bar{q},b)");
 h_MaxDeltaRqqb_cut->GetYaxis()->SetTitle("U.A.");
 h_MaxDeltaRqqb_vs_PtTop = new TH2F("h_MaxDeltaRqqb_vs_PtTop","Max#DeltaR(q,#bar{q},b) vs P_{t}^{top}",50,0.,5.,50,0,1500);
 h_MaxDeltaRqqb_vs_PtTop->GetXaxis()->SetTitle("Max#DeltaR(q,#bar{q},b)");
 h_MaxDeltaRqqb_vs_PtTop->GetYaxis()->SetTitle("P_{t}^{top}");
 h_MaxDeltaRqqb_vs_EtaTop = new TH2F("h_MaxDeltaRqqb_vs_EtaTop","Max#DeltaR(q,#bar{q},b) vs Eta^{top}",50,0.,5.,50,0,5.);
 h_MaxDeltaRqqb_vs_EtaTop->GetXaxis()->SetTitle("Max#DeltaR(q,#bar{q},b)");
 h_MaxDeltaRqqb_vs_EtaTop->GetYaxis()->SetTitle("Eta_{t}^{top}");
 h_MaxDeltaRqqb_vs_GammaTop = new TH2F("h_MaxDeltaRqqb_vs_GammaTop","Max#DeltaR(q,#bar{q},b) vs #Gamma^{top}",50,0.,5.,10,0,20);
 h_MaxDeltaRqqb_vs_GammaTop->GetXaxis()->SetTitle("Max#DeltaR(q,#bar{q},b)");
 h_MaxDeltaRqqb_vs_GammaTop->GetYaxis()->SetTitle("#Gamma^{top}");
 
 h_DeltaRqq = new TH1F("h_DeltaRqq","#DeltaR(q,#bar{q})",50,0.,5.);
 h_DeltaRqq->GetXaxis()->SetTitle("#DeltaR(q,#bar{q})");
 h_DeltaRqq->GetYaxis()->SetTitle("U.A.");
 h_DeltaRqq_cut = new TH1F("h_DeltaRqq_cut","#DeltaR(q,#bar{q}) - cut",50,0.,5.);
 h_DeltaRqq_cut->GetXaxis()->SetTitle("#DeltaR(q,#bar{q})");
 h_DeltaRqq_cut->GetYaxis()->SetTitle("U.A.");
 h_DeltaRqq_vs_PtTop = new TH2F("h_DeltaRqq_vs_PtTop","#DeltaR(q,#bar{q}) vs P_{t}^{top}",50,0.,5.,50,0,1500);
 h_DeltaRqq_vs_PtTop->GetXaxis()->SetTitle("#DeltaR(q,#bar{q})");
 h_DeltaRqq_vs_PtTop->GetYaxis()->SetTitle("P_{t}^{top}");
 h_DeltaRqq_vs_EtaTop = new TH2F("h_DeltaRqq_vs_EtaTop","#DeltaR(q,#bar{q}) vs Eta^{top}",50,0.,5.,50,0,5.);
 h_DeltaRqq_vs_EtaTop->GetXaxis()->SetTitle("#DeltaR(q,#bar{q})");
 h_DeltaRqq_vs_EtaTop->GetYaxis()->SetTitle("Eta_{t}^{top}");
 h_DeltaRqq_vs_GammaTop = new TH2F("h_DeltaRqq_vs_GammaTop","#DeltaR(q,#bar{q}) vs #Gamma^{top}",50,0.,5.,10,0,20);
 h_DeltaRqq_vs_GammaTop->GetXaxis()->SetTitle("#DeltaR(q,#bar{q})");
 h_DeltaRqq_vs_GammaTop->GetYaxis()->SetTitle("#Gamma^{top}");
 h_DeltaRqq_vs_PtW = new TH2F("h_DeltaRqq_vs_PtW","#DeltaR(q,#bar{q}) vs P_{t}^{w}",50,0.,5.,50,0,1500);
 h_DeltaRqq_vs_PtW->GetXaxis()->SetTitle("#DeltaR(q,#bar{q})");
 h_DeltaRqq_vs_PtW->GetYaxis()->SetTitle("P_{t}^{w}");
 h_DeltaRqq_vs_EtaW = new TH2F("h_DeltaRqq_vs_EtaW","#DeltaR(q,#bar{q}) vs Eta^{w}",50,0.,5.,50,0,5.);
 h_DeltaRqq_vs_EtaW->GetXaxis()->SetTitle("#DeltaR(q,#bar{q})");
 h_DeltaRqq_vs_EtaW->GetYaxis()->SetTitle("Eta_{t}^{w}");
 h_DeltaRqq_vs_GammaW = new TH2F("h_DeltaRqq_vs_GammaW","#DeltaR(q,#bar{q}) vs #Gamma^{w}",50,0.,5.,10,0,20);
 h_DeltaRqq_vs_GammaW->GetXaxis()->SetTitle("#DeltaR(q,#bar{q})");
 h_DeltaRqq_vs_GammaW->GetYaxis()->SetTitle("#Gamma^{w}");
 */
} 



PlotTopology::PlotTopology(const PlotTopology& psel){
 /*
 h_MinDeltaRqqb = psel.h_MinDeltaRqqb;
 h_MinDeltaRqqb_cut = psel.h_MinDeltaRqqb_cut;
 h_MinDeltaRqqb_vs_PtTop = psel.h_MinDeltaRqqb_vs_PtTop;
 h_MinDeltaRqqb_vs_EtaTop = psel.h_MinDeltaRqqb_vs_EtaTop;
 h_MinDeltaRqqb_vs_GammaTop = psel.h_MinDeltaRqqb_vs_GammaTop;
 
 h_MaxDeltaRqqb = psel.h_MaxDeltaRqqb;
 h_MaxDeltaRqqb_cut = psel.h_MaxDeltaRqqb_cut;
 h_MaxDeltaRqqb_vs_PtTop = psel.h_MaxDeltaRqqb_vs_PtTop;
 h_MaxDeltaRqqb_vs_EtaTop = psel.h_MaxDeltaRqqb_vs_EtaTop;
 h_MaxDeltaRqqb_vs_GammaTop = psel.h_MaxDeltaRqqb_vs_GammaTop;
 
 h_DeltaRqq = psel.h_DeltaRqq;
 h_DeltaRqq_cut = psel.h_DeltaRqq_cut;
 h_DeltaRqq_vs_PtTop = psel.h_DeltaRqq_vs_PtTop;
 h_DeltaRqq_vs_EtaTop = psel.h_DeltaRqq_vs_EtaTop;
 h_DeltaRqq_vs_GammaTop = psel.h_DeltaRqq_vs_GammaTop;
 h_DeltaRqq_vs_PtW = psel.h_DeltaRqq_vs_PtW;
 h_DeltaRqq_vs_EtaW = psel.h_DeltaRqq_vs_EtaW;
 h_DeltaRqq_vs_GammaW = psel.h_DeltaRqq_vs_GammaW;
 */
}

PlotTopology::~PlotTopology(){;}

void PlotTopology::Fill(TRootNPGenEvent& NPgenEvt){	// e.g. running over mSUGRA events (=NewPhysics)
 nEvents++;
 bool isSemiLeptonicTopology = false;
 
 if(doSemiLeptonicTopology){   
   isSemiLeptonicTopology = (NPgenEvt.leptons().size()==1&&NPgenEvt.quarks().size()==4);
 }   
   
 if((!doSemiLeptonicTopology)||isSemiLeptonicTopology){
   nSelectedEvents++;
   
   vector<TRootMCParticle> electrons;
   vector<TRootMCParticle> muons;
   vector<TRootMCParticle> tauons;
   for(unsigned int i=0;i<NPgenEvt.leptons().size();i++){
	if(fabs(NPgenEvt.leptons()[i].type())==11){electrons.push_back(NPgenEvt.leptons()[i]);}
	if(fabs(NPgenEvt.leptons()[i].type())==13){muons.push_back(NPgenEvt.leptons()[i]);}
	if(fabs(NPgenEvt.leptons()[i].type())==15){tauons.push_back(NPgenEvt.leptons()[i]);}
   }
   vector<TRootMCParticle> electronneutrinos;
   vector<TRootMCParticle> muonneutrinos;
   vector<TRootMCParticle> tauonneutrinos;
   for(unsigned int i=0;i<NPgenEvt.neutrinos().size();i++){
	if(fabs(NPgenEvt.neutrinos()[i].type())==12){electronneutrinos.push_back(NPgenEvt.neutrinos()[i]);}
	if(fabs(NPgenEvt.neutrinos()[i].type())==14){muonneutrinos.push_back(NPgenEvt.neutrinos()[i]);}
	if(fabs(NPgenEvt.neutrinos()[i].type())==16){tauonneutrinos.push_back(NPgenEvt.neutrinos()[i]);}
   }   
   vector<TRootGenTop> hadronicTops;
   vector<TRootGenTop> leptonicTops;
   for(unsigned int i=0;i<NPgenEvt.tops().size();i++){
   	if(NPgenEvt.tops()[i].isHadronic()) hadronicTops.push_back(NPgenEvt.tops()[i]);
	else leptonicTops.push_back(NPgenEvt.tops()[i]); 
   }
   std::sort(hadronicTops.begin(),hadronicTops.end(),HighestPtGenTop());
   std::sort(leptonicTops.begin(),leptonicTops.end(),HighestPtGenTop());   
   
   
   // electrons
   (ElectronHisto.Multiplicity)->Fill(electrons.size());
   for(unsigned int i=0;i<electrons.size();i++){
   	(ElectronHisto.Pt)->Fill(electrons[i].Pt());
	(ElectronHisto.Eta)->Fill(electrons[i].Eta());
	(ElectronHisto.Phi)->Fill(electrons[i].Phi());
	(ElectronHisto.Gamma)->Fill(electrons[i].Gamma());
   }
   
   // muons
   (MuonHisto.Multiplicity)->Fill(muons.size());
   if(muons.size()>0){ 
     TRootMCParticle leadingMuon = muons[0];
     for(unsigned int i=0;i<muons.size();i++){
   	(MuonHisto.Pt)->Fill(muons[i].Pt());
	(MuonHisto.Eta)->Fill(muons[i].Eta());
	(MuonHisto.Phi)->Fill(muons[i].Phi());
	if(muons[i].Pt()>leadingMuon.Pt()) leadingMuon = muons[i];
     }
     
     float DeltaRleadingmuq = 0.;
     float MinDeltaRleadingmuq = 999.;
     float MaxDeltaRleadingmuq = -999.;
     for(unsigned int i=0;i<NPgenEvt.quarks().size();i++){
   		DeltaRleadingmuq = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)leadingMuon,(TLorentzVector)NPgenEvt.quarks()[i]);
		if(DeltaRleadingmuq<MinDeltaRleadingmuq) MinDeltaRleadingmuq = DeltaRleadingmuq;
		if(DeltaRleadingmuq>MaxDeltaRleadingmuq) MaxDeltaRleadingmuq = DeltaRleadingmuq;
     }
     for(unsigned int i=0;i<NPgenEvt.bquarks().size();i++){
   		DeltaRleadingmuq = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)leadingMuon,(TLorentzVector)NPgenEvt.bquarks()[i]);
		if(DeltaRleadingmuq<MinDeltaRleadingmuq) MinDeltaRleadingmuq = DeltaRleadingmuq;
		if(DeltaRleadingmuq>MaxDeltaRleadingmuq) MaxDeltaRleadingmuq = DeltaRleadingmuq;
     }
     (leadingmuqHisto.MinDeltaR)->Fill(MinDeltaRleadingmuq);  
     (leadingmuqHisto.MaxDeltaR)->Fill(MaxDeltaRleadingmuq);
   }
      
   // tauons
   (TauonHisto.Multiplicity)->Fill(tauons.size());
   for(unsigned int i=0;i<tauons.size();i++){
   	(TauonHisto.Pt)->Fill(tauons[i].Pt());
	(TauonHisto.Eta)->Fill(tauons[i].Eta());
	(TauonHisto.Phi)->Fill(tauons[i].Phi());
   }
   
   // electronneutrinos
   (ElectronneutrinoHisto.Multiplicity)->Fill(electronneutrinos.size());
   for(unsigned int i=0;i<electronneutrinos.size();i++){
   	(ElectronneutrinoHisto.Pt)->Fill(electronneutrinos[i].Pt());
	(ElectronneutrinoHisto.Eta)->Fill(electronneutrinos[i].Eta());
	(ElectronneutrinoHisto.Phi)->Fill(electronneutrinos[i].Phi());
   }
   
   // muonneutrinos
   (MuonneutrinoHisto.Multiplicity)->Fill(muonneutrinos.size());
   for(unsigned int i=0;i<muonneutrinos.size();i++){
   	(MuonneutrinoHisto.Pt)->Fill(muonneutrinos[i].Pt());
	(MuonneutrinoHisto.Eta)->Fill(muonneutrinos[i].Eta());
	(MuonneutrinoHisto.Phi)->Fill(muonneutrinos[i].Phi());
   }
   
   // tauonneutrinos
   (TauonneutrinoHisto.Multiplicity)->Fill(tauonneutrinos.size());
   for(unsigned int i=0;i<tauonneutrinos.size();i++){
   	(TauonneutrinoHisto.Pt)->Fill(tauonneutrinos[i].Pt());
	(TauonneutrinoHisto.Eta)->Fill(tauonneutrinos[i].Eta());
	(TauonneutrinoHisto.Phi)->Fill(tauonneutrinos[i].Phi());
   }
   
   // gluinos
   (GluinoHisto.Multiplicity)->Fill(NPgenEvt.gluinos().size());  // or NPgenEvt.numberOfGluinos()
   for(unsigned int i=0;i<NPgenEvt.gluinos().size();i++){
   	(GluinoHisto.Pt)->Fill(NPgenEvt.gluinos()[i].Pt());
	(GluinoHisto.Eta)->Fill(NPgenEvt.gluinos()[i].Eta());
	(GluinoHisto.Phi)->Fill(NPgenEvt.gluinos()[i].Phi());
   }
   
   // stops
   (StopHisto.Multiplicity)->Fill(NPgenEvt.stops().size());
   for(unsigned int i=0;i<NPgenEvt.stops().size();i++){
   	(StopHisto.Pt)->Fill(NPgenEvt.stops()[i].Pt());
	(StopHisto.Eta)->Fill(NPgenEvt.stops()[i].Eta());
	(StopHisto.Phi)->Fill(NPgenEvt.stops()[i].Phi());
   }
   
   // tops
   (TopHisto.Multiplicity)->Fill(NPgenEvt.tops().size());
   for(unsigned int i=0;i<NPgenEvt.tops().size();i++){
   	(TopHisto.Pt)->Fill(NPgenEvt.tops()[i].Pt());
	(TopHisto.Eta)->Fill(NPgenEvt.tops()[i].Eta());
	(TopHisto.Phi)->Fill(NPgenEvt.tops()[i].Phi());
   }
   if(hadronicTops.size()>0){
      	float TopPt = hadronicTops[0].Pt();
      	float TopEta = hadronicTops[0].Eta();
	float TopGamma = hadronicTops[0].Gamma();

      	float WPt = hadronicTops[0].W().Pt();
      	float WEta = hadronicTops[0].W().Eta();
	float WGamma = hadronicTops[0].W().Gamma();
	
   	float DeltaRqq = ROOT::Math::VectorUtil::DeltaR(hadronicTops[0].quark(),hadronicTops[0].quarkBar()); 
      	float DeltaRqb_1 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)hadronicTops[0].quark(),(TLorentzVector)hadronicTops[0].bquark()); 
      	float DeltaRqb_2 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)hadronicTops[0].quarkBar(),(TLorentzVector)hadronicTops[0].bquark());
	
	//float InvMassqq = ROOT::Math::VectorUtil::InvariantMass((TLorentzVector)hadronicTops[0].quark(),(TLorentzVector)hadronicTops[0].quarkBar());
	float InvMassqqb = (hadronicTops[0].quark() + hadronicTops[0].quarkBar() + hadronicTops[0].bquark()).M(); 
	
	(TopqqHisto.DeltaR)->Fill(DeltaRqq);
	(Topqb_1Histo.DeltaR)->Fill(DeltaRqb_1);
	(Topqb_2Histo.DeltaR)->Fill(DeltaRqb_2);
	
	(Topqq_vs_TopHisto.DeltaR_vs_Pt)->Fill(DeltaRqq,TopPt);
	(Topqq_vs_TopHisto.DeltaR_vs_Eta)->Fill(DeltaRqq,TopEta);
	(Topqq_vs_TopHisto.DeltaR_vs_Gamma)->Fill(DeltaRqq,TopGamma);	
	(Topqq_vs_WHisto.DeltaR_vs_Pt)->Fill(DeltaRqq,WPt);
	(Topqq_vs_WHisto.DeltaR_vs_Eta)->Fill(DeltaRqq,WEta);
	(Topqq_vs_WHisto.DeltaR_vs_Gamma)->Fill(DeltaRqq,WGamma);
	
	(TopqqbHisto.InvMass)->Fill(InvMassqqb);
   }
   if(leptonicTops.size()>0){
   	float DeltaRlnu = ROOT::Math::VectorUtil::DeltaR(leptonicTops[0].lepton(),leptonicTops[0].neutrino()); 
      	float DeltaRlb = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)leptonicTops[0].lepton(),(TLorentzVector)leptonicTops[0].bquark());
	float DeltaRnub = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)leptonicTops[0].neutrino(),(TLorentzVector)leptonicTops[0].bquark()); 
	
	(ToplnuHisto.DeltaR)->Fill(DeltaRlnu);
	(ToplbHisto.DeltaR)->Fill(DeltaRlb);
	(TopnubHisto.DeltaR)->Fill(DeltaRnub);  
   }
   
  
   
   float Ht = 0;
   for(unsigned int i=0;i<NPgenEvt.leptons().size();i++){
   	Ht = Ht + NPgenEvt.leptons()[i].Pt();
   }
   (LeptonsHisto.Ht)->Fill(Ht);
   
   
    /*vector<TRootGenTop> hadronicTops;
   for(unsigned int i=0;i<NPgenEvt.tops().size();i++) if(NPgenEvt.tops()[i].isHadronic()) hadronicTops.push_back(NPgenEvt.tops()[i]);
   std::sort(hadronicTops.begin(),hadronicTops.end(),HighestPtGenTop());
   if(hadronicTops.size()>0){
      float TopGamma = hadronicTops[0].Gamma();
      float TopPt = hadronicTops[0].Pt();
      float TopEta = hadronicTops[0].Eta();
      
      float WGamma = hadronicTops[0].W().Gamma();
      float WPt = hadronicTops[0].W().Pt();
      float WEta = hadronicTops[0].W().Eta();

      //float DeltaRqq = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)hadronicTops[0].quark(),(TLorentzVector)hadronicTops[0].quarkBar()); 
      float DeltaRqq = ROOT::Math::VectorUtil::DeltaR(hadronicTops[0].quark(),hadronicTops[0].quarkBar()); 
      float DeltaRqb_1 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)hadronicTops[0].quark(),(TLorentzVector)hadronicTops[0].bquark()); 
      float DeltaRqb_2 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)hadronicTops[0].quarkBar(),(TLorentzVector)hadronicTops[0].bquark());
   
      float minDeltaR = -999.;
      float maxDeltaR = -999.;
      if(DeltaRqb_1<DeltaRqb_2){
      	minDeltaR = DeltaRqb_1;
      	maxDeltaR = DeltaRqb_2;
      }
      else{
      	minDeltaR = DeltaRqb_2;
      	maxDeltaR = DeltaRqb_1;
      }
      if(DeltaRqq<minDeltaR) minDeltaR = DeltaRqq;
      if(DeltaRqq>maxDeltaR) maxDeltaR = DeltaRqq;

      h_MinDeltaRqqb->Fill(minDeltaR);
      if(fabs(hadronicTops[0].bquark().Eta())<2. && fabs(hadronicTops[0].quark().Eta())<2. && fabs(hadronicTops[0].quarkBar().Eta())<2.) h_MinDeltaRqqb_cut->Fill(minDeltaR);
      h_MinDeltaRqqb_vs_PtTop->Fill(minDeltaR,TopPt);
      h_MinDeltaRqqb_vs_EtaTop->Fill(minDeltaR,TopEta);
      h_MinDeltaRqqb_vs_GammaTop->Fill(minDeltaR,TopGamma);
      
      h_MaxDeltaRqqb->Fill(maxDeltaR);
      if(fabs(hadronicTops[0].bquark().Eta())<2. && fabs(hadronicTops[0].quark().Eta())<2. && fabs(hadronicTops[0].quarkBar().Eta())<2.) h_MaxDeltaRqqb_cut->Fill(maxDeltaR);
      h_MaxDeltaRqqb_vs_PtTop->Fill(maxDeltaR,TopPt);
      h_MaxDeltaRqqb_vs_EtaTop->Fill(maxDeltaR,TopEta);
      h_MaxDeltaRqqb_vs_GammaTop->Fill(maxDeltaR,TopGamma);
      
      h_DeltaRqq->Fill(DeltaRqq);
      if(fabs(hadronicTops[0].quark().Eta())<2. && fabs(hadronicTops[0].quarkBar().Eta())<2.) h_DeltaRqq_cut->Fill(DeltaRqq);
      h_DeltaRqq_vs_PtTop->Fill(DeltaRqq,TopPt);
      h_DeltaRqq_vs_EtaTop->Fill(DeltaRqq,TopEta);
      h_DeltaRqq_vs_GammaTop->Fill(DeltaRqq,TopGamma);
      h_DeltaRqq_vs_PtW->Fill(DeltaRqq,WPt);
      h_DeltaRqq_vs_EtaW->Fill(DeltaRqq,WEta);
      h_DeltaRqq_vs_GammaW->Fill(DeltaRqq,WGamma); 

   }
   */
 }
 Efficiency = (double) nSelectedEvents / nEvents; 
}

void PlotTopology::Fill(TRootGenEvent& genEvt){		// e.g. running over ttbar events (!=NewPhysics)
  nEvents++;
  // no distinction between leptons in the case of non-semileptonic topology?
  // if doSemiLeptonicTopology is set to false on a ttbar sample this Fill function skips the filling of the histograms 
  // (has to be extended/modified to be able to run on fully hadronic or fully leptonic ttbar events)
  if(genEvt.isSemiLeptonic()&&doSemiLeptonicTopology){	
    nSelectedEvents++;
  
    vector<TLorentzVector> electrons;
    vector<TLorentzVector> muons;
    vector<TLorentzVector> tauons;
    vector<TLorentzVector> electronneutrinos;
    vector<TLorentzVector> muonneutrinos;
    vector<TLorentzVector> tauonneutrinos;   
   	// for the ttbar dataset: genEvt.lepton() gives just a TLorentzVector, 
	// no vector<> because there is exactly one lepton in a semileptonic event
	if(genEvt.semiLeptonicChannel()==1){
		electrons.push_back(genEvt.lepton());
		electronneutrinos.push_back(genEvt.neutrino());
	}
	if(genEvt.semiLeptonicChannel()==2){
		muons.push_back(genEvt.lepton());
		muonneutrinos.push_back(genEvt.neutrino());
	}
	if(genEvt.semiLeptonicChannel()==3){
		tauons.push_back(genEvt.lepton());
		tauonneutrinos.push_back(genEvt.neutrino());
	} 
    vector<TLorentzVector> hadronicTops;  // for the ttbar dataset: TLorentzVector, not TRootGenTop 
    vector<TLorentzVector> leptonicTops;
   	hadronicTops.push_back(genEvt.hadronicDecayTop());
	leptonicTops.push_back(genEvt.leptonicDecayTop());    
   
   
    // electrons
    (ElectronHisto.Multiplicity)->Fill(electrons.size());
    for(unsigned int i=0;i<electrons.size();i++){
   	(ElectronHisto.Pt)->Fill(electrons[i].Pt());
	(ElectronHisto.Eta)->Fill(electrons[i].Eta());
	(ElectronHisto.Phi)->Fill(electrons[i].Phi());
	(ElectronHisto.Gamma)->Fill(electrons[i].Gamma());
    }
   
    // muons
    (MuonHisto.Multiplicity)->Fill(muons.size());
    if(muons.size()>0){ 
      TLorentzVector leadingMuon = muons[0];
      for(unsigned int i=0;i<muons.size();i++){
   	(MuonHisto.Pt)->Fill(muons[i].Pt());
	(MuonHisto.Eta)->Fill(muons[i].Eta());
	(MuonHisto.Phi)->Fill(muons[i].Phi());
	if(muons[i].Pt()>leadingMuon.Pt()) leadingMuon = muons[i];  // in semileptonic ttbar sample not needed
      }
     
      float DeltaRleadingmuq_1 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)leadingMuon,(TLorentzVector)genEvt.hadronicDecayQuark());
      float DeltaRleadingmuq_2 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)leadingMuon,(TLorentzVector)genEvt.hadronicDecayQuarkBar());
      float DeltaRleadingmub_1 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)leadingMuon,(TLorentzVector)genEvt.hadronicDecayB());
      float DeltaRleadingmub_2 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)leadingMuon,(TLorentzVector)genEvt.leptonicDecayB());
      float MinDeltaRleadingmuq = 999.;
      float MaxDeltaRleadingmuq = -999.;
      if(DeltaRleadingmuq_1<DeltaRleadingmuq_2){ 
     	MinDeltaRleadingmuq = DeltaRleadingmuq_1;
     	MaxDeltaRleadingmuq = DeltaRleadingmuq_2;
      }
      else {
     	MinDeltaRleadingmuq = DeltaRleadingmuq_2;
	MaxDeltaRleadingmuq = DeltaRleadingmuq_1;
      } 
      if(DeltaRleadingmub_1<MinDeltaRleadingmuq) MinDeltaRleadingmuq = DeltaRleadingmub_1;
      if(DeltaRleadingmub_2<MinDeltaRleadingmuq) MinDeltaRleadingmuq = DeltaRleadingmub_2;
      if(DeltaRleadingmub_1>MaxDeltaRleadingmuq) MaxDeltaRleadingmuq = DeltaRleadingmub_1;
      if(DeltaRleadingmub_2>MaxDeltaRleadingmuq) MaxDeltaRleadingmuq = DeltaRleadingmub_2;
        
      (leadingmuqHisto.MinDeltaR)->Fill(MinDeltaRleadingmuq);
      (leadingmuqHisto.MaxDeltaR)->Fill(MaxDeltaRleadingmuq);
    }
      
    // tauons
    (TauonHisto.Multiplicity)->Fill(tauons.size());
    for(unsigned int i=0;i<tauons.size();i++){
   	(TauonHisto.Pt)->Fill(tauons[i].Pt());
	(TauonHisto.Eta)->Fill(tauons[i].Eta());
	(TauonHisto.Phi)->Fill(tauons[i].Phi());
    }
   
    // electronneutrinos
    (ElectronneutrinoHisto.Multiplicity)->Fill(electronneutrinos.size());
    for(unsigned int i=0;i<electronneutrinos.size();i++){
   	(ElectronneutrinoHisto.Pt)->Fill(electronneutrinos[i].Pt());
	(ElectronneutrinoHisto.Eta)->Fill(electronneutrinos[i].Eta());
	(ElectronneutrinoHisto.Phi)->Fill(electronneutrinos[i].Phi());
    }
   
    // muonneutrinos
    (MuonneutrinoHisto.Multiplicity)->Fill(muonneutrinos.size());
    for(unsigned int i=0;i<muonneutrinos.size();i++){
   	(MuonneutrinoHisto.Pt)->Fill(muonneutrinos[i].Pt());
	(MuonneutrinoHisto.Eta)->Fill(muonneutrinos[i].Eta());
	(MuonneutrinoHisto.Phi)->Fill(muonneutrinos[i].Phi());
    }
   
    // tauonneutrinos
    (TauonneutrinoHisto.Multiplicity)->Fill(tauonneutrinos.size());
    for(unsigned int i=0;i<tauonneutrinos.size();i++){
   	(TauonneutrinoHisto.Pt)->Fill(tauonneutrinos[i].Pt());
	(TauonneutrinoHisto.Eta)->Fill(tauonneutrinos[i].Eta());
	(TauonneutrinoHisto.Phi)->Fill(tauonneutrinos[i].Phi());
    }
   
    // tops
    (TopHisto.Multiplicity)->Fill(2);  //for the ttbar dataset
    for(unsigned int i=0;i<hadronicTops.size();i++){
   	(TopHisto.Pt)->Fill(hadronicTops[i].Pt());
	(TopHisto.Eta)->Fill(hadronicTops[i].Eta());
	(TopHisto.Phi)->Fill(hadronicTops[i].Phi());
    }
    for(unsigned int i=0;i<leptonicTops.size();i++){
   	(TopHisto.Pt)->Fill(leptonicTops[i].Pt());
	(TopHisto.Eta)->Fill(leptonicTops[i].Eta());
	(TopHisto.Phi)->Fill(leptonicTops[i].Phi());
    }
    if(hadronicTops.size()>0){
      	float TopPt = hadronicTops[0].Pt();
      	float TopEta = hadronicTops[0].Eta();
	float TopGamma = hadronicTops[0].Gamma();

      	float WPt = genEvt.hadronicDecayW().Pt();
      	float WEta = genEvt.hadronicDecayW().Eta();
	float WGamma = genEvt.hadronicDecayW().Gamma();
	
   	float DeltaRqq = ROOT::Math::VectorUtil::DeltaR(genEvt.hadronicDecayQuark(),genEvt.hadronicDecayQuarkBar()); 
      	float DeltaRqb_1 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)genEvt.hadronicDecayQuark(),(TLorentzVector)genEvt.hadronicDecayB()); 
      	float DeltaRqb_2 = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)genEvt.hadronicDecayQuarkBar(),(TLorentzVector)genEvt.hadronicDecayB());
	
	//float InvMassqq = ROOT::Math::VectorUtil::InvariantMass((TLorentzVector)genEvt.hadronicDecayQuark(),(TLorentzVector)genEvt.hadronicDecayQuarkBar());
	float InvMassqqb = (genEvt.hadronicDecayQuark() + genEvt.hadronicDecayQuarkBar() + genEvt.hadronicDecayB()).M(); 
	
	(TopqqHisto.DeltaR)->Fill(DeltaRqq);
	(Topqb_1Histo.DeltaR)->Fill(DeltaRqb_1);
	(Topqb_2Histo.DeltaR)->Fill(DeltaRqb_2);
	
	(Topqq_vs_TopHisto.DeltaR_vs_Pt)->Fill(DeltaRqq,TopPt);
	(Topqq_vs_TopHisto.DeltaR_vs_Eta)->Fill(DeltaRqq,TopEta);
	(Topqq_vs_TopHisto.DeltaR_vs_Gamma)->Fill(DeltaRqq,TopGamma);	
	(Topqq_vs_WHisto.DeltaR_vs_Pt)->Fill(DeltaRqq,WPt);
	(Topqq_vs_WHisto.DeltaR_vs_Eta)->Fill(DeltaRqq,WEta);
	(Topqq_vs_WHisto.DeltaR_vs_Gamma)->Fill(DeltaRqq,WGamma);
	
	(TopqqbHisto.InvMass)->Fill(InvMassqqb);
    }
    if(leptonicTops.size()>0){
   	float DeltaRlnu = ROOT::Math::VectorUtil::DeltaR(genEvt.lepton(),genEvt.neutrino()); 
      	float DeltaRlb = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)genEvt.lepton(),(TLorentzVector)genEvt.leptonicDecayB());
	float DeltaRnub = ROOT::Math::VectorUtil::DeltaR((TLorentzVector)genEvt.neutrino(),(TLorentzVector)genEvt.leptonicDecayB()); 
	
	(ToplnuHisto.DeltaR)->Fill(DeltaRlnu);
	(ToplbHisto.DeltaR)->Fill(DeltaRlb);
	(TopnubHisto.DeltaR)->Fill(DeltaRnub);  
    }
   
    float Ht = 0; //for the semileptonic ttbar dataset, only one lepton
    Ht = Ht + genEvt.lepton().Pt();
    (LeptonsHisto.Ht)->Fill(Ht);
   
  }
  else {;}
  Efficiency = (double) nSelectedEvents / nEvents;  
}

void PlotTopology::Compute(){
}
  
void PlotTopology::Write(TFile* fout){
 Compute(); 
/*
 h_MinDeltaRqqb->Write();
 h_MinDeltaRqqb_cut->Write();
 h_MinDeltaRqqb_vs_PtTop->Write();
 h_MinDeltaRqqb_vs_EtaTop->Write();
 h_MinDeltaRqqb_vs_GammaTop->Write();
 
 h_MaxDeltaRqqb->Write();
 h_MaxDeltaRqqb_cut->Write();
 h_MaxDeltaRqqb_vs_PtTop->Write();
 h_MaxDeltaRqqb_vs_EtaTop->Write();
 h_MaxDeltaRqqb_vs_GammaTop->Write();

 h_DeltaRqq->Write();
 h_DeltaRqq_cut->Write();
 h_DeltaRqq_vs_PtTop->Write();
 h_DeltaRqq_vs_EtaTop->Write();
 h_DeltaRqq_vs_GammaTop->Write();
 h_DeltaRqq_vs_PtW->Write();
 h_DeltaRqq_vs_EtaW->Write();
 h_DeltaRqq_vs_GammaW->Write();
 */
 
 (ElectronHisto.Multiplicity)->Write();
 (ElectronHisto.Pt)->Write();
 (ElectronHisto.Eta)->Write();
 (ElectronHisto.Phi)->Write();
 (ElectronHisto.Gamma)->Write();
 
 (MuonHisto.Multiplicity)->Write();
 (MuonHisto.Pt)->Write();
 (MuonHisto.Eta)->Write();
 (MuonHisto.Phi)->Write();
 
 (TauonHisto.Multiplicity)->Write();
 (TauonHisto.Pt)->Write();
 (TauonHisto.Eta)->Write();
 (TauonHisto.Phi)->Write();
 
 (ElectronneutrinoHisto.Multiplicity)->Write();
 (ElectronneutrinoHisto.Pt)->Write();
 (ElectronneutrinoHisto.Eta)->Write();
 (ElectronneutrinoHisto.Phi)->Write();
 
 (MuonneutrinoHisto.Multiplicity)->Write();
 (MuonneutrinoHisto.Pt)->Write();
 (MuonneutrinoHisto.Eta)->Write();
 (MuonneutrinoHisto.Phi)->Write();
 
 (TauonneutrinoHisto.Multiplicity)->Write();
 (TauonneutrinoHisto.Pt)->Write();
 (TauonneutrinoHisto.Eta)->Write();
 (TauonneutrinoHisto.Phi)->Write();
 
//only meaningful for the SUSY samples, histograms empty in Ttbar samples
 	(GluinoHisto.Multiplicity)->Write();
 	(GluinoHisto.Pt)->Write();
 	(GluinoHisto.Eta)->Write();
 	(GluinoHisto.Phi)->Write();
 
 	(StopHisto.Multiplicity)->Write();
 	(StopHisto.Pt)->Write();
 	(StopHisto.Eta)->Write();
 	(StopHisto.Phi)->Write();
 
 (TopHisto.Multiplicity)->Write();
 (TopHisto.Pt)->Write();
 (TopHisto.Eta)->Write();
 (TopHisto.Phi)->Write();
 
 (TopqqHisto.DeltaR)->Write();
 (Topqb_1Histo.DeltaR)->Write();
 (Topqb_2Histo.DeltaR)->Write();
 (ToplnuHisto.DeltaR)->Write();
 (ToplbHisto.DeltaR)->Write();
 (TopnubHisto.DeltaR)->Write();
 
 (Topqq_vs_TopHisto.DeltaR_vs_Pt)->Write();
 (Topqq_vs_TopHisto.DeltaR_vs_Eta)->Write();
 (Topqq_vs_TopHisto.DeltaR_vs_Gamma)->Write();	
 (Topqq_vs_WHisto.DeltaR_vs_Pt)->Write();
 (Topqq_vs_WHisto.DeltaR_vs_Eta)->Write();
 (Topqq_vs_WHisto.DeltaR_vs_Gamma)->Write();
 
 (TopqqbHisto.InvMass)->Write();
 
 (leadingmuqHisto.MinDeltaR)->Write();
 (leadingmuqHisto.MaxDeltaR)->Write();
 
 (LeptonsHisto.Ht)->Write();
 
}
