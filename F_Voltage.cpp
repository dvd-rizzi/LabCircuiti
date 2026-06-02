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
  TF1 *r_function = new TF1(
      "r_function",
      "([0]*sqrt((1-(pow(2*pi*x,2))*[1]*[2])^2 + "
      "(2*pi*x*[2]*[3])^2))/sqrt((([4]+[0])*(1-(pow(2*pi*x,2))*[1]*[2]) + "
      "[3])^2 + (2*pi*x*([1]+[2]*[3]*([4]+[0])))^2)",
      2000, 13000);
  r_function->SetParameter(0, p.R);
  r_function->SetParameter(1, p.L);
  r_function->SetParameter(2, p.C);
  r_function->SetParameter(3, p.R_L);
  r_function->SetParameter(4, p.R_v);
  r_function->SetNpx(10000);
  r_function->SetTitle("Funzione di Risposta");
  r_function->Draw();
  r_function->GetXaxis()->SetTitle("Frequenza (Hz)");
  r_function->GetYaxis()->SetTitle("mod(H)");
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
  v_function->SetParameter(5, 2.5);
  v_function->SetParLimits(0, p.R - p.R * 0.05, p.R + p.R * 0.05);
  v_function->SetParLimits(4, p.R_v - p.R_v * 0.05, p.R + p.R_v * 0.05);

  v_function->SetParName(0, "R");
  v_function->SetParName(1, "L");
  v_function->SetParName(2, "C");
  v_function->SetParName(3, "R_L");
  v_function->SetParName(4, "R_v");
  v_function->SetParName(5, "V_0");

  TCanvas *c3 = new TCanvas("c3", "v_function_fit", 800, 600);
  v_function->SetRange(2000, 13000);
  dataset->Fit("v_function", "R");

  int N = dataset->GetN();
  double sum_2{0.};
  for (int i = 0; i < N; ++i) {
    double x;
    double y;
    dataset->GetPoint(i, x, y);
    double f_value = v_function->Eval(x);
    double residual = y - f_value;
    sum_2 += residual * residual;
  }

  int N_par = v_function->GetNpar();
  double sigma_real = std::sqrt(sum_2 / (N - N_par));

  std::cout << "\n========== ANALISI RESIDUI Voltage_function ==========\n";

  std::cout << "Numero punti = " << N << '\n';

  std::cout << "Numero parametri = " << N_par << '\n';

  std::cout << "Sigma_eff (RMS residui) = " << sigma_real << " V\n";

  std::cout << "Chi2 = " << v_function->GetChisquare() << '\n';

  std::cout << "NDF = " << v_function->GetNDF() << '\n';

  std::cout << "Chi2/NDF = "
            << v_function->GetChisquare() / v_function->GetNDF() << '\n';

  dataset->Draw("APE");
  dataset->SetLineColor(4);
  dataset->SetMarkerColor(4);
  v_function->Draw("same");
  dataset->SetTitle("Fit Ampiezza del Voltaggio");
  dataset->GetXaxis()->SetTitle("Frequenza (Hz)");
  dataset->GetYaxis()->SetTitle("Voltaggio (V)");
  dataset->GetXaxis()->CenterTitle(true);
  dataset->GetYaxis()->CenterTitle(true);

  c3->SaveAs("v_function_fit.pdf");
}

