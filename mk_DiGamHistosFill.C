// Purpose: Fill gamma+jet analysis histograms
// Author:  mikko.voutilainen@cern.ch
// Created: June 6, 2021
//#include "CondFormats/JetMETObjects/src/Utilities.cc"
//#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
//#include "CondFormats/JetMETObjects/interface/SimpleJetCorrector.h"
//#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

//#include "CondFormats/JetMETObjects/interface/SimpleJetCorrectionUncertainty.h"
//#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "DiGamHistosFill.h"

#include "TSystem.h"

#include <fstream>
#include <string>

#define GPU
//#define LOCAL

#ifdef LOCAL
// Compile these libraries into *.so first with root -l -b -q mk_CondFormats.C
// (works for 6.18.04?)
/*
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/JetCorrectorParameters.cc+)
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/SimpleJetCorrector.cc+)
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/FactorizedJetCorrector.cc+)

R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/SimpleJetCorrectionUncertainty.cc+)
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/JetCorrectionUncertainty.cc+)
*/
//R__LOAD_LIBRARY(GamHistosFill.C+g)
// As in jetphys/mk2_histosFill.C:
//R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/JetCorrectorParameters_cc)
//R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/SimpleJetCorrector_cc)
//R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/FactorizedJetCorrector_cc)

//R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/SimpleJetCorrectionUncertainty_cc)
//R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/JetCorrectionUncertainty_cc)

R__LOAD_LIBRARY(DiGamHistosFill_C)
#else
// (works for 6.26/10)
R__LOAD_LIBRARY(DiGamHistosFill_C.so)
#endif


