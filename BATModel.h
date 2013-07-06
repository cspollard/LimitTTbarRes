// ***************************************************************
// This file was created using the CreateProject.sh script.
// CreateProject.sh is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://www.mppmu.mpg.de/bat
// ***************************************************************

#ifndef __BAT__BATMODEL__H
#define __BAT__BATMODEL__H

#include "BAT/BCModel.h"

class TH1;

// This is a BATModel header file.
// Model source code is located in file BATLimit/BATModel.cxx

// ---------------------------------------------------------
class BATModel : public BCModel {
    private:
        TH1 *hSignal;
        TH1 *hData;
        std::vector<TH1 *> hBackgrounds;


    public:

        // Constructors and destructor
        BATModel();
        BATModel(const char * name);
        ~BATModel();

        // Methods to overload, see file BATModel.cxx
        void DefineParameters();
        double LogLikelihood(const std::vector<double> &parameters);
        // double LogAPrioriProbability(const std::vector<double> &parameters);

        void AddBackground(const TH1 *h);
        void SetSignal(const TH1 *h);
        void SetData(const TH1 *h);

};
// ---------------------------------------------------------

#endif
