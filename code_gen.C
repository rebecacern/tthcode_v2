#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "objectClasses.h"
#include "TH1F.h"
#include "TH2F.h"

void code_gen(int nsel=0, bool silent=0){
  
  char plotName[300];
  sprintf(plotName,"test");
  if (nsel == 0) {sprintf(plotName,"ttH125");}
  if (nsel == 1111) {sprintf(plotName,"ttH125new");}
  
  
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

  vector<ttH::GenParticle> *pruned_genParticles= 0;
  vector<ttH::Lepton> *preselected_leptons= 0;
  vector<ttH::Lepton> *tightMvaBased_leptons= 0;
  vector<ttH::Electron> *raw_electrons = 0;
  vector<ttH::Muon> *raw_muons = 0;
  
  TBranch *b_mcwgt;   //!
  TBranch *b_wgt;   //!
  TBranch *b_wallTimePerEvent;   //!
  TBranch *b_eventnum;   //!
  TBranch *b_lumiBlock;   //!
  TBranch *b_runNumber;   //!
  TBranch *b_higgs_decay;   //!
 
  TBranch *b_pruned_genParticles = 0;
  TBranch *b_preselected_leptons = 0;
  TBranch *b_tightMvaBased_leptons = 0;
  TBranch *b_raw_electrons = 0;
  TBranch *b_raw_muons = 0;
 
  tree->SetBranchAddress("mcwgt", &mcwgt, &b_mcwgt);
  tree->SetBranchAddress("wgt", &wgt, &b_wgt);
  tree->SetBranchAddress("wallTimePerEvent", &wallTimePerEvent, &b_wallTimePerEvent);
  tree->SetBranchAddress("eventnum", &eventnum, &b_eventnum);
  tree->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
  tree->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
  tree->SetBranchAddress("higgs_decay", &higgs_decay, &b_higgs_decay);
  tree->SetBranchAddress("pruned_genParticles", &pruned_genParticles, &b_pruned_genParticles);
  tree->SetBranchAddress("preselected_leptons", &preselected_leptons, &b_preselected_leptons);
  tree->SetBranchAddress("tightMvaBased_leptons", &tightMvaBased_leptons, &b_tightMvaBased_leptons);
  tree->SetBranchAddress("raw_electrons", &raw_electrons, &b_raw_electrons);
  tree->SetBranchAddress("raw_muons", &raw_muons, &b_raw_muons);
  ///////////////
  
  cout <<"[Info:] You are running GEN code over " << plotName << endl;
  char newRootFile[300];
  sprintf(newRootFile,"results/gen_histos.root");
  TFile f_var(newRootFile, "UPDATE");
  if(!silent){
    std::cout << "[Info:] results root file named " << newRootFile << std::endl;
  }
  
  // Histos
  char title[300];
  sprintf(title,"cuts_%s", plotName);
  TH1F* histo = new TH1F( title, "Cut Flow", 50, 0, 50 );
  histo->Sumw2();
 
  sprintf(title,"deltaR_qq_%s", plotName);
  TH1F* histo_dr_hwwqq = new TH1F( title, "#Delta R between HWW quarks", 100, 0, 5 );
  histo_dr_hwwqq->Sumw2();
 
  sprintf(title,"deltaR_t_qq_%s", plotName);
  TH1F* histo_dr_tqq = new TH1F( title, "#Delta R between top quarks", 100, 0, 5 );
  histo_dr_tqq->Sumw2();
  
  sprintf(title,"deltaR_qqs_%s", plotName);
  TH1F* histo_dr_hwwqqs = new TH1F( title, "#Delta R between HWW lepton and di-quark system", 100, 0, 5 );
  histo_dr_hwwqqs->Sumw2();
  
  sprintf(title,"deltaR_t_qqs_%s", plotName);
  TH1F* histo_dr_tqqs = new TH1F( title, "#Delta R between top lepton and di-quark system", 100, 0, 5 );
  histo_dr_tqqs->Sumw2();
 
  sprintf(title,"deltaR_lq_%s", plotName);
  TH1F* histo_dr_hwwlq = new TH1F( title, "#Delta R between HWW lepton and closest HWW quark", 100, 0, 5 );
  histo_dr_hwwlq->Sumw2();
 
  sprintf(title,"deltaR_l_q_%s", plotName);
  TH1F* histo_dr_hwwl_q = new TH1F( title, "#Delta R between HWW lepton and closest quark", 100, 0, 5 );
  histo_dr_hwwl_q->Sumw2();
 
  sprintf(title,"deltaR_t_lq_%s", plotName);
  TH1F* histo_dr_t_lq = new TH1F( title, "#Delta R between top lepton and closest top quark", 100, 0, 5 );
  histo_dr_t_lq->Sumw2();
  
  sprintf(title,"deltaR_t_l_q_%s", plotName);
  TH1F* histo_dr_t_l_q = new TH1F( title, "#Delta R between top lepton and closest quark", 100, 0, 5 );
  histo_dr_t_l_q->Sumw2();
 
  sprintf(title,"deltaR_ll_%s", plotName);
  TH1F* histo_dr_ll = new TH1F( title, "#Delta R between leptons", 100, 0, 5 );
  histo_dr_ll->Sumw2();
  
  sprintf(title,"lepton_pt_%s", plotName);
  TH1F* histo_lepton_pt = new TH1F( title, "P_{T} of the lepton", 200, 0, 200);
  histo_lepton_pt->Sumw2();
  
  sprintf(title,"lepton_t_pt_%s", plotName);
  TH1F* histo_lepton_t_pt = new TH1F( title, "P_{T} of the lepton (top)", 200, 0, 200);
  histo_lepton_t_pt->Sumw2();
 
  sprintf(title,"Higgs_pt_%s", plotName);
  TH1F* histo_higgs_pt = new TH1F( title, "P_{T} of the Higgs", 500, 0, 500);
  histo_higgs_pt->Sumw2();
 
  sprintf(title,"deltaR_Hpt_%s", plotName);
  TH2F* histo_dr_higgs_pt = new TH2F( title, "#DeltaR Vs P_{T} of the Higgs", 100, 0, 5, 500, 0, 500);
  histo_dr_higgs_pt->Sumw2();
 
  sprintf(title,"deltaR_lpt_%s", plotName);
  TH2F* histo_dr_lep_pt = new TH2F( title, "#DeltaR Vs P_{T} of the lepton", 100, 0, 5, 500, 0, 500);
  histo_dr_lep_pt->Sumw2();


  double weight = 1;
  
  if (!silent) cout << "[Info:] Original sample:  " << totalevents << " events " << endl;
  if (!silent) cout << "[Info:] Number of events in tuple: " << tree->GetEntries() << "(" << tree->GetEntries()*100/totalevents << "%)" << endl;
  
  // loop over events 
  //for(int iEvent = 0; iEvent < 100000; iEvent++){
  for(int iEvent = 0; iEvent < tree->GetEntries(); iEvent++){
    Long64_t tentry = tree->LoadTree(iEvent);
    
    //Point to the proper entry
    b_higgs_decay->GetEntry(tentry);
    b_pruned_genParticles->GetEntry(tentry);
    b_preselected_leptons->GetEntry(tentry);
    b_tightMvaBased_leptons->GetEntry(tentry);
    b_raw_electrons->GetEntry(tentry);
    b_raw_muons->GetEntry(tentry);
  
    histo->Fill(0., weight);
    
    //pseudo-preselection
    int n_raw_lep = 0;
    for (unsigned int i = 0; i < raw_muons->size(); i++){
      ttH::Muon rmuon= raw_muons->at(i);
      bool good_mu = true;
      if (rmuon.tlv().Pt() < 10 || abs(rmuon.tlv().Eta()) >= 2.4) good_mu = false;
      //else if (rmuon.relIso >= 0.5) good_mu = false;
      else if (abs(rmuon.dxy) >= 0.05 || abs(rmuon.dz) >= 0.1)good_mu = false;
      if (good_mu) n_raw_lep++;
    }
   
    for (unsigned int i = 0; i < raw_electrons->size(); i++){
      ttH::Electron rele= raw_electrons->at(i);
      bool good_ele = true;
      if (rele.tlv().Pt() <= 10 || abs(rele.tlv().Eta()) >= 2.5) good_ele = false;
      // else if (rele.relIso >= 0.5) good_ele = false;
      if (abs(rele.SCeta) < 0.8){ if (rele.mvaID <= 0.35) good_ele = false;}
      else if (abs(rele.SCeta) < 1.479) { if( rele.mvaID <= 0.2) good_ele = false;}
      else { if(rele.mvaID <= -0.52) good_ele = false;}
      if (abs(rele.dxy) > 0.05 || abs(rele.dz) > 0.1 || rele.numMissingInnerHits > 1) good_ele = false;
      
      if (good_ele) n_raw_lep++;
    }
 
    if (n_raw_lep < 2) continue; //preselection no-iso
    //if (preselected_leptons->size() < 2) continue; //preselection official
    histo->Fill(1., weight);
    
    if (!higgs_decay) continue;
    histo->Fill(2., weight);
    
    // Get Ws and Higgs          
    bool HWW = false; 
    int indexH = -1;
    int indexWt = -1;
    int indexWat = -1;
    for (unsigned int i = 0; i < pruned_genParticles->size(); i++){
      ttH::GenParticle genpar = pruned_genParticles->at(i);
      if (genpar.pdgID == 25 && genpar.status == 62){
        indexH = i;
        if (genpar.child0 != 9999 && genpar.child1 != 9999){
          ttH::GenParticle child_1 = pruned_genParticles->at(genpar.child0);
	  ttH::GenParticle child_2 = pruned_genParticles->at(genpar.child1);
	  if (abs(child_1.pdgID) == abs(child_2.pdgID) && abs(child_1.pdgID) == 24) HWW = true;
	}
      } 
    } 
    for (unsigned int i = 0; i < pruned_genParticles->size(); i++){
      ttH::GenParticle genpar = pruned_genParticles->at(i);
      if (genpar.pdgID == 24){
        if (genpar.mother != 9999){
          ttH::GenParticle mum = pruned_genParticles->at(genpar.mother);
	  if (mum.pdgID == 6 && genpar.child0 != 9999 && genpar.child1 != 9999) {
	    indexWt = i;
	  }
	} 
      } else if (genpar.pdgID == -24){
        if (genpar.mother != 9999){
          ttH::GenParticle mum = pruned_genParticles->at(genpar.mother);
	  if (mum.pdgID == -6 && genpar.child0 != 9999 && genpar.child1 != 9999) {
	    indexWat = i;
	  }
	} 
      }
    } 
    
    if (!HWW) continue;   
    histo->Fill(3., weight);
    
    //selecting full legacy completed kids0
    if ((pruned_genParticles->at(indexH)).child0 == 9999) continue;
    if ((pruned_genParticles->at(indexH)).child1 == 9999) continue;
    if (indexWt == -1 || indexWat == -1) continue;
    
    //selecting full legacy completed grandkids
    if ((pruned_genParticles->at((pruned_genParticles->at(indexH)).child0)).child0 == 9999) continue;   
    if ((pruned_genParticles->at((pruned_genParticles->at(indexH)).child0)).child1 == 9999) continue;
    if ((pruned_genParticles->at((pruned_genParticles->at(indexH)).child1)).child0 == 9999) continue;
    if ((pruned_genParticles->at((pruned_genParticles->at(indexH)).child1)).child1 == 9999) continue;
    
    if (pruned_genParticles->at(indexWt).child0 == 9999) continue;   
    if (pruned_genParticles->at(indexWt).child1 == 9999) continue;
    if (pruned_genParticles->at(indexWat).child0 == 9999) continue;
    if (pruned_genParticles->at(indexWat).child1 == 9999) continue;
    histo->Fill(4., weight);
    
    //selecting semileptonic HWW
    int nleptons = 0;
    int kid0 = (pruned_genParticles->at((pruned_genParticles->at(indexH)).child0)).child0;
    int kid1 = (pruned_genParticles->at((pruned_genParticles->at(indexH)).child0)).child1;
    int kid2 = (pruned_genParticles->at((pruned_genParticles->at(indexH)).child1)).child0;
    int kid3 = (pruned_genParticles->at((pruned_genParticles->at(indexH)).child1)).child1;
    if (abs((pruned_genParticles->at(kid0)).pdgID) == 11 || abs((pruned_genParticles->at(kid0)).pdgID) == 13) nleptons++; 
    if (abs((pruned_genParticles->at(kid1)).pdgID) == 11 || abs((pruned_genParticles->at(kid1)).pdgID) == 13) nleptons++; 
    if (abs((pruned_genParticles->at(kid2)).pdgID) == 11 || abs((pruned_genParticles->at(kid2)).pdgID) == 13) nleptons++; 
    if (abs((pruned_genParticles->at(kid3)).pdgID) == 11 || abs((pruned_genParticles->at(kid3)).pdgID) == 13) nleptons++; 
    if (nleptons != 1) continue;
    
    int indexlepton = -1; 
    if (abs((pruned_genParticles->at(kid0)).pdgID) == 11 || abs((pruned_genParticles->at(kid0)).pdgID) == 13){indexlepton=kid0;}
    if (abs((pruned_genParticles->at(kid1)).pdgID) == 11 || abs((pruned_genParticles->at(kid1)).pdgID) == 13){indexlepton=kid1;}
    if (abs((pruned_genParticles->at(kid2)).pdgID) == 11 || abs((pruned_genParticles->at(kid2)).pdgID) == 13){indexlepton=kid2;}
    if (abs((pruned_genParticles->at(kid3)).pdgID) == 11 || abs((pruned_genParticles->at(kid3)).pdgID) == 13){indexlepton=kid3;}
    int indexq[2] = {-1, -1}; 
    if (abs((pruned_genParticles->at(kid0)).pdgID) < 5 && abs((pruned_genParticles->at(kid1)).pdgID) < 5){indexq[0]=kid0; indexq[1]=kid1;}
    if (abs((pruned_genParticles->at(kid2)).pdgID) < 5 && abs((pruned_genParticles->at(kid3)).pdgID) < 5){indexq[0]=kid2; indexq[1]=kid3;}
    
    if (abs((pruned_genParticles->at(kid0)).pdgID) == 15 || abs((pruned_genParticles->at(kid0)).pdgID) == 17) continue; 
    if (abs((pruned_genParticles->at(kid1)).pdgID) == 15 || abs((pruned_genParticles->at(kid1)).pdgID) == 17) continue;
    if (abs((pruned_genParticles->at(kid2)).pdgID) == 15 || abs((pruned_genParticles->at(kid2)).pdgID) == 17) continue;
    if (abs((pruned_genParticles->at(kid3)).pdgID) == 15 || abs((pruned_genParticles->at(kid3)).pdgID) == 17) continue;

    if (indexq[0] == -1 || indexq[1] == -1) continue; 
    histo->Fill(5., weight);
    
    //selecting semileptonic tt
    int ntleptons = 0;
    int tkid0 = pruned_genParticles->at(indexWt).child0;
    int tkid1 = pruned_genParticles->at(indexWt).child1;
    int tkid2 = pruned_genParticles->at(indexWat).child0;
    int tkid3 = pruned_genParticles->at(indexWat).child1;
    if (abs((pruned_genParticles->at(tkid0)).pdgID) == 11 || abs((pruned_genParticles->at(tkid0)).pdgID) == 13) ntleptons++; 
    if (abs((pruned_genParticles->at(tkid1)).pdgID) == 11 || abs((pruned_genParticles->at(tkid1)).pdgID) == 13) ntleptons++; 
    if (abs((pruned_genParticles->at(tkid2)).pdgID) == 11 || abs((pruned_genParticles->at(tkid2)).pdgID) == 13) ntleptons++; 
    if (abs((pruned_genParticles->at(tkid3)).pdgID) == 11 || abs((pruned_genParticles->at(tkid3)).pdgID) == 13) ntleptons++; 
    if (ntleptons != 1) continue;
   
    // Identify leptons and light quarks
    int indextlepton = -1; 
    if (abs((pruned_genParticles->at(tkid0)).pdgID) == 11 || abs((pruned_genParticles->at(tkid0)).pdgID) == 13){indextlepton=tkid0;}
    if (abs((pruned_genParticles->at(tkid1)).pdgID) == 11 || abs((pruned_genParticles->at(tkid1)).pdgID) == 13){indextlepton=tkid1;}
    if (abs((pruned_genParticles->at(tkid2)).pdgID) == 11 || abs((pruned_genParticles->at(tkid2)).pdgID) == 13){indextlepton=tkid2;}
    if (abs((pruned_genParticles->at(tkid3)).pdgID) == 11 || abs((pruned_genParticles->at(tkid3)).pdgID) == 13){indextlepton=tkid3;}
    int indextq[2] = {-1, -1}; 
    if (abs((pruned_genParticles->at(tkid0)).pdgID) < 5 && abs((pruned_genParticles->at(tkid1)).pdgID) < 5){indextq[0]=tkid0; indextq[1]=tkid1;}
    if (abs((pruned_genParticles->at(tkid2)).pdgID) < 5 && abs((pruned_genParticles->at(tkid3)).pdgID) < 5){indextq[0]=tkid2; indextq[1]=tkid3;}
    
    if (abs((pruned_genParticles->at(tkid0)).pdgID) == 15 || abs((pruned_genParticles->at(tkid0)).pdgID) == 17) continue; 
    if (abs((pruned_genParticles->at(tkid1)).pdgID) == 15 || abs((pruned_genParticles->at(tkid1)).pdgID) == 17) continue;
    if (abs((pruned_genParticles->at(tkid2)).pdgID) == 15 || abs((pruned_genParticles->at(tkid2)).pdgID) == 17) continue;
    if (abs((pruned_genParticles->at(tkid3)).pdgID) == 15 || abs((pruned_genParticles->at(tkid3)).pdgID) == 17) continue;

    if (indextq[0] == -1 || indextq[1] == -1) continue; 
    histo->Fill(6., weight);
    
    // Fill up stuff
    ttH::GenParticle lep1 = pruned_genParticles->at(indexlepton);
    ttH::GenParticle lep2 = pruned_genParticles->at(indextlepton);
    ttH::GenParticle qw1 = pruned_genParticles->at(indexq[0]);
    ttH::GenParticle qw2 = pruned_genParticles->at(indexq[1]);
    ttH::GenParticle q1 = pruned_genParticles->at(indextq[0]);
    ttH::GenParticle q2 = pruned_genParticles->at(indextq[1]);

    // Chose same sign
    if (lep1.pdgID*lep2.pdgID < 0) continue;
    histo->Fill(7., weight);
    
    // Saving vectors 
    TVector3 vlep1(lep1.tlv().Px(), lep1.tlv().Py(), lep1.tlv().Pz());
    TVector3 vlep2(lep2.tlv().Px(), lep2.tlv().Py(), lep2.tlv().Pz());
    TVector3 vqw1(qw1.tlv().Px(), qw1.tlv().Py(), qw1.tlv().Pz());
    TVector3 vqw2(qw2.tlv().Px(), qw2.tlv().Py(), qw2.tlv().Pz());
    TVector3 vq1(q1.tlv().Px(), q1.tlv().Py(), q1.tlv().Pz());
    TVector3 vq2(q2.tlv().Px(), q2.tlv().Py(), q2.tlv().Pz());
    TVector3 vqq = vqw1+vqw2;
    TVector3 vtqq = vq1+vq2;
    
    // angles
    float mindr = TMath::Min(vqw1.DeltaR(vlep1),vqw2.DeltaR(vlep1));
    float min_dr =  TMath::Min(vq1.DeltaR(vlep1),vq2.DeltaR(vlep1));
    if (min_dr > mindr) min_dr = mindr;
    
    float mintdr = TMath::Min(vq1.DeltaR(vlep2),vq2.DeltaR(vlep2));
    float min_tdr =  TMath::Min(vqw1.DeltaR(vlep2),vqw2.DeltaR(vlep2));
    if (min_tdr > mintdr) min_tdr = mintdr;
    
    
    // Filling histos
    histo_dr_hwwqq->Fill(vqw1.DeltaR(vqw2), weight);
    histo_dr_tqq->Fill(vq1.DeltaR(vq2), weight);
    histo_dr_t_lq->Fill(mintdr, weight);
    histo_dr_t_l_q->Fill(min_tdr, weight);
    histo_dr_hwwlq->Fill(TMath::Min(vqw1.DeltaR(vlep1),vqw2.DeltaR(vlep1)), weight);
    histo_lepton_pt->Fill(vlep1.Pt(), weight);
    histo_lepton_t_pt->Fill(vlep2.Pt(), weight);
    histo_higgs_pt->Fill((pruned_genParticles->at(indexH)).tlv().Pt(), weight);
    histo_dr_higgs_pt->Fill(mindr,(pruned_genParticles->at(indexH)).tlv().Pt(), weight);
    histo_dr_lep_pt->Fill(mindr,vlep1.Pt(), weight);
    histo_dr_hwwqqs->Fill(vqq.DeltaR(vlep1), weight); 
    histo_dr_tqqs->Fill(vtqq.DeltaR(vlep2), weight); 
    histo_dr_hwwl_q->Fill(min_dr, weight);
    histo_dr_ll->Fill(vlep1.DeltaR(vlep2), weight);
    
    // Kinematics gen level 
    if (vlep1.Pt() < 10 || vlep2.Pt() < 10 || abs(vlep1.Eta())> 2.5 || abs(vlep2.Eta())> 2.5) continue;
    if (vlep1.Pt() < 20 && vlep2.Pt() < 20 ) continue;
    histo->Fill(8., weight); 

    // How many have DR < = 0.3
    if (min_dr <= 0.3)  histo->Fill(9., weight); 
    if (min_tdr <= 0.3)  histo->Fill(10., weight); 
    
  }
  
  
  // Outside main loop
  
  if (!silent){
    cout << "---------------------------------------------------" << endl;
    cout << "[Results:] GEN only " << endl;
    cout << "---------------------------------------------------" << endl;
    for (int i = 1; i < 13; i++){
      if (i == 1) cout << " all: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 2) cout << " 2 or more preselected leptons: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 3) cout << " higgs decay: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 4) cout << " HWW : " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 5) cout << " all children present: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 6) cout << " HWW semileptonic: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 7) cout << " tt semileptonic:" << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 8) cout << " SS dileptons: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 9) cout << " Pt 20,10 - Eta: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 10) cout << "  * DeltaR HWW lepton <= 0.3: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) 
			<< "(" << histo->GetBinContent(i)*100/histo->GetBinContent(i-1) << "%) " << endl;     
      if (i == 11) cout << "  * DeltaR top lepton <= 0.3: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) 
			<< "(" << histo->GetBinContent(i)*100/histo->GetBinContent(i-2) << "%) " << endl;
    }
    cout << "---------------------------------------------------" << endl;
  }
  
  // Save rootfile
  f_var.Write();
  f_var.Close();
  

}