void r_fit(parameters p) {
  TGraphErrors *dataset = new TGraphErrors(p.name, "%lg %*lg %lg %lg");
  TF1 *r_function = (TF1 *)gROOT->GetFunction("r_function");

  for (int i = 0; i < dataset->GetN(); ++i) {
    double x, y;
    dataset->GetPoint(i, x, y);
    double errY = dataset->GetErrorY(i);

    dataset->SetPoint(i, x, y / 2.5);
    // dataset->SetPointError(i, 0, errY = (0.0015 + (y/2.5)*0.0015)/2.5);
    dataset->SetPointError(i, 0, errY = 0.006);
  }

  r_function->SetParameter(0, p.R);
  r_function->SetParameter(1, p.L);
  r_function->SetParameter(2, p.C);
  r_function->SetParameter(3, p.R_L);
  r_function->SetParameter(4, p.R_v);

  r_function->SetParName(0, "R");
  r_function->SetParName(1, "L");
  r_function->SetParName(2, "C");
  r_function->SetParName(3, "R_L");
  r_function->SetParName(4, "R_v");

  dataset->Fit("r_function", "R");

  int N = dataset->GetN();
  double sum_2{0.};
  for (int i = 0; i < N; ++i) {
    double x;
    double y;
    dataset->GetPoint(i, x, y);
    double f_value = r_function->Eval(x);
    double residual = y - f_value;
    sum_2 += residual * residual;
  }

  int N_par = r_function->GetNpar();
  double sigma_real = std::sqrt(sum_2 / (N - N_par));

  std::cout << "\n========== ANALISI RESIDUI Response_function ==========\n";

  std::cout << "Numero punti = " << N << '\n';

  std::cout << "Numero parametri = " << N_par << '\n';

  std::cout << "Sigma_eff (RMS residui) = " << sigma_real << " V\n";

  std::cout << "Chi2 = " << r_function->GetChisquare() << '\n';

  std::cout << "NDF = " << r_function->GetNDF() << '\n';

  std::cout << "Chi2/NDF = "
            << r_function->GetChisquare() / r_function->GetNDF() << '\n';

  TCanvas *c3 = new TCanvas("c3", "r_function_fit", 800, 600);
  dataset->Draw("APE");
  dataset->SetLineColor(4);
  dataset->SetMarkerColor(4);
  r_function->Draw("same");
  dataset->SetTitle("Fit funzione di risposta");
  dataset->GetXaxis()->SetTitle("Frequenza (Hz)");
  dataset->GetYaxis()->SetTitle("|H(w)|");
  dataset->GetXaxis()->CenterTitle(true);
  dataset->GetXaxis()->CenterTitle(true);

  c3->SaveAs("r_function_fit.pdf");
}

void multifit(parameters p1, parameters p2, parameters p3) {
  TGraphErrors *dataset1 = new TGraphErrors(p1.name, "%lg %*lg %lg %lg");
  TGraphErrors *dataset2 = new TGraphErrors(p2.name, "%lg %*lg %lg %lg");
  TGraphErrors *dataset3 = new TGraphErrors(p3.name, "%lg %*lg %lg %lg");

  std::vector<TGraphErrors *> datasets = {dataset1, dataset2, dataset3};

  std::cout << "--- Analisi delle frequenza di notch ---" << std::endl;

  for (size_t i = 0; i < datasets.size(); ++i) {
    int n_punti = datasets[i]->GetN();
    if (n_punti == 0)
      continue;

    double *x = datasets[i]->GetX();
    double *y = datasets[i]->GetY();

    int indice_min = TMath::LocMin(n_punti, y);

    double freq_notch = x[indice_min];
    double volt_min = y[indice_min];

    std::cout << "Dataset [" << i << "]:" << std::endl;
    std::cout << "  Frequenza di minimo: (" << freq_notch << ") Hz"
              << std::endl;

    std::cout << "--------------------------------------" << std::endl;
  }

  TF1 *f1 = new TF1(
      "f1",
      "[5]*([0]*sqrt((1-(pow(2*pi*x,2))*[1]*[2])^2 + "
      "(2*pi*x*[2]*[3])^2))/sqrt((([4]+[0])*(1-(pow(2*pi*x,2))*[1]*[2]) + "
      "[3])^2 + (2*pi*x*([1]+[2]*[3]*([4]+[0])))^2)",
      2000, 13000);
  TF1 *f2 = (TF1 *)f1->Clone("f2");
  TF1 *f3 = (TF1 *)f1->Clone("f3");

  auto set_parameters = [](TF1 *f, parameters p) {
    f->SetParameter(0, p.R);
    f->SetParameter(1, p.L);
    f->SetParameter(2, p.C);
    f->SetParameter(3, p.R_L);
    f->SetParameter(4, p.R_v);
    f->SetParameter(5, 2.5);
    f->SetParLimits(0, p.R - p.R * 0.05, p.R + p.R * 0.05);
    f->SetParLimits(4, p.R_v - p.R_v * 0.05, p.R + p.R_v * 0.05);
    f->SetNpx(10000);

    f->SetParName(0, "R");
    f->SetParName(1, "L");
    f->SetParName(2, "C");
    f->SetParName(3, "R_L");
    f->SetParName(4, "R_v");
    f->SetParName(5, "V_0");
  };

  set_parameters(f1, p1);
  set_parameters(f2, p2);
  set_parameters(f3, p3);

  dataset1->Fit(f1, "R");
  dataset2->Fit(f2, "R");
  dataset3->Fit(f3, "R");

  dataset1->SetMarkerStyle(20);
  dataset1->SetMarkerSize(0.1);
  dataset1->SetMarkerColorAlpha(kBlue + 2, 0.6);
  dataset1->SetLineColorAlpha(kAzure + 7, 0.6);
  dataset1->SetLineWidth(1);

  dataset2->SetMarkerStyle(20);
  dataset2->SetMarkerSize(0.1);
  dataset2->SetMarkerColorAlpha(kRed + 2, 0.6);
  dataset2->SetLineColorAlpha(kOrange + 1, 0.6);
  dataset2->SetLineWidth(1);

  dataset3->SetMarkerStyle(20);
  dataset3->SetMarkerSize(0.1);
  dataset3->SetMarkerColorAlpha(kGreen + 3, 0.6);
  dataset3->SetLineColorAlpha(kSpring - 3, 0.6);
  dataset3->SetLineWidth(1);

  f1->SetLineColor(kBlue + 2);
  f1->SetLineWidth(2);

  f2->SetLineColor(kRed + 2);
  f2->SetLineWidth(2);

  f3->SetLineColor(kGreen + 3);
  f3->SetLineWidth(2);

  std::cout << "=============MINIMI DELLE FUNZIONI DI FIT=============" << '\n';
  std::cout << "R1: " << f1->GetMinimumX() << '\n';
  std::cout << "R2: " << f2->GetMinimumX() << '\n';
  std::cout << "R3: " << f3->GetMinimumX() << '\n';


  TCanvas *c_multi = new TCanvas("c_multi", "MultiFit", 800, 600);

  c_multi->SetGrid();

  dataset1->SetTitle("");

  dataset1->GetXaxis()->SetTitle("Frequenza (Hz)");
  dataset1->GetYaxis()->SetTitle("Voltaggio (V)");

  dataset1->GetYaxis()->SetRangeUser(0.4, 2.5);

  dataset1->Draw("APE");
  dataset2->Draw("PE SAME");
  dataset3->Draw("PE SAME");

  f1->Draw("SAME");
  f2->Draw("SAME");
  f3->Draw("SAME");

  TLegend *leg = new TLegend(0.70, 0.15, 0.88, 0.30);

  leg->AddEntry(dataset1, "R = 100 #Omega", "lp");
  leg->AddEntry(dataset2, "R = 330 #Omega", "lp");
  leg->AddEntry(dataset3, "R = 560 #Omega", "lp");

  leg->Draw();

  c_multi->SaveAs("multi_fit.pdf");

  std::cout << "========VALORI FINALI CHI2 RIDOTTO========" << '\n';
  std::cout << "100 O: " << f1->GetChisquare() / f1->GetNDF() << '\n';
  std::cout << "330 O: " << f2->GetChisquare() / f2->GetNDF() << '\n';
  std::cout << "560 O: " << f3->GetChisquare() / f3->GetNDF() << '\n';
}

