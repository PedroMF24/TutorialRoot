#ifndef __HISTS__
#define __HISTS__

#include "TCanvas.h"
#include "TH1D.h"
#include "TH1F.h"
#include "THStack.h"

#include "TRandom3.h"

#include "FileStream.h"

/**
 * @brief Make Histrogram of doubles using a gaussian distribution
 * 
 */
void MakeTH1D() {
    // Create canvas pointer: (name, title, width, height)
    TCanvas *c = new TCanvas("c", "canvas", 1200, 800);
    // Create histogram: (name, title, number of bins, x1, x2);
    TH1D h1("h1","Histogram from a Gaussian",100,-3,3);
    // Fill hist with a random gaussian distribution
    h1.FillRandom("gaus", 10000);
    // Draw hist
    h1.Draw();
    // Save output image or pdf
    c->SaveAs("out/TH1D.png");
}

/**
 * @brief Make Histrogram of floats by calculating and filling each value
 *        Customize the histogram and draw with error bars
 * 
 */
void MakeTH1F() {
    TCanvas *c = new TCanvas("c", "canvas", 1200, 800);
    TH1F *h1 = new TH1F("h1","Random Values", 100, 0, 10);
    // Class for generating random values: 0 is null seed, other ints keep the seed
    TRandom3* rnd = new TRandom3(0);

    for (int i = 0; i < 1000; i++)
    {
        // Generating numbers with a uniform distribution from 0 to 1, than scale by 10
        float x = rnd->Rndm()*10;
        h1->Fill(x);
    }

    // Customize histogram
    h1->SetFillColor(kRed);     // Red: kRed -> 2
    h1->SetMarkerStyle(21);
    h1->SetMarkerColor(kBlue); // Blue: kBlue -> 4

    h1->Draw("E");

    c->SaveAs("out/TH1F.png");
}

/**
 * @brief Stack multiple histograms using TStack, which fits canvas automatically
 *        according to their sizes
 * 
 */
void MakeStack() {
    // Declare Stack pointer
    auto hs = new THStack("hs","Stacked 1D histograms");

    auto h1st = new TH1F("h1st","test hstack",100,-4,4);
    h1st->FillRandom("gaus",20000);
    h1st->SetFillColor(kRed);
    h1st->SetMarkerStyle(21);
    h1st->SetMarkerColor(kRed);
    hs->Add(h1st);

    auto h2st = new TH1F("h2st","test hstack",100,-4,4);
    h2st->FillRandom("gaus",15000);
    h2st->SetFillColor(kBlue);
    h2st->SetMarkerStyle(21);
    h2st->SetMarkerColor(kBlue);
    hs->Add(h2st);

    auto c = new TCanvas("c","stacked hists", 10, 10, 700, 700);
    hs->Draw();
    c->SaveAs("out/THStack.png");
}
#endif