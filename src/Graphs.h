#ifndef __GRAPH__
#define __GRAPH__

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TF1.h"

#include "TApplication.h"
#include "TRootCanvas.h"

#include "TAxis.h"
#include "TRandom3.h"

#include "FileStream.h"

/**
 * @brief Draw simple TGraph, customizing axis
 * 
 */
void Plot() {
cout << "Making plot..." << endl;

    TCanvas *c = new TCanvas("c", "canvas", 1200, 800);

    vector<double> x = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
    vector<double> y = {25, 16, 9, 4, 1, 0, 1, 4, 9, 16, 25};
    int N = x.size();

    TGraph *gr = new TGraph(N, &x[0], &y[0]);
    /* To calculate the values and add them one by one in a loop use function gr->AddPoint(x,y);
    Ex:
    for (int i = 0; i < 10; i++) {
        gr->AddPoint(i, i*2);
    }
    */

    gr->SetTitle("Title");
    gr->GetXaxis()->SetTitle("#theta_{i}");
    gr->GetYaxis()->SetTitle("#sqrt{#delta_{i}}");
    gr->GetXaxis()->CenterTitle();

    gr->SetMarkerStyle(20); // 20
    gr->SetMarkerColor(4);

    gr->Draw("AP");

    c->SaveAs("out/Graph.png");
    c->Close();
}

/**
 * @brief Drawing multiple graphs on the same canvas
 *        Adding legend for each graph
 * 
 */
void MakeMultigraph() {
cout << "Making Multigraph..." << endl;

    TCanvas *c = new TCanvas("c", "canvas", 1200, 800);
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle("Multigraph");
    mg->GetXaxis()->SetTitle("X");
    mg->GetYaxis()->SetTitle("Y");

    vector<double> x = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
    vector<double> y = {25, 16, 9, 4, 1, 0, 1, 4, 9, 16, 25};
    int N = x.size();
    TGraph *gr1 = new TGraph(N, &x[0], &y[0]);
    gr1->SetTitle("Graph1");
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerColor(2);

    x.clear();
    y.clear();

    x = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
    y = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
    N = x.size();
    TGraph *gr2 = new TGraph(N, &x[0], &y[0]);
    gr2->SetTitle("Graph2");
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerColor(4);

    mg->Add(gr1);
    mg->Add(gr2);

    // Create graph legend: (x1, y1, x2, y2) coordinates in percentage
    TLegend *leg = new TLegend(0.9, 0.7, 0.99, 0.9); 
    leg->SetHeader("Legend", "C"); // option "C" allows to center the header
    
    
    // gr1->GetTitle() return the string assigned with gr1->SetTitle("Graph1");
    // This will be the name of gr1 entry in the legend
    // "p" tells the legend to display gr1 as points, there are other options, such as "l" for lines
    leg->AddEntry(gr1, gr1->GetTitle(), "p"); // Add gr1 to the legend
    leg->AddEntry(gr2, gr2->GetTitle(), "p");

    mg->Draw("AP");
    leg->Draw();
    c->SaveAs("out/Multigraph.png");

    c->Close();
    delete leg;
    delete mg;
    delete gr1;
    delete gr2;
    delete c;
}

/**
 * @brief Draw a function
 * 
 */
void Function() {
    auto c = new TCanvas("c", "canvas", 1200, 800);
    TF1 *fa1 = new TF1("fa1","sin(x)/x",0,10);
    fa1->Draw();
    c->SaveAs("out/Function.png");

    c->Close();
    delete c;
    delete fa1;
}

/**
 * @brief Import data from file and draw a graph with error bars
 * 
 */
void ImportData() {
    auto c = new TCanvas("c", "canvas", 1200, 800);
    // Read
    Data values = ReadDAT("data/data2fit.dat");
    vector<double> x, y, ex, ey;
    x = values.X;
    y = values.Y;
    ex = values.dX;
    ey = values.dY;

    int N = x.size();
    TGraphErrors *gr = new TGraphErrors(N, &x[0], &y[0], &ex[0], &ey[0]);

    gr->SetMarkerStyle(20); // 20
    gr->SetMarkerColor(4);

    gr->Draw("AP");

    c->SaveAs("out/ImportData.png");
    c->Close();
    delete c;
    delete gr;
}

/**
 * @brief Generate, write and read values from file
 *        Create graph with error bars
 *        Fit graph to a function
 *        Output of fit indicates parameters, chi squared and more information
 */
void DataFit() {
    cout << "Writting and reading data to fit..." << endl;
    auto c = new TCanvas("c", "canvas", 1200, 800);
    // Gen and Write
    WriteDAT("data/data2fit.dat");
    // Read
    Data values = ReadDAT("data/data2fit.dat");
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

    c->SaveAs("out/DataFit.png");
    c->Close();
    delete func;
    delete c;
    delete gr;
}

/**
 * @brief Uses TApplication to open interative window on screen
 *        Possible to use FitPanel and other tools to edit the graph on the fly
 *        TApplication must be at the beggining of the code and only 1 can exist
 * 
 */
void ShowPlot() {
    // Application to open interactive window
    TApplication app("app", nullptr, nullptr);

    TCanvas *c = new TCanvas("c", "canvas", 1200, 800);

    vector<double> x = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
    vector<double> y = {25, 16, 9, 4, 1, 0, 1, 4, 9, 16, 25};
    int N = x.size();

    TGraph *gr = new TGraph(N, &x[0], &y[0]);

    gr->SetTitle("ShowPlot");

    gr->SetMarkerStyle(20); // 20
    gr->SetMarkerColor(4);

    gr->Draw("AP");

    c->SaveAs("out/ShowPlot.png");

    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    // Connecting canvas
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    // Run app: kTRUE to return control of the program to the user after closing the app
    app.Run(kTRUE);
}

#endif