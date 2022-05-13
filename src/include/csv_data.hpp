#ifndef CSV_DATA_H_GUARD
#define CSV_DATA_H_GUARD

#include <fstream>
#include <string>

struct csv_data {
  short electron_sector;
  float sf;
  float w;
  float w_mc;
  float q2_mc;

  float elec_mom_rec;
  float elec_theta_rec;
  float elec_phi_rec;

  float q2;

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
    // return "w,q2,sf,elec_mom_rec,elec_th_rec,elec_phi_rec,prot_mom_mes,prot_theta_mes,prot_phi_mes,pip_mom_mes,pip_theta_"
    //        "mes,pip_phi_mes,pim_mom_mes,pim_theta_mes,pim_phi_mes,mm2_mPim,mm2_mPip,mm2_mProt,mm2_exclusive_at_zero,energy_x_mu,"
    //        "status_Pim,status_Pip,status_Prot,weight";
    return "w,q2,sf,elec_mom_rec,elec_th_rec,elec_phi_rec,weight,status_elec,status_Pim,status_Pip,status_Prot";

    // return "w_mc,q2_mc,elec_mom_gen,elec_th_gen,elec_phi_gen,prot_mom_gen,prot_th_gen,prot_phi_gen,pip_mom_gen,pip_th_"
    //        "gen,pip_phi_gen,pim_mom_gen,pim_th_gen,pim_phi_gen,weight";
  }

  friend std ::ostream &operator<<(std::ostream &os, const csv_data &data) {
    os << std::setprecision(5);
    os << data.w << ",";
    os << data.q2 << ",";
    os << data.sf << ",";

    os << data.elec_mom_rec << ",";
    os << data.elec_theta_rec << ",";
    os << data.elec_phi_rec << ",";
    os << std::setprecision(10);
    os << data.weight_rec<< ",";
    os << std::setprecision(5);
    os << data.status_Elec << ",";
    os << data.status_Pim << ",";
    os << data.status_Pip << ",";
    os << data.status_Prot << ",";


    // os << data.w_mc << ",";
    // os << data.q2_mc << ",";

    // os << data.gen_elec_mom << ",";
    // os << data.gen_elec_theta<< ",";
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

    // os << data.weight_gen<< ",";

    
    return os;
  }
};

#endif
