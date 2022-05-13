
#ifndef CONSTANTS_H_GUARD
#define CONSTANTS_H_GUARD
#include <unordered_map>
#include "TMath.h"

static const int MAX_PARTS = 100;
static const int N_SIGMA = 3;
static const float PI = TMath::Pi();
static const float DEG2RAD = PI / 180.0;
static const float RAD2DEG = 180.0 / PI;
static const int POSITIVE = 1;
static const int NEGATIVE = -1;

static const float c_special_units = 29.9792458F;
// misc. constants
static const float FSC = 0.00729735253F;
static const float NA = 6.02214129E23F;               // Avigadro's number
static const float QE = 1.60217646E-19F;              // Charge or electron
static const double FS_ALPHA = 0.007297352570866302;  // Fine structure alpha

static const float rga_E0 = 24.0;

// particle codes, usually PDG codes, but always those used in BOS
static const int PROTON = 2212;
static const int NEUTRON = 2112;
static const int PIP = 211;
static const int PIM = -211;
static const int PI0 = 111;
static const int KP = 321;
static const int KM = -321;
static const int PHOTON = 22;
static const int ELECTRON = 11;

// PDG particle masses in GeV/c2
static const float MASS_P = 0.93827203;
static const float MASS_N = 0.93956556;
static const float MASS_E = 0.000511;
static const float MASS_PIP = 0.13957018;
static const float MASS_PIM = 0.13957018;
static const float MASS_PI0 = 0.1349766;
static const float MASS_KP = 0.493677;
static const float MASS_KM = 0.493677;
static const float MASS_G = 0.0;
static const float MASS_OMEGA = 0.78265;

static std::unordered_map<int, float> mass = {{PROTON, MASS_P}, {-PROTON, MASS_P},  {NEUTRON, MASS_N},  {PIP, MASS_PIP},
                                              {PIM, MASS_PIM},  {PI0, MASS_PI0},    {KP, MASS_KP},      {KM, MASS_KM},
                                              {PHOTON, MASS_G}, {ELECTRON, MASS_E}, {-ELECTRON, MASS_E}};

static std::unordered_map<int, std::string> detector_name = {{0, "both"}, {2, "in_Forward"}, {4, "in_Central"}};
static std::unordered_map<int, std::string> cut_names = {
    {0, "all"}, {1, "1pos"}, {2, "1pos_at180"}, {3, "1pos_MM"}, {4, "1pos_at180_MM"}};
static std::unordered_map<int, int> detector_fill = {{0, 0}, {2, 1}, {4, 2}};
static std::map<int, std::string> before_after_cut = {{0, "_before_cut"}, {1, "_after_cut"}};


#endif