//void mk_DiGamHistosFill(string dataset = "X", string version = "vX") {
void mk_DiGamHistosFill(string dataset = "X", string version = "w1") { //again:my versioning with wX


  // Settings
  bool addData = (dataset=="2016B"  || dataset=="2016C" || dataset=="2016D" || 
		  dataset=="2016E"  || dataset=="2016F" || 
		  dataset=="2016FG" || dataset=="2016H" || 
		  dataset=="2016BCD"|| dataset=="2016EF"|| dataset=="2016FGH" ||
		  //dataset=="2016BCDEF" ||
		  dataset=="2017B" || dataset=="2017C" || dataset=="2017D" || 
		  dataset=="2017E" || dataset=="2017F" ||
		  //dataset=="2017BCDEF" || 
		  dataset=="2018A"  || dataset=="2018B" ||
		  dataset=="2018C"  || dataset=="2018D" ||
		  dataset=="2018A1" || dataset=="2018A2" ||
		  dataset=="2018D1" || dataset=="2018D2" ||
		  dataset=="2018D3" || dataset=="2018D4" ||
                  //dataset=="2018ABCD");
		  dataset=="2022C"  || dataset=="2022D" || dataset=="2022E" ||
		  dataset=="2022F"  || dataset=="2022G" ||
		  dataset=="2023B" || dataset=="2023Cv123" ||
		  dataset=="2023Cv4" || dataset=="2023D" ||
		  dataset=="2024B" || dataset=="2024C" ||
		  dataset=="2024D" || dataset=="2024E" ||
		  dataset=="2024F" || dataset=="2024G"
		  );
  bool addMC = (dataset=="2016P8" || dataset=="2017P8" || dataset=="2018P8" ||
		dataset=="2016APVP8" ||
		dataset=="2022P8" || //dataset=="2022QCD" ||
		dataset=="2022EEP8" || //dataset=="2022EEQCD" ||
		dataset=="2023P8" || dataset=="2023BPixP8");

  bool addQCD = (dataset=="2016QCD" || dataset=="2016APVQCD" || 
		 dataset=="2017QCD" || dataset=="2018QCD" ||
		 dataset=="2022QCD" || dataset=="2022EEQCD" ||
		 dataset=="2023QCD" || dataset=="2023BPixQCD");

  //cout << "Clean old shared objects and link files" << endl << flush;
  //gSystem->Exec("rm *.d");
  //gSystem->Exec("rm *.so");
  //gSystem->Exec("rm *.pcm");	

  string path = gSystem->pwd();
  cout << "Current path: " << path << endl << flush;
  /*
  gSystem->AddIncludePath(Form("-I%s",path.c_str()));
  gSystem->AddIncludePath(Form("-I%s/CondFormats/JetMETObjects/interface",path.c_str()));
  */
#ifdef GPU
  // Compile these libraries into *.so first with root -l -b -q mk_CondFormats.C
  //gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/Utilities.cc+");
  //gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/JetCorrectorParameters.cc+");
  //gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/SimpleJetCorrector.cc+");
  //gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/FactorizedJetCorrector.cc+");
  
  //gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/SimpleJetCorrectionUncertainty.cc+");
  //gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/JetCorrectionUncertainty.cc+");

  gROOT->ProcessLine(".L DiGamHistosFill.C+g");
#endif


  TChain *c = new TChain("Events");
  //c->AddFile("files/mc-ht-200to400.root");
  //c->AddFile("files/mc-ht-600toInf.root");
  //c->AddFile("files/mc-pt-15to6000.root");
  //c->AddFile("files/data-2018a.root");
  
  // Automatically figure out where we are running the job
  bool runGPU = (path=="/media/storage/gamjet" ||
		 path=="/media/Duo2/manvouti/gamjet");
  bool runLocal = (path=="/Users/voutila/Dropbox/Cern/gamjet" ||
		   path=="/Users/voutila/Library/CloudStorage/Dropbox/Cern/gamjet" ||
		   path=="/Users/manvouti/Dropbox/Cern/gamjet" ||
		   path=="/home/bschilli/Cern/gamjet");
  bool runLxPlus = (path=="/afs/cern.ch/user/v/voutila/scratch0/gamjet" ||
			path =="/afs/cern.ch/user/b/blehtela/private/gamjet-analysis");
  if (!runLocal)  assert(runGPU || runLxPlus);
  if (!runGPU)    assert(runLocal || runLxPlus);
  if (!runLxPlus) assert(runLocal || runGPU);

  if (runGPU) cout << "Running on Hefaistos (runGPU)" << endl;
  if (runLocal) cout << "Running on iMac or MacBook (runLocal)" << endl;
  if (runLxPlus) cout << "Running on lxplus+eos (runLxPlus)" << endl;
  
  if (addData) {
    //ifstream fin(runLocal ? Form("input_files/dataFiles_local_%s.txt",dataset.c_str()) : (runLxPlus ? Form("input_files/dataFiles_eos_Run%s.txt",dataset.c_str()) : Form("input_files/dataFiles_Run%s.txt",dataset.c_str())), ios::in);
    ifstream fin(runLocal ? Form("input_files/dataFiles_local_%s.txt",dataset.c_str()) : (runLxPlus ? Form("input_files/dataFiles_Run%s.txt",dataset.c_str()) : Form("input_files/dataFiles_Run%s.txt",dataset.c_str())), ios::in); //changed back to this format
    string filename;
    cout << "Chaining data files:" << endl << flush;
    int nFiles(0), nFilesMax(4000);//827);//9999);
    while (fin >> filename && nFiles<nFilesMax) {
      ++nFiles;
      c->AddFile(filename.c_str());
    }
    cout << "Chained " << nFiles <<  " files" << endl << flush;
    
    DiGamHistosFill filler(c,0,dataset,version);
    filler.Loop();
  }
  
  if (addMC) {
    ifstream fin(runLocal ? Form("input_files/mcFiles_local_%s.txt",dataset.c_str()) : (runLxPlus ? "" : Form("input_files/mcFiles_%s.txt",dataset.c_str())), ios::in);
    string filename;
    cout << "Chaining MC files:" << endl << flush;
    int nFiles(0), nFilesMax(1437);//100);
    while (fin >> filename && nFiles<nFilesMax) {
      ++nFiles;
      c->AddFile(filename.c_str());
    }
    cout << "Chained " << nFiles <<  " files" << endl << flush;
  
    DiGamHistosFill filler(c,1,dataset,version);
    filler.Loop();
  }

  if (addQCD) {
    ifstream fin(runLocal ? Form("input_files/mcFiles_local_%s.txt",dataset.c_str()) : (runLxPlus ? "" : Form("input_files/mcFiles_%s.txt",dataset.c_str())), ios::in);
    string filename;
    cout << "Chaining QCD MC files:" << endl << flush;
    int nFiles(0), nFilesMax(5097);
    while (fin >> filename && nFiles<nFilesMax) {
      ++nFiles;
      c->AddFile(filename.c_str());
    }
    cout << "Chained " << nFiles <<  " files" << endl << flush;
  
    DiGamHistosFill filler(c,1,dataset,version);
    filler.Loop();
  }
}
