package clas12Analysis;

import org.jlab.jnp.hipo4.data.Bank;

/**
 * DeltaT
 */
public class DeltaT {
    private static float c_special_units = 29.9792458f;
    private static Bank part;
    private static Bank scint;
    private static float _vertex;

    public DeltaT(Bank particles, Bank scintilators) {
        DeltaT.scint = scintilators;
        DeltaT.part = particles;
        DeltaT._vertex = vertex_time(scint.getFloat("time", 0), scint.getFloat("path", 0), 1);
    }

    private static float vertex_time(float sc_time, float sc_pathlength, float relatavistic_beta) {
        return sc_time - sc_pathlength / (relatavistic_beta * c_special_units);
    }

    private static float beta(int ipart, float mass) {
        final float px = part.getFloat("px", ipart);
        final float py = part.getFloat("py", ipart);
        final float pz = part.getFloat("pz", ipart);
        final float momentum = (float) Math.sqrt(px * px + py * py + pz * pz);
        return (float) (momentum / Math.sqrt(momentum * momentum + mass * mass));
    }

    public float getDeltaT(int ipart, float mass) {
        final float beta = beta(ipart, mass);
        for (int iscin = 0; iscin < scint.getRows(); iscin++) {
            if (scint.getShort("pindex", iscin) == ipart) {
                if (scint.getByte("detector", iscin) == 12) {
                    final float path = scint.getFloat("path", iscin);
                    final float time = scint.getFloat("time", iscin);
                    return (_vertex - vertex_time(time, path, beta));
                }
            }
        }
        return Float.NaN;
    }
}