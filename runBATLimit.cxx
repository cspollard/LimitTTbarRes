// ***************************************************************
// This file was created using the CreateProject.sh script
// for project BATLimit.
// CreateProject.sh is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://www.mppmu.mpg.de/bat
// ***************************************************************

#include "BAT/BCLog.h"
#include "BAT/BCAux.h"
#include "BAT/BCSummaryTool.h"
#include "BAT/BCH1D.h"
#include "BATModel.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 6)
        cout << "Usage: " << argv[0] << " infile bkg_hist sig_hist data_hist outfile" << endl;

    // set nicer style for drawing than the ROOT default
    BCAux::SetStyle();

    // open log file
    BCLog::OpenLog("log.txt");
    BCLog::SetLogLevel(BCLog::detail);

    // open input file.
    TFile *fin = new TFile(argv[1]);

    // get histograms from input file.
    TH1F *hBkg = (TH1F *) fin->Get(argv[2]);
    TH1F *hSig = (TH1F *) fin->Get(argv[3]);
    TH1F *hDat = (TH1F *) fin->Get(argv[4]);

    // create new BATModel object
    BATModel * m = new BATModel();

    // set the range of the normalization parameter.
    m->GetParameter("norm")->SetLimits(0, 300);
    m->SetPriorConstant("norm");
    m->SetNbins("norm", 1000);

    m->SetBackground(hBkg);
    m->SetSignal(hSig);
    m->SetData(hDat);

    // create output file.
    TFile *fout = new TFile(argv[5], "recreate");

    // set up MCMC
    m->SetNIterationsMax(10000);
    m->MCMCSetNIterationsRun(10000);
    m->MCMCSetNIterationsMax(10000);
    m->MCMCSetNChains(5);

    BCLog::OutSummary("Test model created");

    // create a new summary tool object
    BCSummaryTool * summary = new BCSummaryTool(m);

    // perform your analysis here

    // normalize the posterior, i.e. integrate posterior
    // over the full parameter space
    m->Normalize();

    // run MCMC and marginalize posterior wrt. all parameters
    // and all combinations of two parameters
    m->MarginalizeAll();

    // run mode finding; by default using Minuit
    // m->FindMode();

    // if MCMC was run before (MarginalizeAll()) it is
    // possible to use the mode found by MCMC as
    // starting point of Minuit minimization
    // m->SetOptimizationMethod(BCIntegrate::kOptMetropolis);
    // m->FindMode(m->GetBestFitParameters());

    // draw all marginalized distributions into a PostScript file
    // m->PrintAllMarginalized("BATModel_plots.ps");

    // print all summary plots
    // summary->PrintParameterPlot("BATModel_parameters.eps");
    // summary->PrintCorrelationPlot("BATModel_correlation.eps");
    // summary->PrintKnowledgeUpdatePlots("BATModel_update.ps");

    // calculate p-value
    // m->CalculatePValue(m->GetBestFitParameters());

    // print results of the analysis into a text file
    // m->PrintResults("BATModel_results.txt");

    BCH1D *bchist = m->GetMarginalized("norm");
    cout << "95th quantile: " << bchist->GetQuantile(0.95) << endl;

    bchist->GetHistogram()->Write();

    delete m;
    delete summary;

    fout->Close();
    fin->Close();

    BCLog::OutSummary("Test program ran successfully");
    BCLog::OutSummary("Exiting");

    // close log file
    BCLog::CloseLog();

    return 0;
}
