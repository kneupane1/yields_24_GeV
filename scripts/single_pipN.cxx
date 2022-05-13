#include <iostream>
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"

int single_pipN(const std::string &data) {
  gStyle->SetPalette(kViridis);

  TFile *root_data = new TFile(data.c_str());
  TCanvas *can_sec[6];

  for (size_t sec = 0; sec < 6; sec++) {
    can_sec[sec] = new TCanvas(Form("Canvas Sector %lu", sec + 1), Form("Canvas Sector %lu", sec + 1), 1600, 900);
    can_sec[sec]->Divide(2, 2);

    can_sec[sec]->cd(1);
    TH2D *wvsq2 = (TH2D *)root_data->Get(Form("Npip_sec/wvsq2_sec_Npip_%lu", sec + 1));
    wvsq2->Draw();

    can_sec[sec]->cd(2);
    TH2D *dt = (TH2D *)root_data->Get(Form("ftof/delta_t_pip_%lu", sec));
    dt->GetXaxis()->SetRangeUser(0.0, 6.0);
    dt->GetYaxis()->SetRangeUser(-0.6, 0.6);
    dt->Draw();

    can_sec[sec]->cd(4);
    TH1D *mm_full = (TH1D *)root_data->Get(Form("singlePip_sec/MM_Sec_%lu", sec + 1));
    mm_full->Draw();
    TH1D *mm_npip = (TH1D *)root_data->Get(Form("Npip_sec/MM_Npip_Sec_%lu", sec + 1));
    mm_npip->SetFillColorAlpha(kRed, 0.2);
    mm_npip->Draw("SAME");
    can_sec[sec]->cd(3);
    TH1D *w_npip = (TH1D *)root_data->Get(Form("Npip_sec/w_sec_Npip_%lu", sec + 1));
    w_npip->Draw();

    can_sec[sec]->Print(Form("npip_sector_%lu.png", sec + 1));
  }

  return 0;
}

#if not defined(__CLING__)
int main(int argc, char const *argv[]) {
  if (argc < 1) {
    std::cerr << "Not enough arguments" << std::endl;
    std::cerr << "To Use:\t" << argv[0] << " data.root" << std::endl;
    exit(1);
  }

  return single_pipN(argv[1]);
}
#endif