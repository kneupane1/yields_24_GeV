
#ifndef PHYSICS_H_GUARD
#define PHYSICS_H_GUARD
#include <TLorentzVector.h>
#include "TROOT.h"
#include "constants.hpp"

namespace physics {
// Calcuating Q^2
// q^mu^2 = (e^mu - e^mu')^2 = -Q^2
double Q2_calc(const TLorentzVector &e_mu, const TLorentzVector &e_mu_prime);
//	Calcualting W
//	Gotten from s channel [(gamma - P)^2 == s == w^2]
//	Sqrt√[M_p^2 - Q^2 + 2 M_p gamma]
double W_calc(const TLorentzVector &e_mu, const TLorentzVector &e_mu_prime);
double xb_calc(double Q2, double E_prime);
// overload with 4 vectors instaed of other calculations
double xb_calc(const TLorentzVector &e_mu, const TLorentzVector &e_mu_prime);
double theta_calc(double cosz);
double phi_calc(double cosx, double cosy);

double vertex_time(double sc_time, double sc_pathlength, double relatavistic_beta);
double deltat(double electron_vertex_time, double mass, double momentum, double sc_t, double sc_r);
std::shared_ptr<TLorentzVector> fourVec(double px, double py, double pz, double mass);
float phi_boosted(const std::shared_ptr<TLorentzVector> &vec);
}  // namespace physics

#endif