// void time_graph(const std::string &file) {
//   std::vector<double> time_vec;
//   std::vector<double> v_IN_vec;
//   std::vector<double> v_R_vec;
//   double time;
//   double v_IN;
//   double v_R;

//   std::ifstream inputFile(file);

//   if (inputFile.is_open()) {
//     while (inputFile >> time >> v_IN >> v_R) {
//       time_vec.push_back(time);
//       v_IN_vec.push_back(v_IN);
//       v_R_vec.push_back(v_R);
//     }
//     inputFile.close();
//   } else {
//     std::cerr << "Errore: Impossibile aprire il file!" << '\n';
//     return;
//   }

//   TCanvas *timegraph = new TCanvas(
//       "timegraph", "Ampiezza voltaggio in funzione del tempo", 800, 600);

//   TGraph *time_graph_v_IN =
//       new TGraph(time_vec.size(), &time_vec[0], &v_IN_vec[0]);

//   time_graph_v_IN->GetXaxis()->SetLimits(0.0017, 0.0020);

//   TGraph *time_graph_v_R =
//       new TGraph(time_vec.size(), &time_vec[0], &v_R_vec[0]);

//   timegraph->SetGrid();

//   time_graph_v_IN->SetTitle(
//       "Ampiezza del voltaggio ai capi del generatore e della resistenza");
//   time_graph_v_IN->SetLineColor(kBlue + 1);
//   time_graph_v_IN->SetMarkerColor(kBlue + 1);
//   time_graph_v_IN->SetLineWidth(2);
//   time_graph_v_IN->SetMarkerStyle(20);

//   time_graph_v_R->SetLineColor(kRed + 1);
//   time_graph_v_R->SetMarkerColor(kRed + 1);
//   time_graph_v_R->SetLineWidth(2);
//   time_graph_v_R->SetMarkerStyle(20);

