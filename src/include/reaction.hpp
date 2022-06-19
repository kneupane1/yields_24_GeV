
#ifndef REACTION_H_GUARD
#define REACTION_H_GUARD

#include <iostream>
#include "TLorentzRotation.h"
#include "TLorentzVector.h"
#include "branches.hpp"
#include "constants.hpp"
#include "physics.hpp"

class Reaction {
 protected:
  std::shared_ptr<Branches12> _data;

  double _beam_energy = 24.0;
  std::unique_ptr<TLorentzVector> _beam;
  std::unique_ptr<TLorentzVector> _elec;
  std::unique_ptr<TLorentzVector> _gamma;
  std::unique_ptr<TLorentzVector> _target;
  std::unique_ptr<TLorentzVector> _prot;
  std::unique_ptr<TLorentzVector> _pip;
  std::unique_ptr<TLorentzVector> _pim;
  std::unique_ptr<TLorentzVector> _other;
  std::unique_ptr<TLorentzVector> _neutron;


  bool _mc = false;

  bool _hasE = false;
  bool _hasP = false;
  bool _hasPip = false;
  bool _hasPim = false;
  bool _hasOther = false;
  bool _hasNeutron = false;

  short _numProt = 0;
  short _numPip = 0;
  short _numPim = 0;
  short _numPos = 0;
  short _numNeg = 0;
  short _numNeutral = 0;
  short _numOther = 0;

  short _sector = -1;

  float _MM = NAN;
  float _MM2 = NAN;
  float _MM2_exclusive = NAN;
  float _excl_Energy = NAN;
  float _excl_Mom = NAN;;
  float _MM2_mPip = NAN;
  float _MM2_mProt = NAN;

  float _W = NAN;
  float _Q2 = NAN;
  double _emu_prime_mag2 = NAN;;
  double _emu_mag2 = NAN;;
  double _elec_energy = NAN;
  double _elec_mom = NAN;
  float _theta_e = NAN;
  float _elec_phi = NAN;

  float _prot_status = NAN;
  float _pip_status = NAN;
  float _pim_status = NAN;

  void SetElec();




 public:
  Reaction(){};
  Reaction(const std::shared_ptr<Branches12> &data, float beam_energy);
  ~Reaction();
  inline float weight() {
    return _data->mc_weight();
    // return 1.0;
  }
  // Check lists when you swich from mc to exp or vice-versa
  // 1. inline weight function above
  // 2. clas12_yields: auto data = std::make_shared<Branches12>(_chain, true);  turn off true for data
  // 3. from if (data->mc_npart() < 1) to all particle set up im mc events.
  // 4. all mc bank related (generated) output parameters will not work in exp data

  // momentum correction
  void SetMomCorrElec();
  double dpp(float px, float py, float pz, int sec_mom_corr, int ivec);
  double elec_mom();
  double elec_E();
  double elec_theta();
  double elec_phi();
  double elec_prime_mass2();
  double elec_mass2();

  inline bool mc() { return _mc; }
  void SetProton(int i);
  void SetPip(int i);
  void SetPim(int i);
  void SetOther(int i);
  void SetNeutron(int i);

  // missingPim
  float pim_momentum();
  float pim_theta_lab();
  float pim_Phi_lab();
  float pim_momentum_measured();
  float pim_theta_lab_measured();
  float pim_Phi_lab_measured();

  // missingPip
  float pip_momentum();
  float pip_theta_lab();
  float pip_Phi_lab();
  float pip_momentum_measured();
  float pip_theta_lab_measured();
  float pip_Phi_lab_measured();

  // missingProt
  float prot_momentum();
  float prot_theta_lab();
  float prot_Phi_lab();
  float prot_momentum_measured();
  float prot_theta_lab_measured();
  float prot_Phi_lab_measured();

  void CalcMissMass();
  float MM();
  float MM2();
  float MM2_exclusive();
  float Energy_excl();
  float Mom_excl();
  float MM2_mPip();
  float MM2_mProt();

  float w_hadron();
  float w_difference();

  inline float W() { return _W; }
  inline float Q2() { return _Q2; }

  inline short sec() { return _data->dc_sec(0); }
  inline int det() { return abs(_data->status(0) / 1000); }

  inline bool TwoPion_missingPim() {
    bool _channelTwoPi = true;
    _channelTwoPi &= ((_numProt == 1 && _numPip == 1) && (_hasE && _hasP  && _hasPip));
    return _channelTwoPi;
  }

  inline bool TwoPion_exclusive() {
    bool _channelTwoPi_excl = true;

    _channelTwoPi_excl &= ((_numProt == 1 && _numPip == 1 && _numPim == 1) &&
                           (_hasE && _hasP && _hasPip && _hasPim /*&& !_hasNeutron && !_hasOther*/));
    return _channelTwoPi_excl;
  }
  inline bool TwoPion_missingPip() {
    bool _channelTwoPi_mpip = true;

    _channelTwoPi_mpip &=
        ((_numProt == 1 && _numPim == 1) && (_hasE && _hasP && _hasPim /*&&!_hasPip && !_hasNeutron && !_hasOther*/));
    return _channelTwoPi_mpip;
  }
  inline bool TwoPion_missingProt() {
    bool _channelTwoPi_mprot = true;
    _channelTwoPi_mprot &=
        ((_numPip == 1 && _numPim == 1) && (_hasE && _hasPip && _hasPim /*&&!_hasP  && !_hasOther*/));
    return _channelTwoPi_mprot;
  }

  const TLorentzVector &e_mu() { return *_beam; }
  const TLorentzVector &e_mu_prime() { return *_elec; }
  const TLorentzVector &gamma() { return *_gamma; }
};

class MCReaction : public Reaction {
 private:
  float _weight_mc = NAN;
  float _W_mc = NAN;
  float _Q2_mc = NAN;

  std::unique_ptr<TLorentzVector> _elec_mc;
  std::unique_ptr<TLorentzVector> _gamma_mc;
  std::unique_ptr<TLorentzVector> _prot_mc;
  std::unique_ptr<TLorentzVector> _pip_mc;
  std::unique_ptr<TLorentzVector> _pim_mc;
  std::unique_ptr<TLorentzVector> _other_mc;

  float _MM2_exclusive_mc = NAN;
  float _excl_Energy_mc = NAN;
 public:
  void SetMCProton(int i);
  void SetMCPip(int i);
  void SetMCPim(int i);
  void SetMCOther(int i);

  MCReaction(const std::shared_ptr<Branches12> &data, float beam_energy);
  void SetMCElec();
  inline float weight() { return _data->mc_weight(); }
  inline float W_mc() { return _W_mc; }
  inline float Q2_mc() { return _Q2_mc; }

  float elec_mom_mc_gen();
  float pim_mom_mc_gen();
  float pip_mom_mc_gen();
  float prot_mom_mc_gen();

  float elec_E_mc_gen();

  float elec_theta_mc_gen();
  float pim_theta_mc_gen();
  float pip_theta_mc_gen();
  float prot_theta_mc_gen();

  float elec_phi_mc_gen();
  float pim_phi_mc_gen();
  float pip_phi_mc_gen();
  float prot_phi_mc_gen();

  void CalcMissMass_mc();

};

#endif
