
#ifndef MAIN_H_GUARD
#define MAIN_H_GUARD

#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "branches.hpp"
#include "colors.hpp"
#include "cuts.hpp"
#include "reaction.hpp"
#include "syncfile.hpp"

template <class CutType>
size_t run(std::shared_ptr<TChain> _chain, const std::shared_ptr<SyncFile>& _sync, int thread_id) {
  // Get the number of events in this thread
  size_t num_of_events = (int)_chain->GetEntries();

  float beam_energy = 10.6;
  if (std::is_same<CutType, rga_Cuts>::value) {
    beam_energy = 10.6;
  } else if (std::is_same<CutType, uconn_Cuts>::value) {
    beam_energy = 10.6;
  }

  if (getenv("BEAM_E") != NULL) beam_energy = atof(getenv("BEAM_E"));

  // Print some information for each thread
  std::cout << "=============== " << RED << "Thread " << thread_id << DEF << " =============== " << BLUE
            << num_of_events << " Events " << DEF << "===============\n";

  // Make a data object which all the branches can be accessed from
  // for sim data use it
  auto data = std::make_shared<Branches12>(_chain, true);
  // for exp data use it
  // auto data = std::make_shared<Branches12>(_chain);

  // Total number of events "Processed"
  size_t total = 0;
  float vertex_hadron[3][3];

  size_t total_twopion_events = 0;

  // For each event
  for (size_t current_event = 0; current_event < num_of_events; current_event++) {
    // Get current event
    _chain->GetEntry(current_event);

    // If we are the 0th thread print the progress of the thread every 1000 events
    if (thread_id == 0 && current_event % 1000 == 0)
      std::cout << "\t" << (100 * current_event / num_of_events) << " %\r" << std::flush;

    int statusPim = -9999;
    int statusPip = -9999;
    int statusProt = -9999;

    if (data->mc_npart() < 1) continue;

    // // If we pass electron cuts the event is processed
    total++;

    // Make a reaction class from the data given
    auto mc_event = std::make_shared<MCReaction>(data, beam_energy);

    for (int part = 1; part < data->mc_npart(); part++) {
      // Check particle ID's and fill the reaction class

      if (data->mc_pid(part) == PIP) {
        mc_event->SetMCPip(part);
        // vertex_hadron[1][0] = data->vx(part);
        // vertex_hadron[1][1] = data->vy(part);
        // vertex_hadron[1][2] = data->vz(part);

      } else if (data->mc_pid(part) == PROTON) {
        mc_event->SetMCProton(part);

        // vertex_hadron[0][0] = data->vx(part);
        // vertex_hadron[0][1] = data->vy(part);
        // vertex_hadron[0][2] = data->vz(part);

      } else if (data->mc_pid(part) == PIM) {
        mc_event->SetMCPim(part);
        // vertex_hadron[2][0] = data->vx(part);
        // vertex_hadron[2][1] = data->vy(part);
        // vertex_hadron[2][2] = data->vz(part);
        // } else {
        //   mc_event->SetMCOther(part);
      }
    }

    auto dt = std::make_shared<Delta_T>(data);
    auto cuts = std::make_shared<uconn_Cuts>(data);
    // auto cuts = std::make_shared<rga_Cuts>(data);
    if (!cuts->ElectronCuts()) continue;

    // Make a reaction class from the data given
    auto event = std::make_shared<Reaction>(data, beam_energy);

    // For each particle in the event
    for (int part = 1; part < data->gpart(); part++) {
      dt->dt_calc(part);

      // Check particle ID's and fill the reaction class
      if (cuts->IsProton(part)) {
        event->SetProton(part);
        statusProt = abs(data->status(part));

        // vertex_hadron[0][0] = data->vx(part);
        // vertex_hadron[0][1] = data->vy(part);
        // vertex_hadron[0][2] = data->vz(part);

      } else if (cuts->IsPip(part)) {
        event->SetPip(part);
        statusPip = abs(data->status(part));

        // vertex_hadron[1][0] = data->vx(part);
        // vertex_hadron[1][1] = data->vy(part);
        // vertex_hadron[1][2] = data->vz(part);

      } else if (cuts->IsPim(part)) {
        event->SetPim(part);
        statusPim = abs(data->status(part));

        vertex_hadron[2][0] = data->vx(part);
        // vertex_hadron[2][1] = data->vy(part);
        // vertex_hadron[2][2] = data->vz(part);

      } else {
        event->SetOther(part);
      }
    }
    // std::cout << event->weight() << std::endl;

    // if (event->TwoPion_missingPim()) {
    // if (event->TwoPion_missingPip()) {
    // if (event->TwoPion_missingProt()) {
    if (event->TwoPion_exclusive()) {
      if (event->W() > 1.25 && event->W() < 2.55 && event->Q2() > 1.5 && event->Q2() < 10.5) {
      // if (event->W() > 1.25 && event->W() < 2.55 && event->Q2() > 1.5 && event->Q2() < 30.0 && event->weight() > 0.0) {
        // if (event->W() > 1.25 && event->W() < 2.55 ) {
        // if (mc_event->W_mc() > 1.25 && mc_event->W_mc() < 2.55 && mc_event->Q2_mc() > 1.5 && mc_event->Q2_mc() < 30.0
        // &&
        //     mc_event->weight() > 0.0) {
        total_twopion_events++;
        // && abs(event->MM2_exclusive()) < 0.03 && abs(event->Energy_excl()) < 0.3) {
        //   //&&
        //   // abs(event->MM2_exclusive()) < 0.03) {
        //   // total++;
        csv_data output;

        // // // // // //  1) for generated
        output.w_mc = mc_event->W_mc();
        output.q2_mc = mc_event->Q2_mc();

        // // // /// 2) reconstructed  and rec exclusive
        output.w = event->W();
        output.q2 = event->Q2();
        output.w_had = event->w_hadron();
        // output.w_diff = event->w_difference();
        // output.sf = (data->ec_tot_energy(0) / (event->elec_mom()));
        // output.elec_prime_m2 = (event->elec_prime_mass2());
        // output.elec_m2 = (event->elec_mass2());
        // output.elec_energy_rec = (event->elec_E());
        // output.elec_mom_rec = (event->elec_mom());
        // output.elec_theta_rec = (event->elec_theta());
        // output.elec_phi_rec = (event->elec_phi());
        // output.status_Elec = abs(data->status(0));
        // output.weight_rec = event->weight();
        // output.no_of_events =

        // // //         // output.status_Elec =  abs(data->status(0));
        // // //         // output.status_Pim = statusPim;
        // // //         // output.status_Pip = statusPip;
        // // //         // output.status_Prot = statusProt;

        // // //         // 3) reconstructed exclusive

        // // //         // output.prot_mom_exclusive = event->prot_momentum_measured();
        // // //         // output.prot_theta_exclusive = event->prot_theta_lab_measured();
        // // //         // output.prot_phi_exclusive = event->prot_Phi_lab_measured();

        // // //         // output.pip_mom_exclusive = event->pip_momentum_measured();
        // // //         // output.pip_theta_exclusive = event->pip_theta_lab_measured();
        // // //         // output.pip_phi_exclusive = event->pip_Phi_lab_measured();

        // // //         // output.pim_mom_exclusive = event->pim_momentum_measured();
        // // //         // output.pim_theta_exclusive = event->pim_theta_lab_measured();
        // // //         // output.pim_phi_exclusive = event->pim_Phi_lab_measured();

        // // //         // output.mm2_mPim = event->MM2();
        // // //         // output.mm2_mPip = event->MM2_mPip();
        // // //         // output.mm2_mProt = event->MM2_mProt();

        // // //         // output.mm2_exclusive_at_zero = event->MM2_exclusive();
        output.energy_x_mu = event->Energy_excl();
        output.mom_x_mu = event->Mom_excl();

        // // //         // output.status_Pim = statusPim;
        // // //         // output.status_Pip = statusPip;
        // // //         // output.status_Prot = statusProt;

        output.weight_rec = event->weight();

        // //         // output.sf = (data->ec_tot_energy(0) / (event->elec_mom()));
        // output.gen_elec_E = mc_event->elec_E_mc_gen();
        // output.gen_elec_mom = mc_event->elec_mom_mc_gen();
        //         output.gen_elec_theta = (mc_event->elec_theta_mc_gen());
        //         output.gen_elec_phi = (mc_event->elec_phi_mc_gen());

        //         output.gen_prot_mom = (mc_event->prot_mom_mc_gen());
        //         output.gen_prot_theta = (mc_event->prot_theta_mc_gen());
        //         output.gen_prot_phi = (mc_event->prot_phi_mc_gen());

        //         output.gen_pip_mom = (mc_event->pip_mom_mc_gen());
        //         output.gen_pip_theta = (mc_event->pip_theta_mc_gen());
        //         output.gen_pip_phi = (mc_event->pip_phi_mc_gen());

        //         output.gen_pim_mom = (mc_event->pim_mom_mc_gen());
        //         output.gen_pim_theta = (mc_event->pim_theta_mc_gen());
        //         output.gen_pim_phi = (mc_event->pim_phi_mc_gen());

        // output.vertex_x = data->vx(0);
        // output.vertex_y = data->vy(0);
        // output.vertex_z = data->vz(0);

        // output.vertex_had[0][0] = vertex_hadron[0][0];
        // output.vertex_had[0][1] = vertex_hadron[0][1];
        // output.vertex_had[0][2] = vertex_hadron[0][2];

        // output.vertex_had[1][0] = vertex_hadron[1][0];
        // output.vertex_had[1][1] = vertex_hadron[1][1];
        // output.vertex_had[1][2] = vertex_hadron[1][2];

        // output.vertex_had[2][0] = vertex_hadron[2][0];
        // output.vertex_had[2][1] = vertex_hadron[2][1];
        // output.vertex_had[2][2] = vertex_hadron[2][2];

        // output.weight_gen = event->weight();
        // output.weight_gen = mc_event->weight();

        _sync->write(output);
      }
    }
  }
  std::cout << "Percent = " << 100.0 * total / num_of_events << std::endl;
  // Return the total number of events
  std::cout << " total no of events = " << total << std::endl;
  std::cout << " total no of twopion events = " << total_twopion_events << std::endl;

  return num_of_events;
}
#endif
