clc, clear
%spectrum = load('~/Workspace/K5/referenceSpectrum.txt');
%spectrum = getk5_n1024();
spectrum = load('207bi_46.5h.txt');
background = load('background_97h.txt');

intensity = spectrum(:,2);
intensity = intensity - (46.5/97)*background(:,2); %correct for background
index = spectrum(:,1);
%plot(index, intensity)

rangemax = [2800, 5650, 6150];
rangemin = [2650, 5500, 5900];
[peak1,~,~] = fitpeak(rangemin(1),rangemax(1),index,intensity);
[peak2,~,~] = fitpeak(rangemin(2),rangemax(2),index,intensity);
[peak3,~,~] = fitpeak(rangemin(3),rangemax(3),index,intensity);

% E1 = 1.1938; %[MeV]
% E2 = 1.3437; %[MeV]
% E3 = 3.0120; %[MeV]
E1 = 482; %[keV]
E2 = 976; %[keV]
E3 = 1048; %[keV]

sol = polyfit([peak1 peak2 peak3],[E1 E2 E3],1);
energy = sol(1)*index+sol(2);

plot(energy,intensity)
xlabel('E [keV]')
ylabel('count')
set(gca,'fontsize',16)

%save('207Bi.mat','energy','intensity')
save('fit.mat','energy','intensity')


%%
clc
%energy = load('energy.mat');
fit = load('fit.mat');

%spectrum = getk5_n1024();
spectrum = load('137cs_117h.txt');
intensity = spectrum(:,2);
intensity = intensity - (117/97)*background(:,2); %correct for background

semilogy(fit.energy,intensity)
xlabel('E [keV]')
ylabel('count')
set(gca,'fontsize',16)


%%
clc, close all

Z = -56;
Te = fit.energy;
data = spectrum;

kuriedata = kurieplot(Te,spectrum,Z);
[slope,offset] = kuriefit(Te,kuriedata,100:500);
%[slope,offset] = kuriefit(Te,kuriedata,700:1000);
set(gca,'fontsize',16)

%%
clc
Z = -56;
Q1 = 535.8405;
logf1 = logf(Z,Q1)

%clear

Z = -56;
Q2 = 1.0358e+03;
logf2 = logf(Z,Q2)

lambda1 = 0.9*log(2)/30; %[år⁻¹]
lambda2 = 0.1*log(2)/30;
lambda = lambda1 + lambda2;

t1 = log(2)/lambda1 %[år]
t2 = log(2)/lambda2 %[år]

logft1 = logf1 + log10(1.0512e9) %1.0512e9 halveringstid i sek
logft2 = logf2 + log10(9.461e9)


