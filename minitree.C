#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "objectClasses.h"
#include "TH1F.h"
#include "TH2F.h"

float mz = 91.1876;

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
  vector<ttH::Jet> *preselected_jets = 0;
  vector<ttH::MET> *met = 0;
  
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
  TBranch *b_preselected_jets = 0;
  TBranch *b_met = 0;
  
 
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
  tree->SetBranchAddress("preselected_jets", &preselected_jets, &b_preselected_jets);
  tree->SetBranchAddress("met", &met, &b_met);
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
  double event_met;
      
  std::vector<double> *ptLepton;
  std::vector<double> *pxLepton;
  std::vector<double> *pyLepton;
  std::vector<double> *pzLepton;
  std::vector<double> *eLepton;
  std::vector<double> *qLepton;
      
  // Output Tree
  TTree* myTree = new TTree("myTree", "   ");
      
  myTree->Branch("eWeight", &eWeight, "eWeight/D");
  myTree->Branch("event_met", &event_met, "event_met/D");
      
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
    b_preselected_jets->GetEntry(tentry);
    b_met->GetEntry(tentry);
    
    weight = wgt;
    histo->Fill(0., weight);
    
    if (nsel == 0 && !higgs_decay) continue;
    histo->Fill(1., weight);
    
    // two leptons (tight, pt, eta id, all in here)
    if (tightMvaBased_leptons->size() < 2) continue;
    histo->Fill(2., weight);
   

    // Invariant mass cut
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
    
    // Jets, b-tag missing
    if (preselected_jets->size() < 2) continue;
    histo->Fill(4., weight);
    
    //same-sign selection
    if (tightMvaBased_leptons->size() !=2) continue;
    
    ttH::Lepton lep_0 = tightMvaBased_leptons->at(0);
    ttH::Lepton lep_1 = tightMvaBased_leptons->at(1);
    if (lep_0.charge != lep_1.charge) continue;
    if (preselected_jets->size() < 4) continue;
   
    
    histo->Fill(5., weight);
    
    
    //Same flavor kill DY
    bool inv_mass_dy = true;
    for (unsigned int i = 0; i < tightMvaBased_muons->size(); i++){
      ttH::Lepton temp_lep = tightMvaBased_muons->at(i);
      TLorentzVector vlep1(temp_lep.tlv().Px(), temp_lep.tlv().Py(), temp_lep.tlv().Pz(), temp_lep.tlv().E());
      if (i < tightMvaBased_muons->size()-2){
	for (unsigned int j = i+1; j < tightMvaBased_muons->size(); j++){
	  ttH::Lepton temp_lep2 = tightMvaBased_muons->at(j);
	  TLorentzVector vlep2(temp_lep2.tlv().Px(), temp_lep2.tlv().Py(), temp_lep2.tlv().Pz(), temp_lep2.tlv().E());
	  TLorentzVector pair = vlep1+vlep2;
	  if (abs(pair.M() - mz) < 10) inv_mass_dy = false;
	}
      }
    }
    for (unsigned int i = 0; i < tightMvaBased_electrons->size(); i++){
      ttH::Lepton temp_lep = tightMvaBased_electrons->at(i);
      TLorentzVector vlep1(temp_lep.tlv().Px(), temp_lep.tlv().Py(), temp_lep.tlv().Pz(), temp_lep.tlv().E());
      if (i < tightMvaBased_electrons->size()-2){
	for (unsigned int j = i+1; j < tightMvaBased_electrons->size(); j++){
	  ttH::Lepton temp_lep2 = tightMvaBased_electrons->at(j);
	  TLorentzVector vlep2(temp_lep2.tlv().Px(), temp_lep2.tlv().Py(), temp_lep2.tlv().Pz(), temp_lep2.tlv().E());
	  TLorentzVector pair = vlep1+vlep2;
	  if (abs(pair.M() - mz) < 10) inv_mass_dy = false;
	}
      }
    }
    if (!inv_mass_dy) continue;
    histo->Fill(6., weight);
    
   // if (lep_0.tlv().Pt() < 20 || lep_1.tlv().Pt() < 20) continue;
   // histo->Fill(7., weight);
    
    ttH::MET e_met = met->at(0);
   // if (e_met.tlv().Pt() < 60) continue;
   // histo->Fill(8., weight);
    //Filling the Tree (at pre-selection level, leptons and mll)
		     
			
    eWeight = weight;
    event_met = e_met.tlv().Pt();
		 
    ptLepton = new std::vector<double>; 
    pxLepton = new std::vector<double>; 
    pyLepton = new std::vector<double>; 
    pzLepton = new std::vector<double>; 
    eLepton = new std::vector<double>; 
    qLepton = new std::vector<double>;
			
    ptLepton->push_back(lep_0.tlv().Pt());
    ptLepton->push_back(lep_1.tlv().Pt());
			
    pxLepton->push_back(lep_0.tlv().Px());
    pxLepton->push_back(lep_1.tlv().Px());
			
    pyLepton->push_back(lep_0.tlv().Py());
    pyLepton->push_back(lep_1.tlv().Py());
			
    pzLepton->push_back(lep_0.tlv().Pz());
    pzLepton->push_back(lep_1.tlv().Pz());
			
    eLepton->push_back(lep_0.tlv().E());
    eLepton->push_back(lep_1.tlv().E());
			
    qLepton->push_back(lep_0.charge);
    qLepton->push_back(lep_1.charge);
    
   
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
      if (i == 5) cout << " 2 or more jets: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 6) cout << " SS selection: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 7) cout << " SF DY out: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      //if (i == 8) cout << " 20, 20: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      //if (i == 9) cout << " MET > 60: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
    }
    cout << "---------------------------------------------------" << endl;
  }
  
  // Save rootfile
  f_var.Write();
  f_var.Close();
  

}
