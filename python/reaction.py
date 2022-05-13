import numpy as np
from ROOT import TLorentzVector


class reaction():
    def __init__(self, *args, **kwargs):
        # Branches12 _data
        _beam_energy = 7.5
        _beam = TLorentzVector()
        _elec = TLorentzVector()
        _gamma = TLorentzVector()
        _target = TLorentzVector()
        _prot = TLorentzVector()
        _pip = TLorentzVector()
        _pim = TLorentzVector()
        _other = TLorentzVector()
        _neutron = TLorentzVector()

        _mc = False
        _hasE = False
        _hasP = False
        _hasPip = False
        _hasPim = False
        _hasOther = False
        _hasNeutron = False

        _numProt = 0
        _numPip = 0
        _numPim = 0
        _numPos = 0
        _numNeg = 0
        _numNeutral = 0
        _numOther = 0

        _sector = -1

        _MM = np.nan
        _MM2 = np.nan

        _W = np.nan
        _Q2 = np.nan
