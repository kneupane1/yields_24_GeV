import numpy as np
from ROOT import TLorentzVector, TVector3
from constants import *

# A couple lambda aliases for numpy
Square = lambda x: np.square(x)
append = lambda _arr, _val: np.append(_arr, _val)

# Beam 4 vector for electron with energy from CLAS12
e_mu = TLorentzVector(0.0, 0.0,
                      np.sqrt(Square(CLAS12_E0) -
                              Square(get_mass('ELECTRON'))),
                      CLAS12_E0)

# Target four vector for Proton at rest
_p_target = TLorentzVector(0, 0, 0, get_mass('PROTON'))


def all_mom(p, cx, cy, cz):
    """Return the momentum in the px, py, and pz direction from the momentum and cosine of angles."""
    calc = lambda _p, _cos: _p * _cos
    return calc(p, cx), calc(p, cy), calc(p, cz)


def fvec(_px, _py, _pz, _mass):
    """Returns four vector given the momentum in each direction and mass of the particle."""
    _vec = TVector3(_px, _py, _pz)
    _4vec = TLorentzVector()
    _4vec.SetVectM(_vec, _mass)
    return _4vec


def fourvec(_p, _cx, _cy, _cz, _mass):
    """Returns four vector given the momentum, angles, and mass of the particle."""
    _px, _py, _pz = all_mom(_p, _cx, _cy, _cz)
    return fvec(_px, _py, _pz, _mass)


def Q2_calc(_e_mu, _e_mu_prime):
    """Retruns Q^2 value: q^mu^2 = (e^mu - e^mu')^2 = -Q^2."""
    _q_mu = (_e_mu - _e_mu_prime)
    return -_q_mu.Mag2()


def W_calc(_e_mu, _e_mu_prime):
    """Returns W: Gotten from s channel [(gamma - P)^2 == s == w^2], Sqrt[M_p^2 - Q^2 + 2 M_p gamma]."""
    _q_mu = (_e_mu - _e_mu_prime)
    return (_p_target + _q_mu).Mag()


def xb_calc(_Q2, _E_prime):
    """Returns bjorken x value from energy and Q^2."""
    _g = CLAS12_E0 - _E_prime
    _xb = (_Q2 / (2 * get_mass('PROTON') * _g))
    return _xb


def xb_calc(_e_mu, _e_mu_prime):
    """Returns bjorken x value from four vectors."""
    _Q2 = _Q2_calc(_e_mu, _e_mu_prime)
    _q = _e_mu - _e_mu_prime
    return (_Q2 / (2 * (_q.Dot(target))))
