package clas12Analysis;

import org.jlab.groot.ui.TCanvas;

import java.util.HashMap;
import java.util.Map;

import org.jlab.groot.base.GStyle;
import org.jlab.groot.data.H2F;
import org.jlab.groot.data.H1F;

/**
 * Histograms
 */
public class Histograms {
    private static TCanvas can = new TCanvas("DeltaT Proton", 600, 600);
    private static TCanvas can2 = new TCanvas("W vs Q^{2}", 1200, 1200);
    private static TCanvas can3 = new TCanvas("Missing Mass e(p,pi+)e' ", 1200, 1200);
    private static TCanvas can4 = new TCanvas("W sec", 1200, 1200);

    private static H2F dt_prot = new H2F("DeltaT Proton", 1000, 0, 4, 1000, -10, 10);
    private static H2F WvsQ2_cut = new H2F("WvsQ2_cut", 200, 0.5, 3.0, 200, 0, 5.0);
    private static H2F WvsQ2 = new H2F("WvsQ2", 200, 0.5, 3.0, 200, 0, 5.0);
    private static H2F WvsTheta = new H2F("WvsQ2", 200, 0.0, 2.0, 200, 0, 1.0);
    private static H1F W = new H1F("W", 200, 0.5, 3.0);
    private static Map<Integer, H1F> W_sec = new HashMap<Integer, H1F>();;
    private static H1F W_cut = new H1F("W_cut", 200, 0.5, 3.0);
    private static H1F MissingMass = new H1F("MissingMass", 200, 0.8, 1.3);
    private static H1F MissingMass_cut = new H1F("MissingMass", 200, 0.8, 1.3);
    private static H1F MassPhotons = new H1F("MassPhotons", 200, 0.0, 0.4);
    private static H1F MassPhotons_cut = new H1F("MassPhotons", 200, 0.0, 0.4);

    public Histograms() {
        GStyle.setPalette("kViridis");
        for (int sector = 0; sector < 6; sector++) {
            W_sec.put(sector, new H1F("W", 200, 0.5, 3.0));
        }

        can.getCanvas().initTimer(1000);
        dt_prot.setTitleX("Momentum (GeV)");
        dt_prot.setTitleY("#DeltaT (ns)");
        can.cd(0);
        can.draw(dt_prot);

        can2.divide(2, 2);
        can2.getCanvas().initTimer(1000);
        can2.cd(0);
        can2.draw(WvsQ2);
        can2.cd(1);
        can2.draw(W);
        can2.cd(2);
        can2.draw(WvsQ2_cut);
        can2.cd(3);
        can2.draw(W_cut);

        can3.divide(2, 2);
        can3.getCanvas().initTimer(1000);
        can3.cd(0);
        can3.draw(MissingMass);
        MissingMass_cut.setFillColor(46);
        can3.draw(MissingMass_cut, "same");
        can3.cd(1);
        can3.draw(MassPhotons);
        MassPhotons_cut.setFillColor(45);
        can3.draw(MassPhotons_cut, "same");
        can3.cd(2);
        can3.draw(WvsTheta);

        can4.divide(3, 2);
        can4.getCanvas().initTimer(1000);
        for (Map.Entry<Integer, H1F> sec : W_sec.entrySet()) {
            can4.cd(1 + sec.getKey());
            can4.draw(sec.getValue());
        }

    }

    public void fill_dt(float mom, float deltat) {
        dt_prot.fill(mom, deltat);
    }

    public void fill_WvsQ2(Reaction event) {
        WvsTheta.fill(event.W, event.theta_e);
        if (event.Elastic()) {
            W_sec.get(event.sector()).fill(event.W);
            W.fill(event.W, event.Q2);
            WvsQ2.fill(event.W, event.Q2);
        }
        if (event.SinglePip() && event.MM > 0.6 && event.MM < 1.1) {
            W_cut.fill(event.W);
            WvsQ2_cut.fill(event.W, event.Q2);
        }
    }

    public void fill_MM(Reaction event) {
        MissingMass.fill(event.MM);
        if (event.MM > 0.6 && event.MM < 1.1)
            MissingMass_cut.fill(event.MM);
    }

    public void fill_MassPhotons(Reaction event) {
        MassPhotons.fill(event.pi0_mass);
        if (event.MM > 0.6 && event.MM < 1.1)
            MassPhotons_cut.fill(event.pi0_mass);
    }

}