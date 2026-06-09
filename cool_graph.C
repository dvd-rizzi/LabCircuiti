#include "F_Voltage.hpp"

void cool_graph() {
   TCanvas *C = new TCanvas("C", "Sweep 3D", 2000, 1600);
    
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

  int frame_id = 0;
    
    double target1_theta = 0.1;   
    double target1_phi   = -90.0; 

    double target2_theta = 90.0;  
    double target2_phi   = 0.0;   

    double target3_theta = 0.1;   
    double target3_phi   = 0.0;   

    int fase1_frames = 180;
    for (int f = 0; f < fase1_frames; ++f) {
        double t = (double)f / (fase1_frames - 1);
        double current_theta = 30.0 + t * (target1_theta - 30.0);
        double current_phi   = 35.0 + t * (target1_phi - 35.0);
        
        gPad->SetTheta(current_theta);
        gPad->SetPhi(current_phi);
        C->Modified(); C->Update();
        C->SaveAs(Form("frames/frame_%04d.png", frame_id++)); // Usiamo %04d perché supereremo i 999 frame totali
    }

    for (int f = 0; f < 60; ++f) {
        gPad->SetTheta(target1_theta);
        gPad->SetPhi(target1_phi);
        C->Modified(); C->Update();
        C->SaveAs(Form("frames/frame_%04d.png", frame_id++));
    }

    int fase2_frames = 180;
    for (int f = 0; f < fase2_frames; ++f) {
        double t = (double)f / (fase2_frames - 1);
        double current_theta = target1_theta + t * (target2_theta - target1_theta);
        double current_phi   = target1_phi   + t * (target2_phi   - target1_phi);
        
        gPad->SetTheta(current_theta);
        gPad->SetPhi(current_phi);
        C->Modified(); C->Update();
        C->SaveAs(Form("frames/frame_%04d.png", frame_id++));
    }

    for (int f = 0; f < 60; ++f) {
        gPad->SetTheta(target2_theta);
        gPad->SetPhi(target2_phi);
        C->Modified(); C->Update();
        C->SaveAs(Form("frames/frame_%04d.png", frame_id++));
    }

    int fase3_frames = 120;
    for (int f = 0; f < fase3_frames; ++f) {
        double t = (double)f / (fase3_frames - 1);
        double current_theta = target2_theta + t * (target3_theta - target2_theta);
        double current_phi   = target2_phi   + t * (target3_phi   - target2_phi);
        
        gPad->SetTheta(current_theta);
        gPad->SetPhi(current_phi);
        C->Modified(); C->Update();
        C->SaveAs(Form("frames/frame_%04d.png", frame_id++));
    }

    for (int f = 0; f < 60; ++f) {
        gPad->SetTheta(target3_theta);
        gPad->SetPhi(target3_phi);
        C->Modified(); C->Update();
        C->SaveAs(Form("frames/frame_%04d.png", frame_id++));
    }

    C->SaveAs("3D_graph.pdf");
}