#include "F_Voltage.hpp"
void voltage_function(parameters p) {
    double v0{5}; 
    TCanvas *c1 = new TCanvas("c1", "voltage_function", 1280, 800);
    c1->SetFillColor(0);
    c1->SetGrid();
    TF1 *v_function = new TF1("v_function","5*([0]*sqrt((1-(x^2)*[1]*[2])^2+(x*[2]*[3])^2))/sqrt((([4]+[0])*(1-(x^2)*[1]*[2])+[3])^2+(x*([1]+[2]*[3]*([4]+[0])))^2)", 0, 9000);
    v_function->SetParameter(0, p.R);
    v_function->SetParameter(1, p.L);
    v_function->SetParameter(2, p.C);
    v_function->SetParameter(3, p.R_L);
    v_function->SetParameter(4, p.R_v);
    v_function->SetNpx(10000);
    v_function->SetTitle("Funzione Voltaggio");
    v_function->Draw();
    v_function->GetXaxis()->SetTitle("Frequenza (Hz)");
    v_function->GetYaxis()->SetTitle("Voltaggio (V)");
    c1->SaveAs("voltage_function.pdf"); 
}
