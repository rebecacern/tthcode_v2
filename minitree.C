#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "objectClasses.h"
#include "TH1F.h"
#include "TH2F.h"

void minitree(int nsel=0, bool silent=0){
  
  char plotName[300];
  sprintf(plotName,"test");
  if (nsel == 0) {sprintf(plotName,"ttH125");}
  
  
  char myRootFile[300];
  sprintf(myRootFile,"../tuples/%s.root", plotName);

  TFile *fin = new TFile(myRootFile);

  TH1D*  h0;
  h0 = (TH1D*) fin->Get("OSTwoLepAna/numInitialWeightedMCevents");
  int totalevents = h0->GetEntries();

  TTree *tree = (TTree*)fin->Get("OSTwoLepAna/summaryTree");

  // Tree branches
  Double_t        mcwgt;
  Double_t	  wgt;
  Double_t	  wallTimePerEvent;
  Int_t 	  eventnum;
  Int_t 	  lumiBlock;
  Int_t 	  runNumber;
  Int_t 	  higgs_decay;

  vector<ttH::Lepton> *tightMvaBased_leptons= 0;
  vector<ttH::Muon> *tightMvaBased_muons= 0;
  vector<ttH::Electron> *tightMvaBased_electrons = 0;
  
  TBranch *b_mcwgt;   //!
  TBranch *b_wgt;   //!
  TBranch *b_wallTimePerEvent;   //!
  TBranch *b_eventnum;   //!
  TBranch *b_lumiBlock;   //!
  TBranch *b_runNumber;   //!
  TBranch *b_higgs_decay;   //!
  
  TBranch *b_tightMvaBased_leptons = 0;
  TBranch *b_tightMvaBased_muons = 0;
  TBranch *b_tightMvaBased_electrons = 0;
  
 
  tree->SetBranchAddress("mcwgt", &mcwgt, &b_mcwgt);
  tree->SetBranchAddress("wgt", &wgt, &b_wgt);
  tree->SetBranchAddress("wallTimePerEvent", &wallTimePerEvent, &b_wallTimePerEvent);
  tree->SetBranchAddress("eventnum", &eventnum, &b_eventnum);
  tree->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
  tree->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
  tree->SetBranchAddress("higgs_decay", &higgs_decay, &b_higgs_decay);
  tree->SetBranchAddress("tightMvaBased_leptons", &tightMvaBased_leptons, &b_tightMvaBased_leptons);
  tree->SetBranchAddress("tightMvaBased_muons", &tightMvaBased_muons, &b_tightMvaBased_muons);
  tree->SetBranchAddress("tightMvaBased_electrons", &tightMvaBased_electrons, &b_tightMvaBased_electrons);
  ///////////////
  
  cout <<"[Info:] You are running minitree code over " << plotName << endl;
  
    
  char newRootFile[300];
  sprintf(newRootFile,"results/mini_%s.root", plotName);
  TFile f_var(newRootFile, "RECREATE");
  
  if(!silent){
    std::cout << "[Info:] Minitree to be created: " << newRootFile << std::endl;
  }
  
   // Branches of the output Tree
      double eWeight; 
      
      std::vector<double> *ptLepton;
      std::vector<double> *pxLepton;
      std::vector<double> *pyLepton;
      std::vector<double> *pzLepton;
      std::vector<double> *eLepton;
      std::vector<double> *qLepton;
      
   // Output Tree
      TTree* myTree = new TTree("myTree", "   ");
      
      myTree->Branch("eWeight", &eWeight, "eWeight/D");
      
      myTree->Branch("ptLepton","std::vector<double>",&ptLepton);
      myTree->Branch("pxLepton","std::vector<double>",&pxLepton);
      myTree->Branch("pyLepton","std::vector<double>",&pyLepton);
      myTree->Branch("pzLepton","std::vector<double>",&pzLepton);
      myTree->Branch("eLepton","std::vector<double>",&eLepton);
      myTree->Branch("qLepton","std::vector<double>",&qLepton);
      
     
  

  
  // Histos
  char title[300];
  sprintf(title,"cuts_%s", plotName);
  TH1F* histo = new TH1F( title, "Cut Flow", 50, 0, 50 );
  histo->Sumw2();
 
 
  double weight = 1;
  
  if (!silent) cout << "[Info:] Original sample:  " << totalevents << " events " << endl;
  if (!silent) cout << "[Info:] Number of events in tuple: " << tree->GetEntries() << "(" << tree->GetEntries()*100/totalevents << "%)" << endl;
  
  // loop over events 
  //for(int iEvent = 0; iEvent < 100000; iEvent++){
  for(int iEvent = 0; iEvent < tree->GetEntries(); iEvent++){
    Long64_t tentry = tree->LoadTree(iEvent);
    
    //Point to the proper entry
  
    b_tightMvaBased_leptons->GetEntry(tentry);
    b_tightMvaBased_muons->GetEntry(tentry);
    b_tightMvaBased_electrons->GetEntry(tentry);
    b_wgt->GetEntry(tentry);
    b_higgs_decay->GetEntry(tentry);
    
    weight = wgt;
    histo->Fill(0., weight);
    
    if (nsel == 0 && !higgs_decay) continue;
    histo->Fill(1., weight);
    
    if (tightMvaBased_leptons->size() < 2) continue;
    histo->Fill(2., weight);
   
    bool inv_mass = true;
    for (unsigned int i = 0; i < tightMvaBased_leptons->size(); i++){
      ttH::Lepton temp_lep = tightMvaBased_leptons->at(i);
      TLorentzVector vlep1(temp_lep.tlv().Px(), temp_lep.tlv().Py(), temp_lep.tlv().Pz(), temp_lep.tlv().E());
      if (i < tightMvaBased_leptons->size()-2){
      for (unsigned int j = i+1; j < tightMvaBased_leptons->size(); j++){
        ttH::Lepton temp_lep2 = tightMvaBased_leptons->at(j);
        TLorentzVector vlep2(temp_lep2.tlv().Px(), temp_lep2.tlv().Py(), temp_lep2.tlv().Pz(), temp_lep2.tlv().E());
        TLorentzVector pair = vlep1+vlep2;
	if (pair.M() < 12) inv_mass = false;
      }
    }
    }
    if (!inv_mass) continue;
    histo->Fill(3., weight);
    
    
    
      //Filling the Tree (at pre-selection level, leptons and mll)
		     
			
		      eWeight = weight;
		 
		      ptLepton = new std::vector<double>; 
		      pxLepton = new std::vector<double>; 
		      pyLepton = new std::vector<double>; 
		      pzLepton = new std::vector<double>; 
		      eLepton = new std::vector<double>; 
		      qLepton = new std::vector<double>;
			
			ptLepton->push_back(10);
			ptLepton->push_back(20);	
		     // ptLepton->push_back((tightMvaBased_leptons->at(0)).Pt());
		     // ptLepton->push_back((tightMvaBased_leptons->at(1)).Pt());
		    	
       myTree->Fill();
			
		      delete ptLepton;
		      delete pxLepton;
		      delete pyLepton;
		      delete pzLepton;
		      delete eLepton;
		      delete qLepton;
   
  }
  
  
  // Outside main loop
  
  if (!silent){
    cout << "---------------------------------------------------" << endl;
    cout << "[Results:] GEN only " << endl;
    cout << "---------------------------------------------------" << endl;
    for (int i = 1; i < 20; i++){
      if (i == 1) cout << " all: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 2 && nsel == 0) cout << " higgs decay: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 3) cout << " 2 or more tight leptons: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 4) cout << " mll > 12: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
        }
    cout << "---------------------------------------------------" << endl;
  }
  
  // Save rootfile
  f_var.Write();
  f_var.Close();
  

}
