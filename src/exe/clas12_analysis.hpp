#ifndef MAIN_H_GUARD
#define MAIN_H_GUARD

#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "branches.hpp"
#include "colors.hpp"
#include "cuts.hpp"
#include "histogram.hpp"
#include "reaction.hpp"

template <class CutType>
// size_t run(std::shared_ptr<TChain> _chain, const std::shared_ptr<Histogram> &_hists, int thread_id){
size_t run(std::shared_ptr<TChain> _chain, const std::shared_ptr<Histogram>& _hists, int thread_id) {
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
  size_t total_twopion_events = 0;

  // For each event
  for (size_t current_event = 0; current_event < num_of_events; current_event++) {
    // Get current event
    _chain->GetEntry(current_event);

    // If we are the 0th thread print the progress of the thread every 1000 events
    if (thread_id == 0 && current_event % 1000 == 0)
      std::cout << "\t" << (100 * current_event / num_of_events) << " %\r" << std::flush;

    if (data->mc_npart() < 1) continue;

    // // If we pass electron cuts the event is processed
    total++;

    // Make a reaction class from the data given
    auto mc_event = std::make_shared<MCReaction>(data, beam_energy);

    for (int part = 1; part < data->mc_npart(); part++) {
      // Check particle ID's and fill the reaction class

      if (data->mc_pid(part) == PIP) {
        mc_event->SetMCPip(part);

      } else if (data->mc_pid(part) == PROTON) {
        mc_event->SetMCProton(part);

      } else if (data->mc_pid(part) == PIM) {
        mc_event->SetMCPim(part);

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
      _hists->Fill_MomVsBeta(data, part);
      _hists->Fill_deltat_pi(data, dt, part, event);
      _hists->Fill_deltat_prot(data, dt, part, event);

      // Check particle ID's and fill the reaction class
      if (cuts->IsProton(part)) {
        event->SetProton(part);
      } else if (cuts->IsPip(part)) {
        event->SetPip(part);

      } else if (cuts->IsPim(part)) {
        event->SetPim(part);

      } else {
        event->SetOther(part);
      }
    }
    // std::cout << event->weight() << std::endl;

    // if (event->TwoPion_missingPim()) {
    // if (event->TwoPion_missingPip()) {
    // if (event->TwoPion_missingProt()) {
    if (event->TwoPion_exclusive()) {
    //   if (event->W() > 1.25 && event->W() < 2.55 && event->Q2() > 1.5 && event->Q2() < 10.5) {
        if (event->W() > 1.25 && event->W() < 2.55 && event->Q2() > 1.5 && event->Q2() < 30.0 && event->weight() > 0.0) {
        total_twopion_events++;
        _hists->Fill_WvsQ2(event);
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