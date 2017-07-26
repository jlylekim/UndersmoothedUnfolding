//==============================================================================
//  Authors: Junhyung Lyle Kim and Mikael Kuusela
//  June 20, 2017
//
//  This code demonstrates how to use the UndersmoothTau function and performs
//  a coverage study comparing intervals from UndersmoothTau and ScanLcurve
//
//------------------------------------------------------------------------------
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//==============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <TMath.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TFitter.h>
#include <TF1.h>
#include <TStyle.h>
#include <TVector.h>
#include <TGraph.h>
#include <TFile.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>
#include "TUnfold.h"
#include "Math/ProbFunc.h"
using namespace std;

// Smear x by additive zero-mean Gaussian noise with standard deviation sigmaSmear
Double_t smear(Double_t x) {
  const Double_t sigmaSmear = 1.0;
  return x + gRandom->Gaus(0,sigmaSmear);
}

// choose data size
Int_t dataSize = 20000;
//Int_t dataSize = 1000;
//Int_t dataSize = 10000;
//Int_t dataSize = 50000;

// Simulate the data to be unfolded
TH1D* generateData() {
  TH1D* hx = new TH1D ("x", "", 40, -7.0, 7.0); // True observations
  TH1D* hy = new TH1D ("y", "", 42, -7.0, 7.0); // Smeared observations
  Int_t Nx = gRandom->Poisson(dataSize);
  for (Int_t i=0; i<Nx; i++) {
    Double_t u = gRandom->Uniform();
    Double_t x;
    if (u <= 0.2) {
      x = gRandom->Uniform(14.0) - 7.0;
    } else if (u <= 0.5) {
      x = gRandom->Gaus(-2.0, 1.0);
    } else {
      x = gRandom->Gaus(2.0,1.0);
    }
    Double_t y = smear(x);
    hx->Fill(x);
    hy->Fill(y);
  }
  return hy;
}

