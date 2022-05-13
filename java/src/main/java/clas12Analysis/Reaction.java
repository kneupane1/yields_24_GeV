package clas12Analysis;

import static clas12Analysis.PhysicsCalcs.*;
import static clas12Analysis.constants.*;
import org.jlab.jnp.hipo4.data.Bank;
import org.jlab.clas.physics.LorentzVector;
import java.util.ArrayList;
import java.util.List;

/**
 * Reaction
 */
public class Reaction {
    private static Bank particles;
    private static float _beam_energy = 7.5f;

    private static LorentzVector _elec;

    private static LorentzVector _beam = fourVec(0.0f, 0.0f, _beam_energy, MASS_E);
    private static LorentzVector _target = fourVec(0.0f, 0.0f, 0.0f, MASS_P);

    private static LorentzVector _prot;
    private static LorentzVector _pip;
    private static LorentzVector _pim;
    private static LorentzVector _neutron;
    private static List<LorentzVector> _photons;

    // double par[6][16];

    private boolean _hasE = false;
    private boolean _hasP = false;
    private boolean _hasPip = false;
    private boolean _hasPim = false;
    private boolean _hasOther = false;
    private boolean _hasNeutron = false;

    private boolean _boosted = false;

    private short _numProt = 0;
    private short _numPip = 0;
    private short _numPim = 0;
    private short _numPos = 0;
    private short _numNeg = 0;
    private short _numNeutral = 0;
    private short _numPhotons = 0;
    private short _numOther = 0;

    private byte _sector = -1;

    private boolean _MM_calc = false;
    public float MM = Float.NaN;
    public float MM2 = Float.NaN;

    public float pi0_mass = Float.NaN;
    public float pi0_mass2 = Float.NaN;

    public float W = Float.NaN;
    public float Q2 = Float.NaN;
    public float xb = Float.NaN;

    public float theta_e = Float.NaN;
    private float _theta_star = Float.NaN;
    private float _phi_star = Float.NaN;

    Reaction(Bank particle, Byte sector) {
        _sector = sector;
        Reaction.particles = particle;
        Reaction._photons = new ArrayList<LorentzVector>();
        _hasE = true;
        Reaction._elec = fourVec(particles.getFloat("px", 0), particles.getFloat("py", 0), particles.getFloat("pz", 0),
                MASS_E);
        Q2 = Q2_calc(_elec);
        W = W_calc(_elec);
        theta_e = (float) _elec.theta();
    }

    public int sector() {
        return _sector - 1;
    }

    public void SetProton(int i) {
        _numProt++;
        _numPos++;
        _hasP = true;
        _prot = fourVec(particles.getFloat("px", i), particles.getFloat("py", i), particles.getFloat("pz", i), MASS_P);
    }

    public void SetPip(int i) {
        _numPip++;
        _numPos++;
        _hasPip = true;
        _pip = fourVec(particles.getFloat("px", i), particles.getFloat("py", i), particles.getFloat("pz", i), MASS_PIP);
    }

    public void SetPim(int i) {
        _numPim++;
        _numNeg++;
        _hasPim = true;
        _pim = fourVec(particles.getFloat("px", i), particles.getFloat("py", i), particles.getFloat("pz", i), MASS_PIM);
    }

    public void SetNeutron(int i) {
        _numNeutral++;
        _hasNeutron = true;
        _neutron = fourVec(particles.getFloat("px", i), particles.getFloat("py", i), particles.getFloat("pz", i),
                MASS_N);
    }

    public void SetOther(int i) {
        if (particles.getInt("pid", i) == NEUTRON)
            SetNeutron(i);
        else if (particles.getInt("pid", i) == PHOTON) {
            _photons.add(fourVec(particles.getFloat("px", i), particles.getFloat("py", i), particles.getFloat("pz", i),
                    MASS_G));
            _numPhotons++;
        } else {
            _numOther++;
            _hasOther = true;
        }
    }

    public void CalcMissMass() {
        LorentzVector mm = new LorentzVector(0, 0, 0, 0);
        mm.add(_beam);
        mm.sub(_elec);
        mm.add(_target);
        if (SinglePip() || NeutronPip()) {
            mm.sub(_pip);
            MM = (float) mm.mass();
            MM2 = (float) mm.mass2();
        } else if (TwoPion()) {
            mm.sub(_pip);
            mm.sub(_pim);
            MM = (float) mm.mass();
            MM2 = (float) mm.mass2();
        } else if (ProtonPim()) {
            mm.sub(_prot);
            mm.sub(_pim);
            MM = (float) mm.mass();
            MM2 = (float) mm.mass2();
        } else if (SingleP()) {
            mm.sub(_prot);
            MM = (float) mm.mass();
            MM2 = (float) mm.mass2();
        }

        LorentzVector pi0 = new LorentzVector(0, 0, 0, 0);
        if (_numPhotons >= 2) {
            for (LorentzVector p : _photons) {
                pi0.add(p);
            }
            pi0_mass = (float) pi0.mass();
            pi0_mass2 = (float) pi0.mass2();
        }

    }

    public boolean TwoPion() {
        return ((_numPip == 1 && _numPim == 1)
                && (_hasE && !_hasP && _hasPip && _hasPim && !_hasNeutron && !_hasOther));
    }

    public boolean ProtonPim() {
        return ((_numProt == 1 && _numPim == 1)
                && (_hasE && _hasP && !_hasPip && _hasPim && !_hasNeutron && !_hasOther));
    }

    public boolean SinglePip() {
        return ((_numPip == 1) && (_hasE && !_hasP && _hasPip && !_hasPim && !_hasNeutron && !_hasOther));
    }

    public boolean SingleP() {
        return ((_numProt == 1) && (_hasE && _hasP && !_hasPip && !_hasPim && !_hasNeutron && !_hasOther));
    }

    public boolean Elastic() {
        return (_hasE && !_hasP && !_hasPip && !_hasPim && !_hasNeutron && !_hasOther);
    }

    public boolean PPi0() {
        return (SingleP() && ((MM >= 0.05 && MM <= 0.3) || (pi0_mass >= 0.05 && pi0_mass <= 0.2)));
    }

    public boolean NeutronPip() {
        return ((_numPip == 1 && _numNeutral == 1)
                && (_hasE && !_hasP && _hasPip && !_hasPim && _hasNeutron && !_hasOther));
    }

}