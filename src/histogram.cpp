
#include "histogram.hpp"

Histogram::Histogram(const std::string &output_file)
{
        RootOutputFile = std::make_shared<TFile>(output_file.c_str(), "RECREATE");
        def = std::make_shared<TCanvas>("def");
        if (getenv("BEAM_E") != NULL)
        {
                if (atof(getenv("BEAM_E")) < 3)
                {
                        q2_max = 1.0;
                        w_max = 3.5;
                        p_max = 3.0;
                }
                else if (atof(getenv("BEAM_E")) < 8)
                {
                        q2_max = 4.0;
                        w_max = 4.0;
                        p_max = 4.0;
                }
                else if (atof(getenv("BEAM_E")) < 9)
                {
                        q2_max = 7.0;
                        w_max = 7.0;
                        p_max = 7.0;
                }
        }

        momentum = std::make_shared<TH1D>("mom", "mom", bins, p_min, p_max);
        W_hist = std::make_shared<TH1D>("W", "W", bins, zero, w_max);
        Q2_hist = std::make_shared<TH1D>("Q2", "Q2", bins, zero, q2_max);
        W_vs_q2 = std::make_shared<TH2D>("W_vs_q2", "W_vs_q2", bins, zero, w_max,
                                         bins, zero, q2_max);
        makeHists_deltat();
        makeHists_MomVsBeta();
        makeHists_sector();
}

Histogram::~Histogram()
{
        this->Write();
}

void Histogram::Write()
{
        // std::cout << GREEN << "Writting" << DEF << std::endl;

        // // // //  Write_EC();
        std::cerr << BOLDBLUE << "WvsQ2()" << DEF << std::endl;
        TDirectory *WvsQ2_folder = RootOutputFile->mkdir("W vs Q2");
        WvsQ2_folder->cd();
        Write_WvsQ2();

        std::cerr << BOLDBLUE << "Write_MomVsBeta()" << DEF << std::endl;
        TDirectory *Write_MomVsBeta_folder = RootOutputFile->mkdir("Mom Vs Beta");
        Write_MomVsBeta_folder->cd();
        Write_MomVsBeta();

        std::cerr << BOLDBLUE << "Write_deltat()" << DEF << std::endl;
        TDirectory *Write_deltat_folder = RootOutputFile->mkdir("Delta_t");
        Write_deltat_folder->cd();
        Write_deltat();

        std::cerr << BOLDBLUE << "Done Writing!!!" << DEF << std::endl;
}



void Histogram::Fill_WvsQ2(const std::shared_ptr<Reaction> &_e)
{
        short sec = _e->sec();

        W_hist->Fill(_e->W()), _e->weight();
        Q2_hist->Fill(_e->Q2(), _e->weight());
        W_vs_q2->Fill(_e->W(), _e->Q2(), _e->weight());

        if (sec > 0 && sec <= 6)
        {
                W_vs_q2_sec[sec - 1]->Fill(_e->W(), _e->Q2(), _e->weight());
                W_sec[sec - 1]->Fill(_e->W()), _e->weight();
        }
}

void Histogram::Write_WvsQ2()
{
       auto WvsQ2_can =
            std::make_unique<TCanvas>("WvsQ2_can", "W vs Q2 sectors", 1920, 1080);
        WvsQ2_can->Divide(3, 2);
        for (short i = 0; i < num_sectors; i++)
        {
                W_vs_q2_sec[i]->SetYTitle("Q^{2} (GeV^{2})");
                W_vs_q2_sec[i]->SetXTitle("W (GeV)");
                W_vs_q2_sec[i]->SetOption("COLZ1");
                WvsQ2_can->cd(i + 1);
                W_vs_q2_sec[i]->Draw("same");
        }
        WvsQ2_can->Write();

        auto W_can = std::make_unique<TCanvas>("W_can", "W sectors", 1920, 1080);
        W_can->Divide(3, 2);
        for (short i = 0; i < num_sectors; i++)
        {
                W_sec[i]->SetXTitle("W (GeV)");
                W_can->cd(i + 1);

                //  W_sec[i]->Fit("gaus", "QMR+", "QMR+", 0.85, 1.05);
                // gStyle->SetOptFit(01);
                W_sec[i]->Draw("same");
        }
        W_can->Write();


        W_vs_q2->SetXTitle("W (GeV)");
        W_vs_q2->SetYTitle("Q^{2} (GeV^{2})");
        W_vs_q2->SetOption("COLZ1");
        if (W_vs_q2->GetEntries())
                W_vs_q2->Write();

        W_hist->SetXTitle("W (GeV)");
        if (W_hist->GetEntries())
                W_hist->Write();

        Q2_hist->SetXTitle("Q^{2} (GeV^{2})");
        if (Q2_hist->GetEntries())
                Q2_hist->Write();


}
void Histogram::makeHists_sector()
{

        for (short i = 0; i < num_sectors; i++)
        {
                W_vs_q2_sec[i] = std::make_shared<TH2D>(
                    Form("wvsq2_sec_%d", i + 1), Form("W vs Q^{2} Sector: %d", i + 1), bins,
                    zero, w_max, bins, zero, q2_max);

                W_sec[i] =
                    std::make_shared<TH1D>(Form("w_sec_%d", i + 1),
                                           Form("W Sector: %d", i + 1), bins, zero, w_max);

        }
}

