package clas12Analysis;

import java.util.Map;
import java.util.HashMap;

/**
 * constants
 */
public class constants {
    public static int PROTON = 2212;
    public static int NEUTRON = 2112;
    public static int PIP = 211;
    public static int PIM = -211;
    public static int PI0 = 111;
    public static int KP = 321;
    public static int KM = -321;
    public static int PHOTON = 22;
    public static int ELECTRON = 11;

    public static float MASS_P = 0.93827203f;
    public static float MASS_N = 0.93956556f;
    public static float MASS_E = 0.000511f;
    public static float MASS_PIP = 0.13957018f;
    public static float MASS_PIM = 0.13957018f;
    public static float MASS_PI0 = 0.1349766f;
    public static float MASS_KP = 0.493677f;
    public static float MASS_KM = 0.493677f;
    public static float MASS_G = 0.0f;

    public static final Map<Integer, Float> mass = new HashMap<Integer, Float>() {
        {
            put(PROTON, MASS_P);
            put(NEUTRON, MASS_N);
            put(PIP, MASS_PIP);
            put(PIM, MASS_PIM);
            put(PI0, MASS_PI0);
            put(KP, MASS_KP);
            put(KM, MASS_KM);
            put(ELECTRON, MASS_E);
            put(PHOTON, MASS_G);
        }
    };
}