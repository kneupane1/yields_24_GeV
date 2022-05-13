package clas12Analysis;

import org.jlab.clas.physics.LorentzVector;

/**
 * PhysicsCalcs
 */
public class PhysicsCalcs {
    private static float energy = 7.5f;
    private static LorentzVector target = new LorentzVector(0, 0, 0, 0.93828f);
    private static LorentzVector beam = new LorentzVector(0, 0, energy, energy);
    private static LorentzVector gamma;

    PhysicsCalcs(float beam_e) {
        energy = beam_e;
        beam = new LorentzVector(0, 0, energy, energy);
    }

    public static float W_calc(LorentzVector elec) {
        gamma = new LorentzVector(beam);
        gamma.sub(elec);
        gamma.add(target);
        return (float) gamma.mass();
    }

    public static float MM_calc(LorentzVector elec, LorentzVector pip) {
        gamma = new LorentzVector(beam);
        gamma.sub(elec);
        gamma.add(target);
        gamma.sub(pip);
        return (float) gamma.mass();
    }

    public static float Q2_calc(LorentzVector elec) {
        gamma = new LorentzVector(beam);
        gamma.sub(elec);
        return (float) -gamma.mass2();
    }

    public static LorentzVector fourVec(float px, float py, float pz, float mass) {
        LorentzVector _vec = new LorentzVector();
        _vec.setPxPyPzM(px, py, pz, mass);
        return _vec;
    }

}