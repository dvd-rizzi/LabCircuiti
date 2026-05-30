// File: Circuiti.C
// Legge un file con tre colonne:
// Frequenza   V_IN   V_OUT
// e disegna V_IN e V_OUT in funzione della frequenza.

// Compilazione:
// g++ -std=c++17 -O2 plot_circuiti.cpp $(root-config --cflags --libs) -o plot_circuiti
#include "F_Voltage.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TStyle.h"

void Mic(const char* filename = "Dati_Mic/7.6kHz(20k_n_samples)")
{
    std::ifstream fin(filename);

    if (!fin.is_open()) {
        std::cerr << "Errore: non riesco ad aprire "
                  << filename << "\n";
        return;
    }

    std::vector<double> vfreq;
    std::vector<double> vvin;
    std::vector<double> vvr;

    std::string line;

    while (std::getline(fin, line)) {

        if (line.empty()) continue;
        if (line[0] == '#') continue;

        std::istringstream iss(line);

        double freq, vin, vr;

        if (iss >> freq >> vin >> vr) {
            vfreq.push_back(freq);
            vvin.push_back(vin);
            vvr.push_back(vr);
        }
    }

    fin.close();

    if (vfreq.empty()) {
        std::cerr << "Nessun dato letto!\n";
        return;
    }

    gStyle->SetOptStat(0);

    TCanvas* c = new TCanvas("c", "Circuit Graph", 900, 600);

    c->SetGrid();

    // Grafico V_IN
    TGraph* gr_in = new TGraph(
        vfreq.size(),
        &vfreq[0],
        &vvin[0]
    );

    double x_min = 7500;
    double x_max = 7700;

    double y_min = 0.;
    double y_max = 0.1;

    gr_in->GetXaxis()->SetLimits(x_min, x_max);
    gr_in->SetMinimum(y_min);
    gr_in->SetMaximum(y_max);
    
    
    gr_in->SetLineColor(kBlue + 1);
    gr_in->SetMarkerColor(kBlue + 1);
    gr_in->SetLineWidth(2);
    gr_in->SetMarkerStyle(20);
    
    // Grafico V_OUT
    TGraph* gr_out = new TGraph(
        vfreq.size(),
        &vfreq[0],
        &vvr[0]
    );

    gr_out->GetXaxis()->SetLimits(x_min, x_max);
    gr_out->SetMinimum(y_min);
    gr_out->SetMaximum(y_max);
    
    
    
    gr_out->SetLineColor(kRed + 1);
    gr_out->SetMarkerColor(kRed + 1);
    gr_out->SetLineWidth(1);
    gr_out->SetMarkerStyle(21);

    gr_in->Draw("ALP");
    gr_out->Draw("LP SAME");
    


    TLegend* leg = new TLegend(0.7, 0.75, 0.88, 0.88);
    leg->AddEntry(gr_in, "V_{IN}", "lp");
    leg->AddEntry(gr_out, "V_{R}", "lp");
    leg->Draw();

    c->SaveAs("Mic.pdf");
  

}

// int main(int argc, char** argv)
// {
//     std::cout << "Inizio programma...\n";

//     Mic();

//     std::cout << "Fine.\n";

//     return 0;
// }
