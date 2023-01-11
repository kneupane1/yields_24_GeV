#ifndef CSV_DATA_H_GUARD
#define CSV_DATA_H_GUARD

#include <fstream>
#include <string>

struct csv_data {
  short electron_sector;
  float sf;
  float w;
  float w_mc;
  double elec_prime_m2;
  double elec_m2;
  double elec_energy_rec;
  float q2_mc;
  float w_had;
  float w_diff;
  float elec_mom_rec;
  float elec_theta_rec;
  float elec_phi_rec;

  float energy_x_mu;
  float mom_x_mu;

  float q2;

  float gen_elec_E;
  float gen_elec_mom;
  float gen_elec_theta;
  float gen_elec_phi;

  float gen_pim_mom;
  float gen_pim_theta;
  float gen_pim_phi;

  float gen_pip_mom;
  float gen_pip_theta;
  float gen_pip_phi;

  float gen_prot_mom;
  float gen_prot_theta;
  float gen_prot_phi;

  float vertex_x;
  float vertex_y;
  float vertex_z;

  float vertex_had[3][3];

  float weight_gen;
  float weight_rec;

  int status_Elec;
  int status_Pim;
  int status_Pip;
  int status_Prot;

  // Static functions can be called without making a new struct
  static std::string header() {
    // Make a string for the header of the csv file mPim case
    // 24 GeV test
    // return
    // "w,q2,sf,elec_mom_rec,elec_th_rec,elec_phi_rec,prot_mom_mes,prot_theta_mes,prot_phi_mes,pip_mom_mes,pip_theta_"
    //        "mes,pip_phi_mes,pim_mom_mes,pim_theta_mes,pim_phi_mes,mm2_mPim,mm2_mPip,mm2_mProt,mm2_exclusive_at_zero,energy_x_mu,"
    //        "status_Pim,status_Pip,status_Prot,weight";
    // return "w,q2,elec_E_rec,elec_mom_rec,elec_th_rec,elec_phi_rec,status_elec,"
    //        "weight";
    // return "w,q2,w_had,weight,energy_x_mu,mom_x_mu,w_mc,q2_mc";
    // return "w,q2,w_had,weight,energy_x_mu,mom_x_mu";

    // return "vx_elec,vy_elec,vz_elec,vx_prot,vy_prot,vz_prot,vx_pip,vy_pip,vz_pip,vx_pim,vy_pim,vz_pim"
    //        ",weight";
    return "w_gen,q2_gen,w_rec,q2_rec,w_had,energy_x_mu,mom_x_mu,weight";
    // ,elec_mom_gen, elec_th_gen, elec_phi_gen, prot_mom_gen, prot_th_gen, prot_phi_gen, pip_mom_gen, pip_th_ "
    //        "gen,pip_phi_gen,pim_mom_gen,pim_th_gen,pim_phi_gen,weight";
  }

  friend std ::ostream &operator<<(std::ostream &os, const csv_data &data) {
    os << std::setprecision(7);
    os << data.w_mc << ",";
    os << data.q2_mc << ",";

    // // os << data.gen_elec_E << ",";
    // os << data.gen_elec_mom << ",";
    // os << data.gen_elec_theta << ",";
    // os << data.gen_elec_phi << ",";

    // os << data.gen_prot_mom << ",";
    // os << data.gen_prot_theta << ",";
    // os << data.gen_prot_phi << ",";

    // os << data.gen_pip_mom << ",";
    // os << data.gen_pip_theta << ",";
    // os << data.gen_pip_phi <<",";

    // os << data.gen_pim_mom << ",";
    // os << data.gen_pim_theta << ",";
    // os << data.gen_pim_phi<< ",";
    os << data.w << ",";
    os << data.q2 << ",";
    os << data.w_had << ",";
    // // os << data.w_diff << ",";
    // os << data.sf << ",";
    // // os << std::setprecision(10);
    // os << data.elec_prime_m2 << ",";
    // os << data.elec_m2 << ",";
    // os << data.elec_energy_rec << ",";
    // os << data.elec_mom_rec << ",";
    // os << data.elec_theta_rec << ",";
    // os << data.elec_phi_rec << ",";
    // os << std::setprecision(1);

    // os << data.status_Elec << ",";

    // os << std::setprecision(8);
    // os << std::setprecision(7);
    // os << data.status_Elec << ",";
    // // // os << data.status_Pim << ",";
    // // // os << data.status_Pip << ",";
    // // // os << data.status_Prot << ",";

    os << data.energy_x_mu << ",";
    os << data.mom_x_mu << ",";


    // // os << std::setprecision(8);
    os << data.weight_rec << ",";

    // os << data.vertex_x << ",";
    // os << data.vertex_y << ",";
    // os << data.vertex_z << ",";

    // os << data.vertex_had[0][0] << ",";
    // os << data.vertex_had[0][1] << ",";
    // os << data.vertex_had[0][2] << ",";

    // os << data.vertex_had[1][0] << ",";
    // os << data.vertex_had[1][1] << ",";
    // os << data.vertex_had[1][2] << ",";

    // os << data.vertex_had[2][0] << ",";
    // os << data.vertex_had[2][1] << ",";
    // os << data.vertex_had[2][2] << ",";

    // os << data.weight_gen<< ",";

    return os;
  };
};

#endif
