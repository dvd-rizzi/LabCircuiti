#include "TGraph.h"
#include "TGraphErrors.h"
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TFitResult.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TRandom.h>
#include <fstream>
#include <iostream>
#include <TStyle.h>
#include <vector>
#include <TMath.h>

struct parameters
{
    double R;
    double L;
    double C;
    double R_L;
    double R_v;
};

void voltage_function(parameters p);

void response_function(parameters p);

