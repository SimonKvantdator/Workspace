clear all; clc; clf;
T1 = importdata('\\file00.chalmers.se\home\gusmol\.win\My Documents\Expfys 2 Termo\MATDATA\lordag_1206_data1');
T2 = importdata('\\file00.chalmers.se\home\gusmol\.win\My Documents\Expfys 2 Termo\MATDATA\lordag_1206_data2');

%constant
l=0.1013;
T=360; omega = 2*pi/T;
% smooth data
T1 = smooth(smooth(T1));
T2 = smooth(smooth(smooth(T2)));
% calc peaks and bottoms
[pks1, X1] = findpeaks(T1);
[pks2, X2] = findpeaks(T2);
[btm1, X1] = findpeaks(-T1);
btm1 = -btm1;
[btm2, X2] = findpeaks(-T2);
btm2 = -btm2;
%calc amplitude of last 20 periods
pks1= pks1(end-20+1:end);
pks2= pks2(end-20+1:end);
btm1= btm1(end-20+1:end);
btm2= btm2(end-20+1:end);
%making sure amplitudes are not ghost values
amp1=pks1-btm1;
amp1=amp1(abs(amp1) > 1);
amp2=pks2-btm2;
amp2=amp2(abs(amp2) > 1);
amp1=mean(amp1); % mean aplitude of pulse at point 1
amp2=mean(amp2); % mean aplitude of pulse at point 2
%calc velocity of heatwave
peakpos1= X1(end-20+1:end);
peakpos2= X2(end-20+1:end);
phasedifT=peakpos2-peakpos1;
v=l/mean(phasedifT)

%calc
q=amp1/amp2;
a=log(q);
b = l*omega/v;

kappa = pi*l^2/(a*b*T)
% simplified kappa = v*l/(log(q)*2)




hold on;
plot(T1)
plot(T2)


