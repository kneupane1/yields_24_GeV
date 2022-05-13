package clas12Analysis;

import org.jlab.jnp.hipo4.io.HipoReader;
import org.jlab.jnp.hipo4.data.Bank;
import org.jlab.jnp.hipo4.data.Event;
import org.jlab.jnp.hipo4.data.SchemaFactory;
import static clas12Analysis.constants.*;

/**
 * Datahandeler
 */
public class Datahandeler implements Runnable {
    private String Filename = "";
    private SchemaFactory factory;
    private Bank particles;
    private Bank ftParticle;
    private Bank scint;
    private Bank rec_event;
    private Bank rec_track;
    private Event event = new Event();
    private DeltaT dt;
    private static Reaction react;
    private Histograms hists;

    Datahandeler(String file_name, Histograms histograms) {
        Filename = file_name;
        hists = histograms;
    }

    @Override
    public void run() {
        HipoReader reader = new HipoReader();

        reader.open(Filename);
        factory = reader.getSchemaFactory();
        particles = new Bank(factory.getSchema("REC::Particle"));
        ftParticle = new Bank(factory.getSchema("RECFT::Particle"));
        scint = new Bank(factory.getSchema("REC::Scintillator"));
        rec_event = new Bank(factory.getSchema("REC::Event"));
        rec_track = new Bank(factory.getSchema("REC::Track"));

        while (reader.hasNext()) {
            reader.nextEvent(event);
            event.read(rec_event);
            event.read(rec_track);
            event.read(particles);
            event.read(ftParticle);
            event.read(scint);

            if (particles.getInt("pid", 0) != 11 && particles.getRows() == 0)
                continue;

            if (Math.abs(particles.getShort("status", 0)) <= 2000)
                continue;
            if (rec_track.getByte("sector", 0) == 0)
                continue;

            dt = new DeltaT(particles, scint);
            react = new Reaction(particles, rec_track.getByte("sector", 0));

            for (int ipart = 1; ipart < particles.getRows(); ipart++) {
                final float px = particles.getFloat("px", ipart);
                final float py = particles.getFloat("py", ipart);
                final float pz = particles.getFloat("pz", ipart);
                final float p = (float) Math.sqrt((px * px + py * py + pz * pz));

                if (particles.getByte("charge", ipart) == 1) {
                    hists.fill_dt(p, dt.getDeltaT(ipart, MASS_PIP));
                    if (Math.abs(dt.getDeltaT(ipart, MASS_PIP)) < 0.5) {
                        react.SetPip(ipart);
                    } else if (Math.abs(dt.getDeltaT(ipart, MASS_P)) < 0.5) {
                        react.SetProton(ipart);
                    }
                } else if (particles.getByte("charge", ipart) == -1) {
                    if (Math.abs(dt.getDeltaT(ipart, MASS_PIM)) < 0.5)
                        react.SetPim(ipart);
                } else {
                    react.SetOther(ipart);
                }
            }

            react.CalcMissMass();
            hists.fill_WvsQ2(react);
            hists.fill_MassPhotons(react);
            if (react.SinglePip()) {
                hists.fill_MM(react);
            }

        }
        // Done
    }
}