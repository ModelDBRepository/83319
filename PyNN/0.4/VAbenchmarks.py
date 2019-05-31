# coding: utf-8
"""
This is implemention using PyNN of benchmarks 1 and 2 from the paper

Brette R, Rudolph M, Carnevale T, Hines M, Beeman D, Bower JM,
Diesmann M, Morrison A, Goodman PH, Harris Jr. FC, Zirpe M,
Natschlager T, Pecevski D, Ermentrout B, Djurfeldt M, Lansner A,
Rochel O, Vieville T, Muller E, Davison AP, El Boustani S and
Destexhe A.  Simulation of networks of spiking neurons: A review 
of tools and strategies.  Journal of Computational Neuroscience,
vol 23, p 349-398, 2007

The IF network is based on the CUBA and COBA models of Vogels & Abbott (J
Neurosci, 2005).  The model consists of a network of excitatory and
inhibitory neurons, connected via current-based "exponential"
synapses (instantaneous rise, exponential decay).

This implementation uses version 0.4 of the API.

"""

# check version
import pyNN
version = pyNN.__version__.split('.')[0:2]
required_version = ['0','4']
assert (version == required_version), \
       "This implementation is for version %s of the PyNN API, you have version %s" % (".".join(required_version),
                                                                                       ".".join(version))

import sys
import os
from math import *

if len(sys.argv) < 3:
    print "Usage: python VAbenchmarks.py <simulator> <benchmark>\n\n<simulator> is either neuron, nest1, nest2, pcsim or brian\n<benchmark> is either CUBA or COBA."
    sys.exit(1)
simulator = sys.argv[-2]
benchmark = sys.argv[-1]

exec("from pyNN.%s import *" % simulator)

from pyNN.random import NumpyRNG, RandomDistribution
timer = Timer()

# === Define parameters ========================================================

rngseed  = 98765

n        = 4000  # number of cells
r_ei     = 4.0   # number of excitatory cells:number of inhibitory cells
pconn    = 0.02  # connection probability
stim_dur = 50.   # (ms) duration of random stimulation
rate     = 100.  # (Hz) frequency of the random stimulation

dt       = 0.1   # (ms) simulation timestep
tstop    = 1000  # (ms) simulation duration
delay    = 0.1

# Cell parameters
area     = 20000. # (µm²)
tau_m    = 20.    # (ms)
cm       = 1.     # (µF/cm²)
g_leak   = 5e-5   # (S/cm²)
if benchmark == "COBA":
    E_leak   = -60.  # (mV)
elif benchmark == "CUBA":
    E_leak   = -49.  # (mV)
v_thresh = -50.   # (mV)
v_reset  = -60.   # (mV)
t_refrac = 5.     # (ms) (clamped at v_reset)
v_mean   = -60.   # (mV) 'mean' membrane potential, for calculating CUBA weights
tau_exc  = 5.     # (ms)
tau_inh  = 10.    # (ms)

# Synapse parameters
if benchmark == "COBA":
    Gexc = 4.     # (nS)
    Ginh = 51.    # (nS)
elif benchmark == "CUBA":
    Gexc = 0.27   # (nS) # Those weights should be similar to the COBA weights
    Ginh = 4.5    # (nS) # but the delpolarising drift must be taken into account
Erev_exc = 0.     # (mV)
Erev_inh = -80.   # (mV)


# === Calculate derived parameters =============================================

area  = area*1e-8                     # convert to cm²
cm    = cm*area*1000                  # convert to nF
Rm    = 1e-6/(g_leak*area)            # membrane resistance in MΩ
assert tau_m == cm*Rm                 # just to check
n_exc = int(round((n*r_ei/(1+r_ei)))) # number of excitatory cells   
n_inh = n - n_exc                     # number of inhibitory cells
if benchmark == "COBA":
    celltype = IF_cond_exp
    w_exc    = Gexc*1e-3              # We convert conductances to uS
    w_inh    = Ginh*1e-3
elif benchmark == "CUBA":
    celltype = IF_curr_exp
    w_exc = 1e-3*Gexc*(Erev_exc - v_mean) # (nA) weight of excitatory synapses
    w_inh = 1e-3*Ginh*(Erev_inh - v_mean) # (nA)
    if simulator == "brian":
        w_exc = w_exc*0.1
        w_inh = w_inh*0.1
    print w_exc, w_inh
    assert w_exc > 0; assert w_inh < 0

# === Build the network ========================================================

extra = {'threads': 1}

node_id = setup(timestep=dt, min_delay=delay, max_delay=delay, **extra)
np = num_processes()
import socket
host_name = socket.gethostname()
print "Host #%d is on %s" % (node_id+1, host_name)

if extra.has_key('threads'):
    print "%s Initialising the simulator with %d threads..." % (node_id, extra['threads'])
