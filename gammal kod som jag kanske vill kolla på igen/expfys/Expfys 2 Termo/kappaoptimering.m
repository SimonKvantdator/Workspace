clc;    clear;  close
T1 = importdata('/home/simon/Workspace/Expfys 2 Termo/TESTdata1');      %time data for Tdata
T2 = importdata('/home/simon/Workspace/Expfys 2 Termo/TESTdata2');      %time data for Tdata
T1 = T1-mean(T1(1:180));   %setting room temperature as 0
T2 = T2-mean(T2(1:180));
%kappa = 300;    %[W/mK]
kappaGuess = 6.7463e-05;  %calculation time seems to scale with kappa
aGuess = -0.0014;
t = 1:length(T2);

%fun = @TempError;
%sol = fminsearch(fun ,  [kappaGuess aGuess])
%kappa = sol(1);
%a = sol(2);

T = TempDistr(kappaGuess,aGuess);
surf(T, 'Edgecolor', 'none')
xlabel('distance from heating point')
ylabel('time')


fun = @CumulativeTemperatureError;
opts = optimset();
sol = fminsearch(fun, [kappaGuess aGuess], opts);
kappa = sol(1)
a = sol(2)

%CumulativeTemperatureError(kappaGuess,aGuess)

function E = CumulativeTemperatureError(params)
kappa = params(1)
a = params(2)
T2 = importdata('/home/simon/Workspace/Expfys 2 Termo/TESTdata2');      %time data for Tdata
T2 = smooth(T2);
T2 = T2-mean(T2(1:180));
t = 1:length(T2);


T = TempDistr(kappa,a);
index = round(length(T(1,:))*((23-12.8)/60));
T0 = T(:,index);
TError = abs(T0 - T2);
E = trapz(t,TError)

end