void UndersmoothDemo()
{
  gRandom->SetSeed(123456);

  // Fill hLambda by simulating a large number
  // of observations from the true distribution
  TH1D* hLambda = new TH1D("hLambda", "", 40, -7.0, 7.0);
  for(Int_t i=0;i<1000000;i++) {
    Double_t u = gRandom->Uniform();
    Double_t x;
    if (u <= 0.2) {
      x = gRandom->Uniform(14.0) - 7.0;
    } else if (u <= 0.5) {
      x = gRandom->Gaus(-2.0,1.0);
    } else {
      x = gRandom->Gaus(2.0,1.0);
    }
    hLambda->Fill(x);
    Double_t y=smear(x);
  }
  hLambda->Scale(dataSize/1000000.0);


  // Fill an alternative response matrix with a MC spectrum that differs
  // from the actual truth
  TH2D* KAlt=new TH2D("KAlt","", 42, -7.0, 7.0, 40, -7.0, 7.0);
  for(Int_t i=0;i<1000000;i++) {
    Double_t u = gRandom->Uniform();
    Double_t x;
    if (u <= 0.2) {
      x = gRandom->Uniform(14.0) - 7.0;
    } else if (u <= 0.5) {
      x = gRandom->Gaus(-2.0,1.1);
    } else {
      x = gRandom->Gaus(2.0,0.9);
    }
    Double_t y=smear(x);
    KAlt->Fill(y,x);
  }

  // obtain nominal coverage
  Double_t nominalCoverage = ROOT::Math::normal_cdf(1) - ROOT::Math::normal_cdf(-1);

  Int_t repeatNum = 1000;
  //Double_t *tauListY = new Double_t[repeatNum];   // uncomment to save and plot the taus
  //Double_t *tauListX = new Double_t[repeatNum];   // uncomment to save and plot the taus

  // initiating vectors to store confidence interval lengths for each iteration
  vector<Double_t> intervalLengthListUndersmooth(repeatNum);
  vector<Double_t> intervalLengthListLcurve(repeatNum);

  // for plotting binwise coverage
  // initiating with bin# + 1 since bins start from index 1 in histograms
  // first 0 in binwiseCoverage array will be just 0
  Double_t binwiseCoverageLcurve[41] = {0};
  Double_t binwiseCoverageY[41] = {0};
  Double_t binwiseCoverageX[41];
  // initiating vectors to display average interval lengths
  vector<Double_t> intervalLengthsUndersmooth(40);
  vector<Double_t> intervalLengthsLcurve(40);


  // set up constants needed for ScanLcurve
  Double_t tauMin=0.00005;
  //Double_t tauMax=0.1;   //for size, sizeLambdaMC, curvature cases
  Double_t tauMax=0.2; //for curvatureLambdaMC case

  Int_t nScan=50;
  TSpline *logTauX,*logTauY;
  TGraph *lCurve;


  //Double_t biasScale=0.0;
  Double_t biasScale=dataSize/1000000.0;

  // to plot one realization of unfolded histogram
  TH1D *unfoldedLcurve = new TH1D("Unfolded","", 40, -7.0, 7.0);
  TH1D *unfoldedUndersmooth = new TH1D("Unfolded","", 40, -7.0, 7.0);

  for (Int_t i=0; i<repeatNum; i++) {
    TUnfold* unfoldLcurve = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintNone);
    TUnfold* unfoldUndersmooth = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintNone);

    //TUnfold* unfoldLcurve = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeSize, TUnfold::kEConstraintNone);
    //TUnfold* unfoldUndersmooth = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeSize, TUnfold::kEConstraintNone);

    TH1D *hLambdaHatInvLcurve = new TH1D("Unfolded","", 40, -7.0, 7.0);
    TH1D *hLambdaHatInvUndersmooth = new TH1D("Unfolded","", 40, -7.0, 7.0);

    TH1D* hy = generateData();

    unfoldLcurve->SetInput(hy, biasScale);
    unfoldUndersmooth->SetInput(hy, biasScale);
    //unfoldOracle->SetInput(hy, biasScaleLK);

    // Unfolding using L-curve
    unfoldLcurve->ScanLcurve(nScan,tauMin,tauMax,&lCurve,&logTauX,&logTauY);
    unfoldUndersmooth->ScanLcurve(nScan,tauMin,tauMax,&lCurve,&logTauX,&logTauY);

    Double_t deltaFromLcurve = unfoldUndersmooth->GetTau();
    //tauListLcurve[i] = deltaFromLcurve;


    Double_t new_delta = unfoldUndersmooth->UndersmoothTau(deltaFromLcurve, 0.01, 1000);
    std::cout << "Undersmoothed tau is: " << new_delta << std::endl;

    unfoldUndersmooth->DoUnfold(new_delta);

    // save delta obtained from debiasTau
    //tauListX[i] = i;
    //tauListY[i] = new_delta;

    unfoldLcurve->GetOutput(hLambdaHatInvLcurve);
    unfoldLcurve->GetOutput(unfoldedLcurve);
    unfoldUndersmooth->GetOutput(hLambdaHatInvUndersmooth);
    unfoldUndersmooth->GetOutput(unfoldedUndersmooth);

    // check coverage in each bin
    // should start from 1; GetBinContent(0) and GetBinContent(41) are other information
    for (Int_t j=1; j<41; j++) {
      Double_t hLambdaValue = hLambda->GetBinContent(j);

      Double_t hLambdaHatInvLcurveVar = hLambdaHatInvLcurve->GetBinError(j);
      Double_t hLambdaHatInvLcurveLow = hLambdaHatInvLcurve->GetBinContent(j) - hLambdaHatInvLcurveVar;
      Double_t hLambdaHatInvLcurveUp = hLambdaHatInvLcurve->GetBinContent(j) + hLambdaHatInvLcurveVar;

      Double_t hLambdaHatInvUndersmoothVar = hLambdaHatInvUndersmooth->GetBinError(j);
      Double_t hLambdaHatInvUndersmoothLow = hLambdaHatInvUndersmooth->GetBinContent(j) - hLambdaHatInvUndersmoothVar;
      Double_t hLambdaHatInvUndersmoothUp = hLambdaHatInvUndersmooth->GetBinContent(j) + hLambdaHatInvUndersmoothVar;

      binwiseCoverageX[j] = j;  // for plotting

      intervalLengthsLcurve[j-1] = hLambdaHatInvLcurveVar * 2.0;
      intervalLengthsUndersmooth[j-1] = hLambdaHatInvUndersmoothVar * 2.0;

      // check if lcurve scan covered the true lambda
      if (hLambdaHatInvLcurveLow <= hLambdaValue && hLambdaValue <= hLambdaHatInvLcurveUp) {
        binwiseCoverageLcurve[j] = binwiseCoverageLcurve[j] + (1.0 / repeatNum);
      }

      if (hLambdaHatInvUndersmoothLow <= hLambdaValue && hLambdaValue <= hLambdaHatInvUndersmoothUp) {
        // binwiseCoverageY starts from index 0 but it is initialized to 0 so neglect;
        binwiseCoverageY[j] = binwiseCoverageY[j] + (1.0 / repeatNum);
      }
    }
    // store average interval lengths of the unfolded output in this particular repetition
    intervalLengthListLcurve[i] = TMath::Mean(intervalLengthsLcurve.begin(), intervalLengthsLcurve.end());
    intervalLengthListUndersmooth[i] = TMath::Mean(intervalLengthsUndersmooth.begin(), intervalLengthsUndersmooth.end());

    delete hy;
    delete unfoldUndersmooth;
    delete unfoldLcurve;
    delete hLambdaHatInvUndersmooth;
    delete hLambdaHatInvLcurve;
  }

  // print out obtained binwise coverage from undersmoothing
  for (Int_t k=1; k<41; k++) {
    cout << "Bin #: " << k << " Coverage: " << binwiseCoverageY[k] << endl;
  }

  // obtain average interval lengths
  Double_t averageIntervalLengthLcurve = TMath::Mean(intervalLengthListLcurve.begin(), intervalLengthListLcurve.end());
  Double_t averageIntervalLengthUndersmooth = TMath::Mean(intervalLengthListUndersmooth.begin(), intervalLengthListUndersmooth.end());

  // Set up plotting binwise coverage
  gStyle->SetPadTickY(2);
  gStyle->SetTickLength(0.01, "Y");
  gStyle->SetTickLength(0.0, "X");

  TH1D* undersmoothCoverageHist = new TH1D ("x", "", 40, 0, 41);
  TH1D* lcurveCoverageHist = new TH1D ("x", "", 40, 0, 41);

  for (Int_t i=1;i<41;i++) {
    undersmoothCoverageHist->SetBinContent(i,binwiseCoverageY[i]);
    lcurveCoverageHist->SetBinContent(i,binwiseCoverageLcurve[i]);
  }

  TCanvas* output = new TCanvas("output");
  output->Divide(2,2);

  output->cd(1);
  lcurveCoverageHist->SetLineColor(1);   // black
  lcurveCoverageHist->SetBarWidth(0.9);
  lcurveCoverageHist->SetFillColor(38);
  lcurveCoverageHist->SetTitle("Binwise coverage, ScanLcurve");
  lcurveCoverageHist->GetXaxis()->SetTitle("Bin");
  lcurveCoverageHist->GetYaxis()->SetTitle("Coverage (1000 repetitions)");
  lcurveCoverageHist->GetYaxis()->SetRangeUser(0.0, 0.8);
  lcurveCoverageHist->SetStats(kFALSE);
  lcurveCoverageHist->Draw("bar");
  TLine *nominalCov = new TLine(0,nominalCoverage,41,nominalCoverage);
  nominalCov->SetLineColor(1);  // black
  nominalCov->SetLineStyle(3);  // dotted line; 1: simple, 2: dashed, 3: dotted
  nominalCov->Draw("SAME");
  TLatex *lcurveIntervalLength = new TLatex(10.5,0.81,Form("#scale[0.85]{#bf{Average interval length: %g}}", averageIntervalLengthLcurve));
  lcurveIntervalLength->Draw();

  output->cd(2);
  undersmoothCoverageHist->SetLineColor(1);   // black
  undersmoothCoverageHist->SetBarWidth(0.9);
  undersmoothCoverageHist->SetFillColor(38);
  undersmoothCoverageHist->SetTitle("Binwise coverage, Undersmoothing");
  undersmoothCoverageHist->GetXaxis()->SetTitle("Bin");
  undersmoothCoverageHist->GetYaxis()->SetTitle("Coverage (1000 repetitions)");
  undersmoothCoverageHist->GetYaxis()->SetRangeUser(0.0, 0.8);
  undersmoothCoverageHist->SetStats(kFALSE);
  undersmoothCoverageHist->Draw("bar");
  nominalCov->SetLineColor(1);  // black
  nominalCov->SetLineStyle(3);  // dotted line; 1: simple, 2: dashed, 3: dotted
  nominalCov->Draw("SAME");
  TLatex *undersmoothIntervalLength = new TLatex(10.5,0.81,Form("#scale[0.85]{#bf{Average interval length: %g}}", averageIntervalLengthUndersmooth));
  undersmoothIntervalLength->Draw();

  TLegend* legendData = new TLegend(0.15,0.7,0.35,0.85);
  output->cd(3);
  unfoldedLcurve->SetStats(kFALSE);
  unfoldedLcurve->SetLineColor(15); // Gray
  unfoldedLcurve->SetTitle("Unfolded, ScanLcurve");
  unfoldedLcurve->GetYaxis()->SetRangeUser(0.0, 1600.0);
  unfoldedLcurve->Draw("");
  legendData->AddEntry(unfoldedLcurve,"Unfolded","l");
  hLambda->SetStats(kFALSE);
  hLambda->SetLineColor(6); // Magenta
  hLambda->Draw("SAME HIST");
  legendData->AddEntry(hLambda,"True","l");
  legendData->Draw();

  output->cd(4);
  unfoldedUndersmooth->SetStats(kFALSE);
  unfoldedUndersmooth->SetLineColor(15); // Gray
  unfoldedUndersmooth->SetTitle("Unfolded, Undersmoothing");
  unfoldedUndersmooth->GetYaxis()->SetRangeUser(0.0, 1600.0);
  unfoldedUndersmooth->Draw("");
  hLambda->SetStats(kFALSE);
  hLambda->SetLineColor(6); // Magenta
  hLambda->Draw("SAME HIST");

  output->SaveAs("UndersmoothDemo.pdf");
}

int main() {
  UndersmoothDemo();
  return 0;
}
