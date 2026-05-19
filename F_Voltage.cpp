// Wow! Che bel codice!

#include "F_Voltage.hpp"

void voltage_function(parameters p) {
  double v0{5};
  TCanvas *c1 = new TCanvas("c1", "voltage_function", 800, 600);
  c1->SetFillColor(0);
  c1->SetGrid();
  TF1 *v_function = new TF1(
      "v_function",
      "[5]*([0]*sqrt((1-(pow(2*pi*x,2))*[1]*[2])^2 + "
      "(2*pi*x*[2]*[3])^2))/sqrt((([4]+[0])*(1-(pow(2*pi*x,2))*[1]*[2]) + "
      "[3])^2 + (2*pi*x*([1]+[2]*[3]*([4]+[0])))^2)",
      1000, 15000);
  v_function->SetParameter(0, p.R);
  v_function->SetParameter(1, p.L);
  v_function->SetParameter(2, p.C);
  v_function->SetParameter(3, p.R_L);
  v_function->SetParameter(4, p.R_v);
  v_function->SetParameter(5, 2.5);
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

void gaussian_error(const std::string &file) {
  TH1D *hist = new TH1D("hist", "Occorrenze nel voltaggio", 22, 4.919, 4.926);
  double value;
  std::ifstream inputFile(file);

  if (inputFile.is_open()) {
    while (inputFile >> value) {
      hist->Fill(value);
    }
    inputFile.close();
  } else {
    std::cerr << "Errore: Impossibile aprire il file!" << '\n';
    return;
  }
  hist->Fit("gaus");

  TCanvas *c_h = new TCanvas("c_h", "Istogramma voltaggio fissato", 800, 600);
  hist->Draw("");
  c_h->SaveAs("Istogramma.pdf");
}

void v_fit(parameters p) {
  TGraphErrors *dataset = new TGraphErrors(p.name, "%lg %*lg %lg %lg");
  TF1 *v_function = (TF1 *)gROOT->GetFunction("v_function");
  v_function->SetParameter(0, p.R);
  v_function->SetParameter(1, p.L);
  v_function->SetParameter(2, p.C);
  v_function->SetParameter(3, p.R_L);
  v_function->SetParameter(4, p.R_v);

  TCanvas *c3 = new TCanvas("c3", "v_function_fit", 800, 600);
  v_function->SetRange(2000, 13000);
  dataset->Fit("v_function", "R");

  int N = dataset->GetN();
  double sum_2{0.};
  for (int i; i < N; ++i) {
    double x;
    double y;
    dataset->GetPoint(i, x, y);
    double f_value = v_function->Eval(x);
    double residual = y - f_value;
    sum_2 += residual * residual;
  }

  int N_par = v_function->GetNpar();
  double sigma_real = std::sqrt(sum_2 / (N - N_par));

  std::cout << "\n========== ANALISI RESIDUI ==========\n";

  std::cout << "Numero punti = " << N << '\n';

  std::cout << "Numero parametri = " << N_par << '\n';

  std::cout << "Sigma_eff (RMS residui) = " << sigma_real << " V\n";

  std::cout << "Chi2 = " << v_function->GetChisquare() << '\n';

  std::cout << "NDF = " << v_function->GetNDF() << '\n';

  std::cout << "Chi2/NDF = " << v_function->GetChisquare() / v_function->GetNDF() << '\n';

  dataset->Draw("APE");
  dataset->SetLineColor(4);
  dataset->SetMarkerColor(4);
  v_function->Draw("same");
  dataset->SetTitle("Fit Ampiezza del Voltaggio");
  dataset->GetXaxis()->SetTitle("Frequenza (Hz)");
  dataset->GetYaxis()->SetTitle("Voltaggio (V)");
  dataset->GetXaxis()->CenterTitle(true);
  dataset->GetXaxis()->CenterTitle(true);

  c3->SaveAs("v_function_fit.pdf");
}

void r_fit(parameters p) {
  TGraphErrors *dataset = new TGraphErrors(p.name, "%lg %lg %lg");
  TF1 *r_function = (TF1 *)gROOT->GetFunction("r_function");
  r_function->SetParameter(0, p.R);
  r_function->SetParameter(1, p.L);
  r_function->SetParameter(2, p.C);
  r_function->SetParameter(3, p.R_L);
  r_function->SetParameter(4, p.R_v);

  TCanvas *c3 = new TCanvas("c3", "r_function_fit", 800, 600);
  dataset->Fit("r_function", "R");
  dataset->Draw("APE");
  dataset->SetLineColor(4);
  dataset->SetMarkerColor(4);
  r_function->Draw("same");
  dataset->SetTitle("Fit funzione di risposta");
  dataset->GetXaxis()->SetTitle("Modulo H(w)");
  dataset->GetYaxis()->SetTitle("Voltaggio (V)");
  dataset->GetXaxis()->CenterTitle(true);
  dataset->GetXaxis()->CenterTitle(true);

  c3->SaveAs("r_function_fit.pdf");
}

void multifit(parameters p1, parameters p2, parameters p3) {
  TGraphErrors *dataset1 = new TGraphErrors(p1.name, "%lg %*lg %lg %lg");
  TGraphErrors *dataset2 = new TGraphErrors(p2.name, "%lg %*lg %lg %lg");
  TGraphErrors *dataset3 = new TGraphErrors(p3.name, "%lg %*lg %lg %lg");
  
   TF1 *f1 = new TF1(
      "f1",
      "[5]*([0]*sqrt((1-(pow(2*pi*x,2))*[1]*[2])^2 + "
      "(2*pi*x*[2]*[3])^2))/sqrt((([4]+[0])*(1-(pow(2*pi*x,2))*[1]*[2]) + "
      "[3])^2 + (2*pi*x*([1]+[2]*[3]*([4]+[0])))^2)",
      1500, 14000);
  TF1 *f2 = (TF1 *)f1->Clone("f2");
  TF1 *f3 = (TF1 *)f1->Clone("f3");

  auto set_parameters = [](TF1 *f, parameters p) {
    f->SetParameter(0, p.R);
    f->SetParameter(1, p.L);
    f->SetParameter(2, p.C);
    f->SetParameter(3, p.R_L);
    f->SetParameter(4, p.R_v);
    f->SetParameter(5, 2.5);
    f->SetNpx(10000);
  };

  set_parameters(f1, p1);
  set_parameters(f2, p2);
  set_parameters(f3, p3);
  
  dataset1->Fit(f1, "R");
  dataset2->Fit(f2, "R");
  dataset3->Fit(f3, "R");


  dataset1->SetMarkerColor(kBlue);
  dataset1->SetLineColor(kCyan); 
  f1->SetLineColor(kBlue);      

  dataset2->SetMarkerColor(kRed);
  dataset2->SetLineColor(kOrange+7); 
  f2->SetLineColor(kRed);            

  dataset3->SetMarkerColor(kGreen+2);
  dataset3->SetLineColor(kSpring);  
  f3->SetLineColor(kGreen+2);
  
  TCanvas *c_multi = new TCanvas("c_multi", "MultiFit", 800, 600);


  c_multi->SetGrid();

  dataset1->SetTitle("Confronto filtri notch");

  dataset1->GetXaxis()->SetTitle("Frequenza (Hz)");
  dataset1->GetYaxis()->SetTitle("Voltaggio (V)");

  dataset1->GetYaxis()->SetRangeUser(0.4, 2.5);

  dataset1->Draw("APE");
  dataset2->Draw("PE SAME");
  dataset3->Draw("PE SAME");

  f1->Draw("SAME");
  f2->Draw("SAME");
  f3->Draw("SAME");

  TLegend *leg =
      new TLegend(0.70, 0.15, 0.88, 0.30);

  leg->AddEntry(dataset1, "R = 100 #Omega", "lp");
  leg->AddEntry(dataset2, "R = 330 #Omega", "lp");
  leg->AddEntry(dataset3, "R = 560 #Omega", "lp");

  leg->Draw();

  c_multi->SaveAs("multi_fit.pdf");


}
