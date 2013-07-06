// ***************************************************************
// This file was created using the CreateProject.sh script
// for project BATLimit.
// CreateProject.sh is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://www.mppmu.mpg.de/bat
// ***************************************************************

#include "BAT/BCMTFAnalysisFacility.h"
#include "BAT/BCMTF.h"
#include "BAT/BCAux.h"
#include "BAT/BCLog.h"

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 5) {
        cout << "Usage: " << argv[0] << " infile bkg_hist sig_hist data_hist" << endl;
        return -1;
    }

    // open input file.
    TFile *fin = new TFile(argv[1]);
    if (!fin) {
        cout << "invalid infile." << endl;
        return -1;
    }

    // get histograms from input file.
    TH1D *hBkg = (TH1D *) fin->Get(argv[2]);
    TH1D *hSig = (TH1D *) fin->Get(argv[3]);
    TH1D *hDat = (TH1D *) fin->Get(argv[4]);

    cout << "Number of signal events: " << hSig->Integral() << endl;

    TH1D *hSigBkg = (TH1D *) hBkg->Clone();
    hSigBkg->Add(hSig);
    hSigBkg->Add(hSig);
    hSigBkg->Add(hBkg);
    hSigBkg->Add(hBkg);

    BCAux::SetStyle();

    // create new BATModel object
    BCMTF *m = new BCMTF();
    m->AddChannel("all");

    m->AddProcess("ttbar");
    m->SetTemplate("all", "ttbar", *hBkg);
    m->SetPriorDelta("ttbar", hBkg->Integral());

    m->AddSystematic("ttbar_norm", 0, 10);
    m->SetSystematicVariation("all", "ttbar", "ttbar_norm", 1, 1);
    m->SetPriorGauss("ttbar_norm", 0, 1);

    m->AddProcess("signal", 0, 10);
    m->SetTemplate("all", "signal", *hSig);
    m->SetPriorConstant("signal");

    // m->SetData("all", *hDat);
    m->SetData("all", *hSigBkg);

    BCMTFAnalysisFacility *maf = new BCMTFAnalysisFacility(m);
    maf->SetLogLevel(BCLog::warning);

    maf->PerformSingleChannelAnalyses("analysis/", "");

    vector<double> params = m->GetBestFitParameters();
    for (size_t i = 0; i < params.size(); i++)
        cout << "parameter " << i << ": " << params[i] << endl;

    m->PrintStack("all", params);

    /*
    TFile *fout = new TFile("ensemble_tree.root", "recreate");
    TTree *tens = maf->BuildEnsembles(params, 1000);
    TTree *tsum = maf->PerformEnsembleTest(tens, 1000);

    tens->Write();
    tsum->Write();
    fout->Close();
    */

    return 0;
}