else:
    print "%s Initialising the simulator with single thread..." % (node_id)
    
# Small function to display information only on node 0
def nprint(s):
    if (node_id == 0):
        print s

cell_params = {
    'tau_m'      : tau_m,    'tau_syn_E'  : tau_exc,  'tau_syn_I'  : tau_inh,
    'v_rest'     : E_leak,   'v_reset'    : v_reset,  'v_thresh'   : v_thresh,
    'cm'         : cm,       'tau_refrac' : t_refrac}

if (benchmark == "COBA"):
    cell_params['e_rev_E'] = Erev_exc
    cell_params['e_rev_I'] = Erev_inh
    
timer.start()

print "%s Creating cell populations..." % node_id
exc_cells = Population((n_exc,), celltype, cell_params, "Excitatory_Cells")
inh_cells = Population((n_inh,), celltype, cell_params, "Inhibitory_Cells")
if benchmark == "COBA":
    ext_stim = Population((20,), SpikeSourcePoisson,{'rate' : rate, 'duration' : stim_dur},"expoisson")
    rconn = 0.01
    ext_conn = FixedProbabilityConnector(rconn, weights=0.1)

print "%s Initialising membrane potential to random values..." % node_id
rng = NumpyRNG(seed=rngseed, parallel_safe=True, rank=node_id, num_processes=np)
uniformDistr = RandomDistribution('uniform', [v_reset,v_thresh], rng=rng)
if simulator == "brian":
    uniformDistr = RandomDistribution('uniform', [v_reset*0.001,v_thresh*0.001], rng=rng)
exc_cells.randomInit(uniformDistr)
inh_cells.randomInit(uniformDistr)

print "%s Connecting populations..." % node_id
exc_conn = FixedProbabilityConnector(pconn, weights=w_exc, delays=delay)
inh_conn = FixedProbabilityConnector(pconn, weights=w_inh, delays=delay)

connections={}
connections['e2e'] = Projection(exc_cells, exc_cells, exc_conn, target='excitatory', rng=rng)
connections['e2i'] = Projection(exc_cells, inh_cells, exc_conn, target='excitatory', rng=rng)
connections['i2e'] = Projection(inh_cells, exc_cells, inh_conn, target='inhibitory', rng=rng)
connections['i2i'] = Projection(inh_cells, inh_cells, inh_conn, target='inhibitory', rng=rng)
if (benchmark == "COBA"):
    connections['ext2e'] = Projection(ext_stim, exc_cells, ext_conn, target='excitatory')
    connections['ext2i'] = Projection(ext_stim, inh_cells, ext_conn, target='excitatory')

# === Setup recording ==========================================================
print "%s Setting up recording..." % node_id
exc_cells.record()
inh_cells.record()
vrecord_list = [exc_cells[0],exc_cells[1]]
exc_cells.record_v(vrecord_list)

buildCPUTime = timer.elapsedTime()

# === Run simulation ===========================================================
print "%d Running simulation..." % node_id
timer.reset()

run(tstop)

simCPUTime = timer.elapsedTime()

E_rate = exc_cells.meanSpikeCount()*1000.0/tstop
I_rate = inh_cells.meanSpikeCount()*1000.0/tstop


# === Print results to file ====================================================

print "%d Writing data to file..." % node_id
timer.reset()

if not os.path.exists("Results"):
    os.mkdir("Results")

exc_cells.printSpikes("Results/VAbenchmark_%s_exc_%s_np%d.ras" % (benchmark, simulator, np))
inh_cells.printSpikes("Results/VAbenchmark_%s_inh_%s_np%d.ras" % (benchmark, simulator, np))
exc_cells.print_v("Results/VAbenchmark_%s_exc_%s_np%d.v" % (benchmark, simulator, np), compatible_output=True)
writeCPUTime = timer.elapsedTime()

syn_string = "%d e→e  %d e→i  %d i→e  %d i→i" % (len(connections['e2e']), len(connections['e2i']), len(connections['i2e']), len(connections['i2i']))

nprint("\n--- Vogels-Abbott Network Simulation ---")
nprint("Nodes                  : %d" % np)
nprint("Simulation type        : %s" % benchmark)
nprint("Number of Neurons      : %d" % n)
nprint("Number of Synapses     : %s" % syn_string)
nprint("Excitatory conductance : %g nS" % Gexc)
nprint("Inhibitory conductance : %g nS" % Ginh)
nprint("Excitatory rate        : %g Hz (may be wrong for MPI/threads mode)" % E_rate)
nprint("Inhibitory rate        : %g Hz (may be wrong for MPI/threads mode)" % I_rate)
nprint("Build time             : %g s" % buildCPUTime) 
nprint("Simulation time        : %g s" % simCPUTime)
nprint("Writing time           : %g s" % writeCPUTime)


# === Finished with simulator ==================================================

end()
