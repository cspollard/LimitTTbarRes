#ifndef __LLR_h__
#define __LLR_h__

class TH1;

namespace LLR {
    // calculate the log likelihood of the data given the prediction in
    // the iBinth bin.
    double BinLogLikelihood(TH1 *data, TH1 *pred, int iBin);

    // calculate the log likelihood of the data histogram given the
    // prediction histogram
    double LogLikelihood(TH1 *data, TH1 *pred);

    // calculate the log likelihood ratio of the data histogram given the
    // background prediction and the data given the signal+background
    // prediction.
    double LLRatio(TH1 *d, TH1 *b, TH1 *sb);

    // calculate the nsigma deviation of the data histogram given the
    // background prediction and the signal+background prediction assuming
    // gaussian fluctuations.
    double NSigma(TH1 *d, TH1 *b, TH1 *sb);
} // LLR

#endif
