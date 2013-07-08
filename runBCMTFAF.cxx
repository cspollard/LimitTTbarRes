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

    BCAux::SetStyle();

    // create new BCMTF object
    BCMTF *m = new BCMTF();

    // only one channel
    m->AddChannel("all");

    // force the nominal ttbar normalization (delta function)
    m->AddProcess("ttbar");
    m->SetTemplate("all", "ttbar", *hBkg);
    m->SetPriorDelta("ttbar", hBkg->Integral());

    // let the ttbar normalization float by up to 10 in each direction
    m->AddSystematic("ttbar_norm", -10, 10);

    // let the variation be +/- 100% of the nominal normalization
    m->SetSystematicVariation("all", "ttbar", "ttbar_norm", 1, 1);
    m->SetPriorGauss("ttbar_norm", 0, 1);

    // add signal with flat prior
    m->AddProcess("signal", 0, 10);
    m->SetTemplate("all", "signal", *hSig);
    m->SetPriorConstant("signal");

    // add data
    m->SetData("all", *hDat);

    BCMTFAnalysisFacility *maf = new BCMTFAnalysisFacility(m);
    maf->SetLogLevel(BCLog::warning);

    maf->PerformSingleChannelAnalyses("analysis/", "");

    vector<double> params = m->GetBestFitParameters();
    for (size_t i = 0; i < params.size(); i++)
        cout << "parameter " << i << ": " << params[i] << endl;

    m->PrintStack("all", params);

    return 0;
}
