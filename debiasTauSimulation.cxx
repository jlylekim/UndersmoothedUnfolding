//==============================================================================
//  Junhyung "Lyle" Kim
//  June 20, 2017
//
//==============================================================================

#if !defined(__CINT__) || defined(__MAKECINT__)
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
#include "TUnfold.h"
#include "Math/ProbFunc.h"
#endif

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

void debiasTauSimulation() {

  gRandom->SetSeed(123456);
  /*
  // get true histogram
  TH1D* hLambda;
  TFile *inputFile=new TFile("hLambda.root");
  inputFile->GetObject("hLambda",hLambda);

  //hLambda->Scale(1.0/1000.0);
  //hLambda->Scale(1.0/50.0);
  hLambda->Scale(dataSize/1000000.0);
  */

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

  // for plotting different delta's (tau's)
  Int_t repeatNum = 10;
  Double_t *tauListY = new Double_t[repeatNum];
  Double_t *tauListX = new Double_t[repeatNum];
  Double_t *intervalLengthList = new Double_t[repeatNum];
  Double_t *intervalLengthListOracle = new Double_t[repeatNum];
  Double_t *intervalLengthListLcurve = new Double_t[repeatNum];

  // for plotting binwise coverage
  // initiating with bin# + 1 since bins start from index 1 in histograms
  // first 0 in binwiseCoverage array will be just 0
  Double_t binwiseCoverageLcurve[41] = {0};
  Double_t binwiseCoverageY[41] = {0};
  Double_t binwiseCoverageX[41];
  // initiating vector to store interval lengths
  vector<Double_t> intervalLengths(40);
  vector<Double_t> intervalLengthsOracle(40);
  vector<Double_t> intervalLengthsLcurve(40);


  // set up constants needed for ScanLcurve
  Double_t tauMin=0.00005;
  //Double_t tauMax=0.1;   //for size, sizeLambdaMC, curvature cases
  Double_t tauMax=0.2; //for curvatureLambdaMC case

  Int_t nScan=50;
  TSpline *logTauX,*logTauY;
  TGraph *lCurve;

  //Double_t biasScaleLK=0.0;
  Double_t biasScaleLK=dataSize/1000000.0;

  // to plot one realization of unfolded histogram
  TH1D *unfoldedLcurve = new TH1D("Unfolded","", 40, -7.0, 7.0);
  TH1D *unfoldedDebias = new TH1D("Unfolded","", 40, -7.0, 7.0);

  for (Int_t i=0; i<repeatNum; i++) {
    Int_t numIterOracle = 0;

    TUnfold* unfoldLcurve = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintNone);

    //TUnfold* unfold = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeSize, TUnfold::kEConstraintNone);
    TUnfold* unfold = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintNone);
    //TUnfold* unfoldOracle = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeSize, TUnfold::kEConstraintNone);
    TUnfold* unfoldOracle = new TUnfold(KAlt, TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintNone);

    TH1D *hLambdaHatInvLcurve = new TH1D("Unfolded","", 40, -7.0, 7.0);
    TH1D *hLambdaHatInv = new TH1D("Unfolded","", 40, -7.0, 7.0);
    TH1D *hLambdaHatInvOracle = new TH1D("Unfolded","", 40, -7.0, 7.0);

    TH1D* hy = generateData();

    unfoldLcurve->SetInput(hy, biasScaleLK);
    unfold->SetInput(hy, biasScaleLK);
    unfoldOracle->SetInput(hy, biasScaleLK);

    // Unfolding using L-curve
    unfoldLcurve->ScanLcurve(nScan,tauMin,tauMax,&lCurve,&logTauX,&logTauY);
    unfold->ScanLcurve(nScan,tauMin,tauMax,&lCurve,&logTauX,&logTauY);

    Double_t deltaFromLcurve = unfold->GetTau();
    //tauListLcurve[i] = deltaFromLcurve;
    Double_t oracleDelta = deltaFromLcurve;

    // for obtaining oracle interval length
    TVectorD oracleCoverage = unfoldOracle->ComputeCoverageOracle(hLambda, oracleDelta);
    // subtracting tolerance (0.01) from the nominal coverage
    while (oracleCoverage.Min() < nominalCoverage - 0.01) {
      oracleDelta = oracleDelta * 0.9;
      oracleCoverage = unfoldOracle->ComputeCoverageOracle(hLambda, oracleDelta);
      numIterOracle++;
    }

    Double_t new_delta = unfold->DebiasTau(deltaFromLcurve, 0.01, 1000);
    std::cout << "New delta is: " << new_delta << std::endl;

    unfold->DoUnfold(new_delta);
    unfoldOracle->DoUnfold(oracleDelta);

    // save delta obtained from debiasTau // i can probably get rid of all the parts pertinent to saving tau
    tauListX[i] = i;
    tauListY[i] = new_delta;

    unfoldLcurve->GetOutput(hLambdaHatInvLcurve);
    unfoldLcurve->GetOutput(unfoldedLcurve);
    unfold->GetOutput(hLambdaHatInv);
    unfold->GetOutput(unfoldedDebias);
    unfoldOracle->GetOutput(hLambdaHatInvOracle);

    // check coverage in each bin
    // should start from 1; GetBinContent(0) and GetBinContent(41) are other information
    for (Int_t j=1; j<41; j++) {
      Double_t hLambdaValue = hLambda->GetBinContent(j);

      Double_t hLambdaHatInvLcurveVar = hLambdaHatInvLcurve->GetBinError(j);
      Double_t hLambdaHatInvLcurveLow = hLambdaHatInvLcurve->GetBinContent(j) - hLambdaHatInvLcurveVar;
      Double_t hLambdaHatInvLcurveUp = hLambdaHatInvLcurve->GetBinContent(j) + hLambdaHatInvLcurveVar;

      Double_t hLambdaHatInvOracleVar = hLambdaHatInvOracle->GetBinError(j);  // to save oracle interval length

      Double_t hLambdaHatInvVar = hLambdaHatInv->GetBinError(j);
      Double_t hLambdaHatInvLow = hLambdaHatInv->GetBinContent(j) - hLambdaHatInvVar;
      Double_t hLambdaHatInvUp = hLambdaHatInv->GetBinContent(j) + hLambdaHatInvVar;

      binwiseCoverageX[j] = j;  // for plotting

      intervalLengthsLcurve[j-1] = hLambdaHatInvLcurveVar * 2.0;
      intervalLengths[j-1] = hLambdaHatInvVar * 2.0;
      intervalLengthsOracle[j-1] = hLambdaHatInvOracleVar * 2.0;

      // check if lcurve scan covered the true lambda
      if (hLambdaHatInvLcurveLow <= hLambdaValue && hLambdaValue <= hLambdaHatInvLcurveUp) {
        // binwiseCoverageY starts from index 0 but it is initialized to 0 so neglect;
        binwiseCoverageLcurve[j] = binwiseCoverageLcurve[j] + (1.0 / repeatNum);
      }

      if (hLambdaHatInvLow <= hLambdaValue && hLambdaValue <= hLambdaHatInvUp) {
        // binwiseCoverageY starts from index 0 but it is initialized to 0 so neglect;
        binwiseCoverageY[j] = binwiseCoverageY[j] + (1.0 / repeatNum);
      }
    }
    // store average interval lengths of the unfolded output in this particular repetition
    intervalLengthListLcurve[i] = TMath::Mean(intervalLengthsLcurve.begin(), intervalLengthsLcurve.end());
    intervalLengthList[i] = TMath::Mean(intervalLengths.begin(), intervalLengths.end());
    intervalLengthListOracle[i] = TMath::Mean(intervalLengthsOracle.begin(), intervalLengthsOracle.end());

    delete hy;
    delete unfold;
    delete unfoldOracle;
    delete unfoldLcurve;
    delete hLambdaHatInv;
    delete hLambdaHatInvOracle;
    delete hLambdaHatInvLcurve;
    cout << "Oracle Iteration #: " << numIterOracle << endl;
  }

  for (Int_t k=1; k<41; k++) {
    //cout << "Delta: " << deltaList[k] << "  Coverage: " << coverageList[k] << endl;
    cout << "Bin #: " << k << " Coverage: " << binwiseCoverageY[k] << endl;
  }


  /*
  ofstream delta_txtfile_lcurve ("26May2017_deltas_Lcurve_fromUS_check.txt");
  if (delta_txtfile_lcurve.is_open()) {
    for(Int_t i=0; i<repeatNum; i++){
        delta_txtfile_lcurve << tauListLcurve[i] << "\n" ;
    }
    delta_txtfile_lcurve.close();
  }
  */

  /*
  ofstream coverage_txtfile ("10000data/26May2017_binwise_coverage_US.txt");
  if (coverage_txtfile.is_open()) {
    for(Int_t i=1; i<41; i++){
        coverage_txtfile << binwiseCoverageY[i] << "\n" ;
    }
    coverage_txtfile.close();
  }

  // write obtained deltas in txt file
  ofstream delta_txtfile ("10000data/26May2017_deltas_US.txt");
  if (delta_txtfile.is_open()) {
    for(Int_t i=0; i<repeatNum; i++){
        delta_txtfile << tauListY[i] << "\n" ;
    }
    delta_txtfile.close();
  }

  ofstream interval_length_txtfile ("10000data/26May2017_average_length_US.txt");
  if (interval_length_txtfile.is_open()) {
    for(Int_t i=0; i<repeatNum; i++){
        interval_length_txtfile << intervalLengthList[i] << "\n" ;
    }
    interval_length_txtfile.close();
  }

  ofstream interval_length_txtfile2 ("10000data/26May2017_average_length_US_oracle.txt");
  if (interval_length_txtfile2.is_open()) {
    for(Int_t i=0; i<repeatNum; i++){
        interval_length_txtfile2 << intervalLengthListOracle[i] << "\n" ;
    }
    interval_length_txtfile2.close();
  }
  */

  // Set up plotting binwise coverage
  gStyle->SetPadTickY(2);
  gStyle->SetTickLength(0.01, "Y");
  gStyle->SetTickLength(0.0, "X");

  TH1D* debiasCoverageHist = new TH1D ("x", "", 40, 0, 41);
  TH1D* lcurveCoverageHist = new TH1D ("x", "", 40, 0, 41);

  for (Int_t i=1;i<41;i++) {
    debiasCoverageHist->SetBinContent(i,binwiseCoverageY[i]);
    lcurveCoverageHist->SetBinContent(i,binwiseCoverageLcurve[i]);
  }

  TCanvas* output = new TCanvas("output");
  output->Divide(2,2);

  //TCanvas* c1 = new TCanvas("c1");
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

  //TCanvas* c2 = new TCanvas("c2");
  output->cd(2);
  debiasCoverageHist->SetLineColor(1);   // black
  debiasCoverageHist->SetBarWidth(0.9);
  debiasCoverageHist->SetFillColor(38);
  debiasCoverageHist->SetTitle("Binwise coverage, Undersmoothing");
  debiasCoverageHist->GetXaxis()->SetTitle("Bin");
  debiasCoverageHist->GetYaxis()->SetTitle("Coverage (1000 repetitions)");
  debiasCoverageHist->GetYaxis()->SetRangeUser(0.0, 0.8);
  debiasCoverageHist->SetStats(kFALSE);
  debiasCoverageHist->Draw("bar");
  nominalCov->SetLineColor(1);  // black
  nominalCov->SetLineStyle(3);  // dotted line; 1: simple, 2: dashed, 3: dotted
  nominalCov->Draw("SAME");

  TLegend* legendData = new TLegend(0.15,0.6,0.4,0.85);
  //TCanvas* c3 = new TCanvas("c3");
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

  //TCanvas* c4 = new TCanvas("c4");
  output->cd(4);
  unfoldedDebias->SetStats(kFALSE);
  unfoldedDebias->SetLineColor(15); // Gray
  unfoldedDebias->SetTitle("Unfolded, Undersmoothing");
  unfoldedDebias->GetYaxis()->SetRangeUser(0.0, 1600.0);
  unfoldedDebias->Draw("");
  hLambda->SetStats(kFALSE);
  hLambda->SetLineColor(6); // Magenta
  hLambda->Draw("SAME HIST");

  output->SaveAs("debiasTauDemo.pdf");
}

#ifndef __CINT__
int main () { debiasTauSimulation(); return 0; }  // Main program when run stand-alone
#endif
