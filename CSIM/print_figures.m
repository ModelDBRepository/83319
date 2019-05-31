figure(1);
set(gcf,'PaperUnits','Centimeter');
set(gcf,'PaperPosition',[2 10 7.6 3]);

set(gcf,'Units','Centimeter');
set(gcf,'Position',[2 15 7.6 3]);

ylabel('');

set(gca,'Position',[0.1 0.4 0.85 0.55], 'FontSize', 8 );

if all( modelname(1:2) ~= 'hh' )
  set(gca,'YTick',[-70 -60 -50],'XLim', [0 Tsim], 'YLim',[-75 -25]);
  xlabel('');
else
  set(gca,'XLim', [0 Tsim], 'YLim',[-80 0],'YTick',-80:20:0);
  xlabel('time [sec]')
end

set( get(gca,'XLabel'),  'FontSize', 8 );
set( get(gca,'YLabel'),  'FontSize', 8 );

print('-deps2', sprintf('../fig/csim_%s_vm.eps', modelname ) );

pause(3);

figure(2);
set(gcf,'PaperUnits','Centimeter');
set(gcf,'PaperPosition',[2 10 7.6 3]);

set(gcf,'Units','Centimeter');
set(gcf,'Position',[10 15 7.6 3]);

if all( modelname(1:2) ~= 'hh' )
  xlabel('');
else
  xlabel('time [sec]')
end

set(gca,'Position',[0.1 0.4 0.85 0.55], 'FontSize', 8);
set( get(gca,'XLabel'),  'FontSize', 8 );
set( get(gca,'YLabel'),  'FontSize', 8 );

print('-deps2', sprintf('../fig/csim_%s_spikes.eps', modelname ) );
