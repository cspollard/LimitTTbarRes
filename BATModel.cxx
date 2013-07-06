// ***************************************************************
// This file was created using the CreateProject.sh script.
// CreateProject.sh is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://www.mppmu.mpg.de/bat
// ***************************************************************

#include "BATModel.h"
#include "BAT/BCMath.h"
#include "TH1.h"
#include "LLR.h"

// ---------------------------------------------------------
BATModel::BATModel() : BCModel() {
    DefineParameters();
    return;
}

// ---------------------------------------------------------
BATModel::BATModel(const char * name) : BCModel(name) {
    DefineParameters();
    return;
}

// ---------------------------------------------------------
BATModel::~BATModel() {
    return;
}

// ---------------------------------------------------------
void BATModel::DefineParameters() {
    AddParameter("sig_norm", 0, 500);
    return;
}

// ---------------------------------------------------------
double BATModel::LogLikelihood(const std::vector<double> &parameters) {
    double norm = parameters.at(0);

    hSignal->Scale(norm/hSignal->Integral());

    TH1F *hSignalBackground = (TH1F *) hSignal->Clone();
    hSignalBackground->Add(hBackground);

    double ll = LLR::LogLikelihood(hData, hSignalBackground);

    // std::cout << "norm: " << norm << ", ll: " << ll << std::endl;

    return ll;
}

void BATModel::AddBackground(const string &name, const TH1 *h, double min, double max) {
    hBackgrounds.push_back((TH1 *) h->Clone);

    return;
}

void BATModel::SetSignal(const TH1 *h) {
    delete hSignal;
    hSignal = (TH1 *) h->Clone();

    return;
}

void BATModel::SetData(const TH1 *h) {
    delete hData;
    hData = (TH1 *) h->Clone();

    return;
}
