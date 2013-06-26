#include "LLR.h"

#include <iostream>

#include <TMath.h>
#include <TH1.h>

namespace LLR {

    double BinLogLikelihood(TH1 *data, TH1 *pred, int iBin) {
        double lambda = pred->GetBinContent(iBin);
        double n = data->GetBinContent(iBin);

        return n*TMath::Log(lambda) - lambda - TMath::LnGamma(n+1);
    }

    double LogLikelihood(TH1 *data, TH1 *pred) {
        int nbins = data->GetNbinsX()*data->GetNbinsY()*data->GetNbinsZ();

        double ll = 0, l;
        for (int iBin = 1; iBin < nbins+1; iBin++) {
            l = BinLogLikelihood(data, pred, iBin);
            ll += l;
        }

        return ll;
    }

    double LLRatio(TH1 *d, TH1 *b, TH1 *sb) {
        return LogLikelihood(d, b) - LogLikelihood(d, sb);
    }

    double NSigma(TH1 *d, TH1 *b, TH1 *sb) {
        double llr = LLRatio(d, b, sb);
        return llr > 0 ? 0 : sqrt(-2*llr);
    }

} // LLR
