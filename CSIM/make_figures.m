%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Plot the response
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%
% Get the responses
%
voltage = csim('get', recVm,     'traces' );
spikes  = csim('get', recSpikes, 'traces' );

%
% Plot voltage trace
%

% search neuron with a suitable number of spikes
nSpikes = [];
for c = 1:length(spikes.channel)
  nSpikes(c) = length(spikes.channel(c).data);
end
[v, i ] = min( abs( nSpikes - 5 ));

% plot the membrane voltage of channel i
figure(1); clf;
plot( DTsim:DTsim:Tsim, voltage.channel(i).data*1000 );

% paste the spikes onto the plot
Vthresh = csim('get',voltage.channel(i).idx,'Vthresh')*1000;
hl=line( [ spikes.channel(i).data; spikes.channel(i).data ], ...
      [ Vthresh; Vthresh+25 ]*ones( 1, length(spikes.channel(i).data) ), ...
      'Color', [ 0, 0, 1 ] );

% set some options of the plot
ylabel('Vm [mV]');
xlabel('time [sec]');


%
% Plot the spike raster
%
figure(2); clf;
cla reset; hold on;
for c = 1:length(spikes.channel)
    st=spikes.channel(c).data;
    plot( st, c*ones(1,length(st)), 'k.' );
end
box on;
set(gca,'XLim', [0 Tsim],'YLim',[0 c]);
xlabel('time [sec]');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Analyse inter spike intervals (ISI)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% get all the spikes
spikes  = csim('get', recAllSpikes, 'traces' );

cv  = []; % list of coefficient of variatioins
r   = []; % list of firing rates
ISI = []; % list of all inter spike intervals
for i = 1:length(spikes.channel)
    isi = diff( spikes.channel(i).data );
    ISI = [ ISI isi ];
    if length(isi) > 1 
       cv = [ cv std(isi) / mean(isi) ];
       r  = [ r  length(spikes.channel(i).data) / Tsim ];
    end
end
fprintf( 'mean CV of ISI = %g\n', mean(cv) );

% show histograms
figure(3);
subplot(1,2,1);
hist( ISI, 50 );
xlabel('ISI');
ylabel('frequency')

subplot(1,2,2);
hist( cv, 50 );
xlabel('CV');
ylabel('frequency')
