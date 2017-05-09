function HeatEqMexWrapper()

N=100;
x=linspace(0,1,N+1);
u0=1./(1e-2 + cos(x*5).^2);
%plotfcn=@(u,t) plot(x,u,'r');

HeatEqMex(u0,1e-4,100,'HeatEqMexPlotter');

end

