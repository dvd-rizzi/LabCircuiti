#include "F_Voltage.hpp"

void voltage_function(parameters p) {
  double v0{5};
  TCanvas *c1 = new TCanvas("c1", "voltage_function", 800, 600);
  c1->SetFillColor(0);
  c1->SetGrid();
  TF1 *v_function = new TF1(
      "v_function",
      "5*([0]*sqrt((1-(pow(2*pi*x,2))*[1]*[2])^2 + "
      "(2*pi*x*[2]*[3])^2))/sqrt((([4]+[0])*(1-(pow(2*pi*x,2))*[1]*[2]) + "
      "[3])^2 + (2*pi*x*([1]+[2]*[3]*([4]+[0])))^2)",
      1000, 15000);
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

void response_function(parameters p) {
  TCanvas *c2 = new TCanvas("c2", "response_function", 800, 600);
  c2->SetFillColor(0);
  c2->SetGrid();
  TF1 *v_function = new TF1(
      "R_function",
      "([0]*sqrt((1-(pow(2*pi*x,2))*[1]*[2])^2 + "
      "(2*pi*x*[2]*[3])^2))/sqrt((([4]+[0])*(1-(pow(2*pi*x,2))*[1]*[2]) + "
      "[3])^2 + (2*pi*x*([1]+[2]*[3]*([4]+[0])))^2)",
      1000, 15000);
  v_function->SetParameter(0, p.R);
  v_function->SetParameter(1, p.L);
  v_function->SetParameter(2, p.C);
  v_function->SetParameter(3, p.R_L);
  v_function->SetParameter(4, p.R_v);
  v_function->SetNpx(10000);
  v_function->SetTitle("Funzione di Risposta");
  v_function->Draw();
  v_function->GetXaxis()->SetTitle("Frequenza (Hz)");
  v_function->GetYaxis()->SetTitle("mod(H)");
  c2->SaveAs("response_function.pdf");
}

void fit(parameters p) {
  TGraphErrors *dataset = new TGraphErrors(p.name, "%lg %lg %lg");
  TF1 *v_function = (TF1 *)gROOT->GetFunction("voltage_function");
  v_function->SetParameter(0, p.R);
  v_function->SetParameter(1, p.L);
  v_function->SetParameter(2, p.C);
  v_function->SetParameter(3, p.R_L);
  v_function->SetParameter(4, p.R_v);

  TCanvas *c3 = new TCanvas("c3", "v_function_fit", 800, 600);
  dataset->Fit("voltage_function", "R");
  dataset->Draw("APE");
  dataset->SetLineColor(4);
  dataset->SetMarkerColor(4);
  v_function->Draw("same");
  dataset->SetTitle("Fit figura di diffrazione");
  dataset->GetXaxis()->SetTitle("Posizione (m)");
  dataset->GetYaxis()->SetTitle("Voltaggio (V)");
  dataset->GetXaxis()->CenterTitle(true);
  dataset->GetXaxis()->CenterTitle(true);

  c3->SaveAs("v_function_fit.pdf");
}
