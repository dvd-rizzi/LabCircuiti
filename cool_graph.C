#include "F_Voltage.hpp"

void cool_graph() {
   TCanvas *C = new TCanvas("C", "Sweep 3D", 1000, 800);
    
    TGraph2D *graph = new TGraph2D();
    TGraph2D *proj_volt = new TGraph2D();
    TGraph2D *proj_fase = new TGraph2D();

    std::ifstream infile("Grafico3D/Sweep1_100");
    if (!infile.is_open()) {
        std::cerr << "Error: file not found!" << std::endl;
        return;
    }

    double freq, volt, fase;
    int i = 0;
    
    double fase_costante = -2.0; 
    double voltaggio_costante = 2.25; 

    while (infile >> freq >> volt >> fase) {

        while (fase > 180.0) {
            fase -= 360.0;
        }
        while (fase < -180.0) {
            fase += 360.0;
        }
        
        graph->SetPoint(i, freq, volt, fase);          
        proj_volt->SetPoint(i, freq, volt, fase_costante); 
        proj_fase->SetPoint(i, freq, voltaggio_costante, fase); 
        i++;
    }
    infile.close();

    double f_min = graph->GetXmin();
    double v_max = graph->GetYmax();
    double p_min = graph->GetZmin();

    for (int k = 0; k < graph->GetN(); ++k) {
        double f, v, p;
        graph->GetPoint(k, f, v, p);
    
        proj_volt->SetPoint(k, f, v, p_min);
    
        proj_fase->SetPoint(k, f, v_max, p);
    }

    graph->SetTitle(";Frequenza (Hz);Voltaggio (V);Fase (#circ)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.6);

    proj_volt->SetMarkerStyle(20);
    proj_volt->SetMarkerSize(0.4);
    proj_volt->SetMarkerColor(kBlue);

    proj_fase->SetMarkerStyle(20);
    proj_fase->SetMarkerSize(0.4);
    proj_fase->SetMarkerColor(kRed);

    graph->GetXaxis()->SetTitleOffset(1.7); 
    graph->GetYaxis()->SetTitleOffset(1.7);

    graph->Draw("P"); 
    proj_volt->Draw("P SAME");
    proj_fase->Draw("P SAME");

    gPad->SetPhi(35);
    gPad->SetTheta(30);

    C->SaveAs("3D_graph.pdf");
}