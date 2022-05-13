#!/usr/bin/env python
from __future__ import print_function
import uproot
# Load matplotlib for plotting
import matplotlib.pyplot as plt
import sys
import numpy as np

from reaction import *
from physics import *
from delta_t import delta_t, vertex_time


clas12 = uproot.open(sys.argv[1])['clas12']

# Initialize empty array
mom = np.array([])
W = []
Q2 = []
beta = np.array([])

p_pos = []
beta_pos = []

p_dt = []
deltat_proton = []

pid, beta, px, py, pz, charge = clas12.arrays(
    ["REC_Particle_pid", "REC_Particle_beta",
     "REC_Particle_px", "REC_Particle_py",
     "REC_Particle_pz", "REC_Particle_charge"], outputtype=tuple)

# For every event that was loaded in
for pid_event, beta_event, px_event, py_event, pz_event, charge_event in zip(pid, beta, px, py, pz, charge):
    if(len(pid_event) > 0):
        for pidi, pxi, pyi, pzi, bi, qi in zip(pid_event, px_event, py_event, pz_event, beta_event, charge_event):
            #np.append(bi, beta)

            # Add the Momentum to the p array
            p2 = pxi**2 + pyi**2 + pzi**2
            p2 = np.abs(p2)
            np.append(mom, np.sqrt(p2))

            #[(beta, mom) for b, p in zip(bi, mom) if b.any() != 0.0]
            # if(pidi == 11):
            #    e_mu_p = fvec(px[x], py[x], pz[x], get_mass('ELECTRON'))
            #    Q2.append(Q2_calc(e_mu, e_mu_p))
            #    W.append(W_calc(e_mu, e_mu_p))
        # If the particle is positive
        # if charge[x] > 0:
        # p_pos.append(sqrt(p2[x]))
        # beta_pos.append(beta[x])

# Loop over length of REC_Scintillator
# for j in range(len(evnt.REC_Scintillator_pindex)):
# Get Electron vertex from first particle
#    electron_vertex = vertex_time(evnt.REC_Scintillator_time[0],
#                                  evnt.REC_Scintillator_path[0], 1.0)
# Get index for REC_Particle from REC_Scintillator
#    index = evnt.REC_Scintillator_pindex[j]

# Calculate momentum and fill array
#    p2 = evnt.REC_Particle_px[index]**2 + evnt.REC_Particle_py[index]**2 + evnt.REC_Particle_pz[index]**2
#    p2 = abs(p2)
# This array might be different from the p array so we are calculating and refilling it
#    p_dt.append(sqrt(p2))

# Calulating delta_t assuming the mass of a proton and append to the array
#    dt = delta_t(electron_vertex, MASS_P, sqrt(p2),
#                 evnt.REC_Scintillator_time[j],
#                 evnt.REC_Scintillator_path[j])
#    deltat_proton.append(dt)

# Momentum
output_file = "Momentum.pdf"
# Make the figure for plotting
fig = plt.figure(
    num=None, figsize=(16, 9), dpi=200, facecolor='w', edgecolor='k')
# Fill the histogram
plt.hist(mom, 500, normed=1, histtype='stepfilled', alpha=0.75, range=[0, 10])
# Add labels
plt.title("Electron Momentum")
plt.xlabel("P (GeV)")
plt.ylabel("Count")
# Save to output_file name
plt.savefig(output_file)

# W
output_file = "W.pdf"
# Make the figure for plotting
fig = plt.figure(
    num=None, figsize=(16, 9), dpi=200, facecolor='w', edgecolor='k')
# Fill the histogram
plt.hist(W, 500, normed=1, histtype='stepfilled', alpha=0.75, range=[0, 5])
# Add labels
plt.title("W")
plt.xlabel("W (GeV)")
plt.ylabel("Count")
# Save to output_file name
plt.savefig(output_file)

# Q2
output_file = "Q2.pdf"
# Make the figure for plotting
fig = plt.figure(
    num=None, figsize=(16, 9), dpi=200, facecolor='w', edgecolor='k')
# Fill the histogram
plt.hist(Q2, 500, normed=1, histtype='stepfilled', alpha=0.75, range=[0, 10])
# Add labels
plt.title("$Q^2$")
plt.xlabel("$Q^2$ ($GeV^2$)")
plt.ylabel("Count")
# Save to output_file name
plt.savefig(output_file)

# WvsQ2
output_file = "WvsQ2.pdf"
# Make the figure for plotting
fig = plt.figure(
    num=None, figsize=(16, 9), dpi=200, facecolor='w', edgecolor='k')
# Fill the histogram
plt.hist2d(W, Q2, bins=500, normed=True, range=[[0, 5], [0, 10]])
# Add labels
plt.title("W vs $Q^2$")
plt.xlabel("W (GeV)")
plt.ylabel("$Q^2$ ($Gev^2$)")
# Save to output_file name
plt.savefig(output_file)

# MomVsBeta
output_file = "MomVsBeta.pdf"
# Make the figure for plotting
fig = plt.figure(
    num=None, figsize=(16, 9), dpi=200, facecolor='w', edgecolor='k')
# Fill the histogram
plt.hist2d(mom, beta, bins=500, normed=True, range=[[0, 5], [0, 1.2]])
# Add labels
plt.title("W vs $Q^2$")
plt.xlabel("W (GeV)")
plt.ylabel("$Q^2$ ($Gev^2$)")
plt.colorbar()
# Save to output_file name
plt.savefig(output_file)
