# README for the Benchmarks simulations associated with the following review paper

Brette R, Rudolph M, Carnevale T, Hines M, Beeman D, Bower JM, Diesmann M, Morrison A, Goodman PH, Harris Jr. FC, Zirpe M, Natschlager T, Pecevski D, Ermentrout B, Djurfeldt M, Lansner A, Rochel O, Vieville T, Muller E, Davison AP, ElBoustani S and Destexhe A. Simulation of networks of spiking neurons: A review of tools and strategies. *Journal of Computational Neuroscience*, vol 23, p 349-398, 2007

## Abstract

We review different aspects of the simulation of spiking neural networks. We start by reviewing the different types of simulation strategies and algorithms that are currently implemented. We next review the precision of those simulation strategies, in particular in cases where plasticity depends on the exact timing of the spikes. We overview different simulators and simulation environments presently available (restricted to those freely available, open source and documented). For each simulation tool, its advantages and pitfalls are reviewed, with an aim to allow the reader to identify which simulator is appropriate for a given task. Finally, we provide a series of benchmark simulations of different types of networks of spiking neurons, including Hodgkin-Huxley type, integrate-and-fire models, interacting with current-based or conductance-based synapses, using clock-driven or event-driven integration strategies. The same set of models are implemented on the different simulators, and the codes are made available. The ultimate goal of this review is to provide a resource to facilitate identifying the appropriate integration strategy and simulation tool to use for a given modeling problem related to spiking neural networks.

The codes included in this package refer to the benchmark simulations described above. The main goal is to provide a series of benchmark simulations of networks of spiking neurons, and how these are implemented in the different simulators overviewed in the paper. See details in the enclosed file *Appendix2.pdf*, which describes these different benchmarks. Some of these benchmarks were based on the Vogels-Abbott model ([Vogels TP and Abbott LF. Signal propagation and logic gating in networks of integrate-and-fire neurons. *J. Neurosci.* 25: 10786-10795, 2005](http://www.jneurosci.org/cgi/content/full/25/46/10786)).

The submitted version of the paper is available at this URL:
[http://arxiv.org/abs/q-bio.NC/0611089](http://arxiv.org/abs/q-bio.NC/0611089)

These files updated October 15th, 2008 to include Brian and PyNN.

20110805 Ted Carnevale corrected the synaptic time constants for the NEURON implemetation of the coba model (see NEURON/coba/cobacell.hoc).

---

2025-05-27 – Standardized to Markdown