//   time_graph_v_IN->Draw("ALP");
//   time_graph_v_R->Draw("LP SAME");

//   TLegend *leg = new TLegend(0.7, 0.75, 0.88, 0.88);
//   leg->AddEntry(time_graph_v_IN, "V_{IN}", "lp");
//   leg->AddEntry(time_graph_v_R, "V_{R}", "lp");
//   leg->Draw();

//   timegraph->SaveAs("TimeGraph.pdf");
// }

double Phase_equation(double *x, double *par) {
  double freq = x[0];
    double Ri   = par[0]; 
    double L    = par[1]; 
    double C    = par[2]; 
    double RL   = par[3]; 
    double Rv   = par[4]; 

    double omega = 2.0 * TMath::Pi() * freq;
    double omega2 = omega * omega;
    double Rsum = Rv + Ri; 

    double resonance = 1.0 - omega2 * L * C;

    double faseNum = TMath::ATan2(omega * RL * C, resonance);

    double realDen = Rsum * resonance + RL;
    double imagDen = omega * (L + Rsum * RL * C);
    double faseDen = TMath::ATan2(imagDen, realDen);

    return (faseNum - faseDen) * (180.0 / TMath::Pi());
}

void fitFase(parameters p) {

  std::vector<double> f;
  std::vector<double> phi;

  std::ifstream in(p.name);
  if (!in.is_open()) {
    std::cerr << "Errore: impossibile aprire il file " << p.name << std::endl;
    return;
  }

  double col1, col2, col3, col4, col5;

  while (in >> col1 >> col2 >> col3 >> col4 >> col5) {
    while (col5 > 180.0) {
      col5 -= 360.0;
    }
    while (col5 < -180.0) {
      col5 += 360.0;
    }
    f.push_back(col1);
    phi.push_back(col5);
  }
  in.close();

  int N = f.size();
  if (N == 0) {
    std::cerr << "Nessun dato letto dal file." << std::endl;
    return;
  }

  TGraph *gr = new TGraph(N, f.data(), phi.data());

  gr->SetTitle("Differenza di fase;Frequenza [Hz];#Delta#phi [deg]");
  gr->SetMarkerStyle(20);

  TF1 *fit = new TF1("fit", Phase_equation, f.front(), f.back(), 5);

  fit->SetParName(0, "R");
  fit->SetParName(1, "L");
  fit->SetParName(2, "C");
  fit->SetParName(3, "R_L");
  fit->SetParName(4, "R_v");

  fit->SetParameter(0, p.R);
  fit->SetParameter(1, p.L);
  fit->SetParameter(2, p.C);
  fit->SetParameter(3, p.R_L);
  fit->SetParameter(4, p.R_v);

  fit->SetParLimits(0, p.R - p.R * 0.05, p.R + p.R * 0.05);
  fit->SetParLimits(4, p.R_v - p.R_v * 0.05, p.R + p.R_v * 0.05);

  TCanvas *c = new TCanvas("c", "Fit Fase", 800, 600);

  gr->Draw("AP");

  gr->Fit(fit, "R S");

  c->Update();

  std::cout << "\n===== RISULTATI =====\n";

  for (int i = 0; i < 5; i++) {
    std::cout << fit->GetParName(i) << " = " << fit->GetParameter(i) << " +/- "
              << fit->GetParError(i) << std::endl;
  }

  std::cout << "Chi2/NDF = " << fit->GetChisquare() << "/" << fit->GetNDF()
            << std::endl;

  c->SaveAs("PhaseFit.pdf");
}

void disegnaModello(parameters p) {
  double fmin = 2000;
  double fmax = 14000;

  TF1 *modello = new TF1("modello", Phase_equation, fmin, fmax, 5);

  modello->SetParameter(0, p.R);
  modello->SetParameter(1, p.L);
  modello->SetParameter(2, p.C);
  modello->SetParameter(3, p.R_L);
  modello->SetParameter(4, p.R_v);

  modello->SetLineColor(kRed);
  modello->SetLineWidth(3);
  modello->SetTitle("Verifica Modello Teorico;Frequenza [Hz];#Delta#phi [deg]");

  TCanvas *c_test = new TCanvas("c_test", "Test Modello", 800, 600);
  c_test->SetGrid();

  modello->Draw();

  c_test->SaveAs("Phase.pdf");

  std::cout << "--- Visualizzazione Modello ---" << std::endl;
  std::cout << "R: " << p.R << " | L: " << p.L << " | C: " << p.C << std::endl;
  std::cout << "R_L: " << p.R_L << " | R_v: " << p.R_v << std::endl;
}