void Histogram::makeHists_deltat()
{
        std::string tof = "";
        for (short p = 0; p < particle_num; p++)
        {
                for (short c = 0; c < charge_num; c++)
                {
                        for (short i = 0; i < with_id_num; i++)
                        {
                                tof = "ftof";
                                delta_t_hist[p][c][i][0] = std::make_shared<TH2D>(
                                    Form("delta_t_%s_%s_%s_%s", tof.c_str(), particle_name[p].c_str(),
                                         charge_name[c].c_str(), id_name[i].c_str()),
                                    Form("#Deltat %s %s %s %s", tof.c_str(), particle_name[p].c_str(),
                                         charge_name[c].c_str(), id_name[i].c_str()),
                                    bins, p_min, p_max, bins, Dt_min, Dt_max);

                                tof = "ctof";
                                delta_t_hist[p][c][i][1] = std::make_shared<TH2D>(
                                    Form("delta_t_%s_%s_%s_%s", tof.c_str(), particle_name[p].c_str(),
                                         charge_name[c].c_str(), id_name[i].c_str()),
                                    Form("#Deltat %s %s %s %s", tof.c_str(), particle_name[p].c_str(),
                                         charge_name[c].c_str(), id_name[i].c_str()),
                                    bins, 0, 3.0, bins, -6.0, 6.0);
                        }
                }
        }
}

void Histogram::Fill_deltat_pi(const std::shared_ptr<Branches12> &data,
                               const std::shared_ptr<Delta_T> &dt, int part, const std::shared_ptr<Reaction> &_e)
{
        auto _cuts = std::make_unique<Cuts>(data, dt);
        int charge = data->charge(part);
        bool fc = dt->ctof();
        int pid = data->pid(part);
        float mom = data->p(part);
        float time = NAN;
        if (fc)
                time = dt->dt_ctof_Pi();
        else
                time = dt->dt_Pi();

        if (charge == 1)
        {
                delta_t_hist[1][0][0][fc]->Fill(mom, time, _e->weight());
                if (_cuts->IsPip(part))
                        delta_t_hist[1][0][1][fc]->Fill(mom, time, _e->weight());
                else
                        delta_t_hist[1][0][2][fc]->Fill(mom, time, _e->weight());
        }
        else if (charge == -1)
        {
                delta_t_hist[1][1][0][fc]->Fill(mom, time, _e->weight());
                //if (_cuts->IsPim(part))
                if (pid != ELECTRON)
                        delta_t_hist[1][1][1][fc]->Fill(mom, time, _e->weight());
                else
                        delta_t_hist[1][1][2][fc]->Fill(mom, time, _e->weight());
        }
}

void Histogram::Fill_deltat_prot(const std::shared_ptr<Branches12> &data,
                                 const std::shared_ptr<Delta_T> &dt, int part, const std::shared_ptr<Reaction> &_e)
{
        auto _cuts = std::make_unique<Cuts>(data, dt);
        int status = abs(data->status(part));
        int charge = data->charge(part);
        bool fc = dt->ctof();
        int pid = data->pid(part);
        float mom = data->p(part);
        float time = NAN;

        if (fc)
                //if(status >=4000 && status < 6000){
                time = dt->dt_ctof_P();
        //}
        else
                time = dt->dt_P();

        if (charge == 1)
        {
                delta_t_hist[2][0][0][fc]->Fill(mom, time, _e->weight());
                if (_cuts->IsProton(part))
                        delta_t_hist[2][0][1][fc]->Fill(mom, time, _e->weight());
                else
                        delta_t_hist[2][0][2][fc]->Fill(mom, time, _e->weight());

                delta_t_hist[2][1][0][fc]->Fill(mom, time, _e->weight());
                if (pid == PROTON)
                        delta_t_hist[2][1][1][fc]->Fill(mom, time, _e->weight());
                else
                        delta_t_hist[2][1][2][fc]->Fill(mom, time, _e->weight());
        }
}

