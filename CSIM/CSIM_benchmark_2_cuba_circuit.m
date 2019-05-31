%================================================================================
%
%  CSIM implementation of a benchmark simulation described in the paper
%  "Simulation of networks of spiking neurons: A review of tools and strategies"
%  using the "Circuit Tools" available from www.lsm.tugraz.at.
%
%  Benchmark 2: Current-based (CUBA) IF network. This benchmark consists of a 
%               network of intefrate-and-fire neurons connected with 
%               current-based synapses.
%
%  The "Circuit Tools" and CSIM are freely available from www.lsm.tugraz.at
%
%  Authors: Dejan Pecevski, dejan@igi.tugraz.at
%           Thomas Natschlaeger, thomas.natschlaeger@scch.at
%
%  Date: April 2006
%
%================================================================================

close all; clear csim; 

% Global parameter values
ConnP           = 0.02;   % connectivity probability
Tsim            = 0.4;    % duration of the simulation [sec]
DTsim           = 0.1e-3; % simulation time step [sec]
Tinp            = 50e-3;  % length of the initial stimulus [sec]
nInputNeurons   = 10 ;    % number of neurons which provide initial input (for a time span of Tinp)
inpConnP        = 0.01 ;  % connectivity from input neurons to network neurons
inputFiringRate = 80;     % firing rate of the input neurons during the initial input

% initialize an empty neural microcircuit object
nmc = neural_microcircuit('dt_sim', DTsim);

% Add a pool of conductance based neurons to the circuit 
[nmc, pool] = add(nmc, 'pool', 'type', 'LifNeuron', ...
                  'size', [20 20 10], 'origin', [20 1 1], 'frac_EXC', 0.8, ...
                  'Neuron.Cm', 2e-10, 'Neuron.Rm', 1e8, ...
                  'Neuron.Vthresh',  -50e-3, 'Neuron.Vreset', -60e-3, 'Neuron.Trefract', 5e-3, ...
                  'Neuron.Vresting', -49e-3, 'Neuron.Vinit',  -60e-3, 'Neuron.Iinject', [0 0] ) ;

% Create the connections in the network

Erev_exc = 0 ;
Erev_inh = -80e-3 ;
Vmean    = -60e-3 ;
Winh     = (Erev_inh-Vmean)*4.5e-9;
Wexc     = (Erev_exc-Vmean)*0.27e-9;

[nmc, cn] = add( nmc, 'Conn', 'dest', pool, 'src', pool, 'type', ...                          % connect pool with itself
                 'StaticSpikingSynapse', 'lambda', Inf, 'C', ConnP * ones(1,4), ...           % connectivity does not depend on distance
                 'SH_W', 0, 'SH_delay', 0, 'rescale', 0, 'constW', 0, 'Synapse.delay', 0, ... % no synaptic heterogeneity (SH)
                 'Synapse([EE IE]).W', Wexc, 'Synapse([EE IE]).tau',  5e-3, ...               % excitatory synapses
                 'Synapse([EI II]).W', Winh, 'Synapse([EI II]).tau', 10e-3 );                 % inhibitory synapses

% Create the input neurons for the inital stimulation
[nmc, inp] = add(nmc, 'pool', 'origin', [1 nInputNeurons 1], 'size', [1 nInputNeurons 1], ...
                      'type', 'SpikingInputNeuron', 'frac_EXC', 1);

% Connect the input neurons to the network
[nmc, cinp] = add( nmc, 'Conn', 'src', inp, 'dest', pool, ...
                   'type', 'StaticSpikingSynapse', 'lambda', Inf, 'C', inpConnP*ones(1,4), ...
                   'SH_W', 0, 'SH_delay', 0, 'rescale', 0, 'constW', 0, ...
                   'Synapse.W', Wexc, 'Synapse.tau', 5e-3, 'Synapse.delay', 0);

% Create the stimulus
S = generate( constant_rate('nChannels', nInputNeurons, 'f', inputFiringRate, 'Tstim', Tinp) );

% Record the spikings of some random neurons
nmc = record(nmc, 'Volume', [20 1 1 ; 30 20 1 ], 'Field', 'spikes', 'dt', DTsim);

% Record also the membrane potential of two neurons
nmc = record(nmc, 'Volume', [30 10 5 ; 30 10 5], 'Field', 'Vm', 'dt', DTsim);
nmc = record(nmc, 'Volume', [25 15 5 ; 25 15 5], 'Field', 'Vm', 'dt', DTsim);

% Simulate the network
tic; fprintf('Running simulation: ');
reset(nmc);
R = simulate(nmc, Tsim, S);
fprintf('Done. %gsec CPU time for %gms simulation time\n', round(toc), Tsim*1000 );

% Finally make some plots
% note that plot_response is part of the circuit tools
plot_response(R); 
