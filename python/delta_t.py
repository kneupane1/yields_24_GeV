import numpy as np
c_special_units = 29.9792458


def vertex_time(sc_time, sc_pathlength, relativistic_beta):
    return (sc_time - sc_pathlength / (relativistic_beta * c_special_units))


def delta_t(electron_vertex_time, mass, momentum, sc_t, sc_r):
    relativistic_beta = 1.0 / (np.sqrt(1.0 + mass / momentum) *
                               (mass / momentum))
    return (electron_vertex_time - vertex_time(sc_t, sc_r, relativistic_beta))