// do
// pid == 11 at first;
// skim vs pid at 0 is 11. compare.
void Histogram::Write_deltat()
{
        TDirectory *ftof_folder = RootOutputFile->mkdir("ftof");
        ftof_folder->cd();
        for (short p = 0; p < particle_num; p++)
        {
                for (short c = 0; c < charge_num; c++)
                {
                        for (short i = 0; i < with_id_num; i++)
                        {
                                delta_t_hist[p][c][i][0]->SetXTitle("Momentum (GeV)");
                                delta_t_hist[p][c][i][0]->SetYTitle("#Deltat");
                                delta_t_hist[p][c][i][0]->SetOption("COLZ1");
                                if (delta_t_hist[p][c][i][0]->GetEntries() > 1)
                                        delta_t_hist[p][c][i][0]->Write();
                        }
                }
        }
        TDirectory *ctof_folder = RootOutputFile->mkdir("ctof");
        ctof_folder->cd();
        for (short p = 0; p < particle_num; p++)
        {
                for (short c = 0; c < charge_num; c++)
                {
                        for (short i = 0; i < with_id_num; i++)
                        {
                                delta_t_hist[p][c][i][1]->SetXTitle("Momentum (GeV)");
                                delta_t_hist[p][c][i][1]->SetYTitle("#Deltat");
                                delta_t_hist[p][c][i][1]->SetOption("COLZ1");
                                if (delta_t_hist[p][c][i][1]->GetEntries() > 1)
                                        delta_t_hist[p][c][i][1]->Write();
                        }
                }
        }
}

void Histogram::makeHists_MomVsBeta()
{
        for (short p = 0; p < particle_num; p++)
        {
                for (short c = 0; c < charge_num; c++)
                {
                        for (short i = 0; i < with_id_num; i++)
                        {
                                momvsbeta_hist[p][c][i] = std::make_shared<TH2D>(
                                    Form("mom_vs_beta_%s_%s_%s", particle_name[p].c_str(),
                                         charge_name[c].c_str(), id_name[i].c_str()),
                                    Form("Momentum vs #beta %s %s %s", particle_name[p].c_str(),
                                         charge_name[c].c_str(), id_name[i].c_str()),
                                    bins, p_min, p_max, bins, zero, 1.2);
                        }
                }
        }
}

void Histogram::Fill_MomVsBeta(const std::shared_ptr<Branches12> &data,
                               int part)
{
        int good_ID = 0;
        float beta = data->beta(part);
        float mom = data->p(part);
        int charge = data->charge(part);
        int pid = data->pid(part);
        if (beta != 0)
        {
                momentum->Fill(mom);
                for (short p = 0; p < particle_num; p++)
                {
                        switch (p)
                        {
                        case 0:
                                good_ID = ELECTRON;
                                break;
                        case 1:
                                good_ID = PIP;
                                break;
                        case 2:
                                good_ID = PROTON;
                                break;
                        case 3:
                                good_ID = KP;
                                break;
                        }

                        momvsbeta_hist[p][0][0]->Fill(mom, beta);
                        if (good_ID == abs(pid))
                        {
                                momvsbeta_hist[p][0][1]->Fill(mom, beta);
                        }
                        else
                        {
                                momvsbeta_hist[p][0][2]->Fill(mom, beta);
                        }

                        if (charge == -1)
                        {
                                momvsbeta_hist[p][2][0]->Fill(mom, beta);
                                if (-good_ID == pid)
                                {
                                        momvsbeta_hist[p][2][1]->Fill(mom, beta);
                                }
                                else
                                {
                                        momvsbeta_hist[p][2][2]->Fill(mom, beta);
                                }
                        }
                        else if (charge == 1)
                        {
                                momvsbeta_hist[p][1][0]->Fill(mom, beta);
                                if (good_ID == pid)
                                {
                                        momvsbeta_hist[p][1][1]->Fill(mom, beta);
                                }
                                else
                                {
                                        momvsbeta_hist[p][1][2]->Fill(mom, beta);
                                }
                        }
                }
        }
}

void Histogram::Write_MomVsBeta()
{
        momentum->SetXTitle("Momentum (GeV)");
        momentum->Write();
        for (short p = 0; p < particle_num; p++)
        {
                for (short c = 0; c < charge_num; c++)
                {
                        for (short i = 0; i < with_id_num; i++)
                        {
                                momvsbeta_hist[p][c][i]->SetXTitle("Momentum (GeV)");
                                momvsbeta_hist[p][c][i]->SetYTitle("#beta");
                                momvsbeta_hist[p][c][i]->SetOption("COLZ1");
                                momvsbeta_hist[p][c][i]->Write();
                        }
                }
        }
}
