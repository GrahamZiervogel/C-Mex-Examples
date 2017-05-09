function HeatEqMexPlotter(u,t)
% Plotting function for HeatEqMex.c

plot(linspace(0,1,length(u)),u,'r');
ylim([0,100]);                          %Free the y axis.
title(sprintf('t = %.4f',t));           %Display t in the plot title.

end