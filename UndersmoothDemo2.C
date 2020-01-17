
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using std::cout;
using std::endl;
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
#include <TRatioPlot.h>
#include "Math/ProbFunc.h"

TH1D* generateData(Int_t j, vector<vector<string>> data) {
  TH1D* hy = new TH1D ("y", "", 30, 400.0, 1000.0); // Smeared observations
  for (Int_t i=0; i<30; i++) {
    double val = atof(data[i][j].c_str());
    hy->SetBinContent(i+1, val);
  }
  return hy;
}

void UndersmoothDemo2()
{
  gRandom->SetSeed(123456);

  Int_t binSize = 30;

  Double_t* fPertBinsE = new Double_t[binSize];   // lambdaMC
  Double_t* fBinsE = new Double_t[binSize];       // true lambda

  TH1D* hfBinsE = new TH1D ("fBinsE", "", 30, 400.0, 1000.0); // true lambda
  TH1D* hfPertBinsE = new TH1D ("fPertBinsE", "", 30, 400.0, 1000.0); // lambdaMC


  // read true lambda from .csv file
  Int_t ind = 0;
  ifstream fBinsE_csv ("fBinsE.csv");
  if (fBinsE_csv.is_open()) {
    while(!fBinsE_csv.eof()){
        fBinsE_csv >> fBinsE[ind];
        ind = ind + 1;
    }
    fBinsE_csv.close();
  }
  for (Int_t i=0; i<binSize; i++) {
    hfBinsE->SetBinContent(i+1, fBinsE[i]); // true lambda
  }

  // read lambda MC from .csv file
  ind = 0;
  ifstream fPertBinsE_csv ("fPertBinsE.csv");
  if (fPertBinsE_csv.is_open()) {
    while(!fPertBinsE_csv.eof()){
        fPertBinsE_csv >> fPertBinsE[ind];
        ind = ind + 1;
    }
    fPertBinsE_csv.close();
  }
  for (Int_t i=0; i<binSize; i++) {
    hfPertBinsE->SetBinContent(i+1, fPertBinsE[i]); // lambda MC
  }

  // read KAlt (response matrix) from .csv file
  ifstream kHistPert_csv ("kHistPert.csv");
  vector<vector<string>> kPert;
  string line;
  int r = 0;
  while(r < binSize && getline(kHistPert_csv, line)){
    vector<string> row;
    stringstream iss(line);
    int c = 0;
    string val;
    while (c < binSize && getline(iss, val, ',')) {
      row.push_back(val);
    }
    kPert.push_back(row);
  }

  // response matrix in probability version
  TH2D* kHistPert = new TH2D("kHistPert","", 30, 0.0, 1.0, 30, 0.0, 1.0);
  for (int i = 0; i < binSize; i++) {
    //cout << "row num: " << r << endl;
    for (int j = 0; j < binSize; j++) {
      //temp = kPert[i][j];
      double val = atof(kPert[i][j].c_str());
      kHistPert->SetBinContent(i+1,j+1,val);  // KAlt
    }
  }

  // construct response matrix in count version
  TH2D* kHistPertCount = new TH2D("kHistPertCount","", 30, 0.0, 1.0, 30, 0.0, 1.0);
  for (int i = 0; i < binSize; i++) {
    //cout << "row num: " << r << endl;
    for (int j = 0; j < binSize; j++) {
      //temp = kPert[i][j];
      double val = atof(kPert[i][j].c_str());
      //kHistPertCount->(SetBinContent(i+1,j+,val));
      kHistPertCount->SetBinContent(i+1,j+1,(val*hfPertBinsE->GetBinContent(j+1)));
    }
  }

  double colsum[30] = {0.0};
  for (int j=0; j<binSize; j++) {
    double temp = 0.0;
    for (int i=0; i<binSize; i++) {
      temp = temp + kHistPertCount->GetBinContent(i+1,j+1);
    }
    colsum[j] = temp;
    std::cout << j << "th: " << colsum[j] << std::endl;
  }

  for (int j=0; j<binSize; j++) {
    kHistPertCount->SetBinContent(0,j+1, (fPertBinsE[j] - colsum[j]));
  }


  ifstream y10000realizations_csv ("y10000realizations.csv");
  vector<vector<string>> ySeveralData;
  string line2;
  int r2 = 0;
  while(r2 < binSize && getline(y10000realizations_csv, line2)){
    vector<string> row2;
    stringstream iss(line2);
    int c2 = 0;
    string val2;
    while (c2 < binSize && getline(iss, val2, ',')) {
      row2.push_back(val2);
    }
    ySeveralData.push_back(row2);
  }



  // obtain nominal coverage
  Double_t nominalCoverage = ROOT::Math::normal_cdf(1) - ROOT::Math::normal_cdf(-1);

  Int_t repeatNum = 1000;

  // vectors to store confidence interval lengths for each iteration
  vector<Double_t> intervalLengthListUndersmooth(repeatNum);
  vector<Double_t> intervalLengthListLcurve(repeatNum);

  Double_t binwiseCoverageLcurve[31] = {0};
  Double_t binwiseCoverageY[31] = {0};
  Double_t binwiseCoverageX[31];
  // vectors to display average interval lengths
  vector<Double_t> intervalLengthsUndersmooth(30);
  vector<Double_t> intervalLengthsLcurve(30);

  // set up parameters for ScanLcurve
  Double_t tauMin=1.0E-9;
  Double_t tauMax=1.0E-3;

  Int_t nScan=50;
  TSpline *logTauX,*logTauY;
  TGraph *lCurve;

  // to plot one realization of unfolded histogram
  TH1D *unfoldedLcurve = new TH1D("UnfoldedLcurve","", 30, 400.0, 1000.0);
  TH1D *unfoldedUndersmooth = new TH1D("UnfoldedUndersmooth","", 30, 400.0, 1000.0);


  //coverage study
  for (Int_t i=0; i<repeatNum; i++) {
    TUnfold* unfoldLcurve = new TUnfold(kHistPertCount, TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintNone);
    TUnfold* unfoldUndersmooth = new TUnfold(kHistPertCount, TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintNone);

    TH1D *hLambdaHatInvLcurve = new TH1D("LcurveUnfolded","", 30, 400.0, 1000.0);
    TH1D *hLambdaHatInvUndersmooth = new TH1D("UndersmoothUnfolded","", 30, 400.0, 1000.0);

    TH1D* hy = generateData(i, ySeveralData);

    unfoldLcurve->SetInput(hy, 1.0);
    unfoldUndersmooth->SetInput(hy, 1.0);

    // Unfolding using L-curve
    unfoldLcurve->ScanLcurve(nScan,tauMin,tauMax,&lCurve,&logTauX,&logTauY);
    unfoldUndersmooth->ScanLcurve(nScan,tauMin,tauMax,&lCurve,&logTauX,&logTauY);

    Double_t deltaFromLcurve = unfoldUndersmooth->GetTau();

    Double_t new_delta = unfoldUndersmooth->UndersmoothTau(deltaFromLcurve, 0.01, 1000);
    std::cout << "Undersmoothed tau is: " << new_delta << std::endl;

    unfoldUndersmooth->DoUnfold(new_delta);


    unfoldLcurve->GetOutput(hLambdaHatInvLcurve);
    unfoldLcurve->GetOutput(unfoldedLcurve);
    unfoldUndersmooth->GetOutput(hLambdaHatInvUndersmooth);
    unfoldUndersmooth->GetOutput(unfoldedUndersmooth);


    for (Int_t j=1; j<31; j++) {
      Double_t hLambdaValue = hfBinsE->GetBinContent(j);

      Double_t hLambdaHatInvLcurveVar = hLambdaHatInvLcurve->GetBinError(j);
      Double_t hLambdaHatInvLcurveLow = hLambdaHatInvLcurve->GetBinContent(j) - hLambdaHatInvLcurveVar;
      Double_t hLambdaHatInvLcurveUp = hLambdaHatInvLcurve->GetBinContent(j) + hLambdaHatInvLcurveVar;

      Double_t hLambdaHatInvUndersmoothVar = hLambdaHatInvUndersmooth->GetBinError(j);
      Double_t hLambdaHatInvUndersmoothLow = hLambdaHatInvUndersmooth->GetBinContent(j) - hLambdaHatInvUndersmoothVar;
      Double_t hLambdaHatInvUndersmoothUp = hLambdaHatInvUndersmooth->GetBinContent(j) + hLambdaHatInvUndersmoothVar;

      binwiseCoverageX[j] = j;

      intervalLengthsLcurve[j-1] = hLambdaHatInvLcurveVar * 2.0;
      intervalLengthsUndersmooth[j-1] = hLambdaHatInvUndersmoothVar * 2.0;

      // check if ScanLcurve covered the true lambda
      if (hLambdaHatInvLcurveLow <= hLambdaValue && hLambdaValue <= hLambdaHatInvLcurveUp) {
        binwiseCoverageLcurve[j] = binwiseCoverageLcurve[j] + (1.0 / repeatNum);
      }

      // check if UndersmoothTau covered the true lambda
      if (hLambdaHatInvUndersmoothLow <= hLambdaValue && hLambdaValue <= hLambdaHatInvUndersmoothUp) {
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

  for (Int_t k=1; k<31; k++) {
    cout << "Bin #: " << k << " Coverage: " << binwiseCoverageY[k] << endl;
  }

  // obtain average interval lengths
  Double_t averageIntervalLengthLcurve = TMath::Mean(intervalLengthListLcurve.begin(), intervalLengthListLcurve.end());
  Double_t averageIntervalLengthUndersmooth = TMath::Mean(intervalLengthListUndersmooth.begin(), intervalLengthListUndersmooth.end());


  // Set up plotting binwise coverage
  gStyle->SetPadTickY(2);
  gStyle->SetTickLength(0.01, "Y");
  gStyle->SetTickLength(0.0, "X");

  TH1D* undersmoothCoverageHist = new TH1D ("x1", "", 30, 0, 31);
  TH1D* lcurveCoverageHist = new TH1D ("x", "", 30, 0, 31);

  for (Int_t i=1;i<31;i++) {
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
  TLine *nominalCov = new TLine(0,nominalCoverage,31,nominalCoverage);
  nominalCov->SetLineColor(1);  // black
  nominalCov->SetLineStyle(3);  // dotted line; 1: simple, 2: dashed, 3: dotted
  nominalCov->Draw("SAME");
  TLatex *lcurveIntervalLength = new TLatex(7.0,0.81,Form("#scale[0.85]{#bf{Average interval length: %g}}", averageIntervalLengthLcurve));
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
  TLatex *undersmoothIntervalLength = new TLatex(7.0,0.81,Form("#scale[0.85]{#bf{Average interval length: %g}}", averageIntervalLengthUndersmooth));
  undersmoothIntervalLength->Draw();

  TLegend* legendData = new TLegend(0.65,0.7,0.85,0.85);


  // ratio plot added
  gStyle->SetOptStat(0);
  gStyle->SetTitleFontSize(0.11);
  // gStyle->SetTitleOffset(5.0);
  gStyle->SetTitleY(1.02); //title Y location
  // gStyle->SetTitleAlign(5);
  output->cd(3);
  hfBinsE->SetLineColor(6); // Magenta
  unfoldedLcurve->SetLineColor(15); // Gray
  unfoldedLcurve->SetTitle("Unfolded, ScanLcurve");
  auto ratio_Lcurve = new TRatioPlot(unfoldedLcurve, hfBinsE, "diff");
  ratio_Lcurve -> Draw();
  ratio_Lcurve -> SetSplitFraction(0.55);
  ratio_Lcurve->GetLowerRefGraph()->SetMaximum(1.7E7);
  ratio_Lcurve->GetLowerRefGraph()->SetMinimum(-1.7E7);

  output->cd(4);
  unfoldedUndersmooth->SetLineColor(15); // Gray
  unfoldedUndersmooth->SetTitle("Unfolded, Undersmoothing");
  auto ratio_US = new TRatioPlot(unfoldedUndersmooth, hfBinsE, "diff");
  ratio_US -> Draw();
  ratio_US -> SetSplitFraction(0.55);
  ratio_US->GetLowerRefGraph()->SetMaximum(1.7E7);
  ratio_US->GetLowerRefGraph()->SetMinimum(-1.7E7);


  output->SaveAs("UndersmoothDemo2.pdf");
}

int main() {
  UndersmoothDemo2();
  return 0;
}
