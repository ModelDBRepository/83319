%================================================================================
%
%  CSIM implementation of a benchmark simulation described in the paper
%  "Simulation of networks of spiking neurons: A review of tools and strategies"
%
%  Benchmark 3: Conductance-based HH network. This benchmark consists of a 
%               network of HH point neurons connected by 
%               conductance-based synapses.
%
%  CSIM is freely available from www.lsm.tugraz.at/csim
%
%  Authors: Dejan Pecevski, dejan@igi.tugraz.at
%           Thomas Natschlaeger, thomas.natschlaeger@scch.at
%
%  Date: April 2006
%
%================================================================================

% bring Matlab into its initial state (note that this also clears CSIM)
close all
clear all
modelname = 'hh';

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Global parameter values
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

nNeurons        = 4000;   % number of neurons
ConnP           = 0.02;   % connectivity probability
Frac_EXC        = 0.8;    % fraction of excitatory neurons
Tsim            = 0.4;    % duration of the simulation [sec]
DTsim           = 0.1e-3; % simulation time step [sec]
nRecordNeurons  = 250;    % number of neurons to plot the spikes from
Tinp            = 50e-3;  % length of the initial stimulus [sec]
nInputNeurons   = 10 ;    % number of neurons which provide initial input (for a time span of Tinp)
inpConnP        = 0.01 ;  % connectivity from input neurons to network neurons
inputFiringRate = 80;     % firing rate of the input neurons during the initial input

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Create the neurons and set their parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

neuronIdx = csim('create', 'TraubsHHNeuron', nNeurons);

csim('get', neuronIdx(1) );

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Create synaptic connections
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tic; fprintf('Making synaptic connections: ');

% create connectivity matrix
c = rand( nNeurons, nNeurons ) < ConnP;

% set diagonal elements to zero to avoid self loops
c( find( eye(nNeurons) ) ) = 0;

% get to lists such that destIdx(i) and srcIdx(i) are the indices
% of the destination and source neuron of the i-th synapse respectively.
[dest_n src_n] = find( c );

destIdx = neuronIdx(dest_n);
srcIdx = neuronIdx(src_n);

% create synapse objects
nSyn = size(destIdx, 2);
synapses = csim('create', 'StaticSpikingCbSynapse', nSyn);

% connect the neurons via synapses
csim('connect', destIdx, srcIdx, synapses);

% Extract the inhibitory and excitatory synapse indices and set their parameters
% We assume that neurons with indices above Frac_EXC*nNeurons are inhibitory

Erev_exc = 0;
Erev_inh = -80e-3;
Vmean    = -60e-3;

excSynIdx  = synapses(find(src_n <  (Frac_EXC*nNeurons)));
csim('set', excSynIdx, 'W',  6e-9, 'E',      0, 'tau',  5e-3, 'delay', 0);

inhSynIdx  = synapses(find(src_n >= (Frac_EXC*nNeurons)));
csim('set', inhSynIdx, 'W', 67e-9, 'E', -80e-3, 'tau', 10e-3, 'delay', 0);

fprintf('Created %i conductance based synapses in %g seconds\n', nSyn, toc );

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Create input neurons for the initial stimulus
% and connect them to random neurons in circuit
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% create input neurons
InpNeuronIdx = csim('create', 'SpikingInputNeuron', nInputNeurons);

% randomly select source and destination neurons
[src_n dest_n] = find( rand(nInputNeurons,nNeurons) < inpConnP );
destCircIdx = neuronIdx(dest_n);
srcInputIdx = InpNeuronIdx(src_n);

% create input synapses
nInputSyn = size(destCircIdx, 2);
inSynapsesIdx = csim('create', 'StaticSpikingCbSynapse', nInputSyn);
csim('set', inSynapsesIdx, 'W', 6e-9, 'E', 0, 'tau', 5e-3, 'delay', 0);

% connect input neurons to random neurons in circuit
csim('connect', destCircIdx, srcInputIdx, inSynapsesIdx);

% create spike trains for the input neurons
for i=1:nInputNeurons
  in_channels(i).data    = sort( rand(1,inputFiringRate*Tinp) * Tinp );
  in_channels(i).idx     = InpNeuronIdx(i);
  in_channels(i).spiking = 1;
  in_channels(i).dt      = -1;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Create recorders to record spikes and voltage traces
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% create separate recorders for spikes and voltages
% Note that in principle this is not necessary but it is
% for convenience.
recVm        = csim('create', 'Recorder');
recSpikes    = csim('create', 'Recorder');
recAllSpikes = csim('create', 'Recorder');

% set the recording time step for the voltages equal to 
% the simulation time step
csim('set', recVm, 'dt', DTsim);

% randomly select nRecordNeurons neurons to record from
rp = randperm(nNeurons);
recNeuronIdx = neuronIdx( rp(1:nRecordNeurons) );

% we record the membrane voltages of some selected neurons
csim('connect', recVm, recNeuronIdx, 'Vm');

% we record the spikes of selected neurons
csim('connect', recSpikes, recNeuronIdx, 'spikes');

% we record the spikes of all neurons
csim('connect', recAllSpikes, neuronIdx, 'spikes');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Simulate the circuit
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tic; fprintf('Running simulation: ');

% set time step of the simulation
csim('set','dt', DTsim);

% first set t = 0
csim('reset');

% run simulation for Tsim seconds
csim('simulate', Tsim, in_channels);

fprintf('Done. %gsec CPU time for %gms simulation time\n', round(toc), Tsim*1000 );

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Make some figures out of simulation results
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

make_figures
