#include "../src/FileStream.h"

/**
 * @brief Script to run with ROOT interpreter
 * $root script.cpp
 * 
 * IMPORTANT! Function name must be the same as the file name,
 * in this case, script.cpp -> void script()
 */
void script() {
    auto c = new TCanvas("c", "canvas", 1200, 800);
    Data values = ReadDAT("../data/data2fit.dat");
    vector<double> x, y, ex, ey;
    x = values.X;
    y = values.Y;
    ex = values.dX;
    ey = values.dY;

    int N = x.size();
    TGraphErrors *gr = new TGraphErrors(N, &x[0], &y[0], &ex[0], &ey[0]);
    // TGraphErrors *gr = new TGraphErrors(N, &(values.X[0]), &(values.Y[0]), &(values.dX[0]), &(values.Y[0]));

    TF1* func = new TF1("func", "[0]*x*x+[1]*x+[2]");
    func->SetParNames("a","b","c");
    func->SetParameters(1, 1, 0);

    gr->Fit("func");

    gr->SetMarkerStyle(20); // 20
    gr->SetMarkerColor(4);
    gr->SetLineColor(4);

    gr->Draw("AP");

    c->SaveAs("../out/Script.png");
}