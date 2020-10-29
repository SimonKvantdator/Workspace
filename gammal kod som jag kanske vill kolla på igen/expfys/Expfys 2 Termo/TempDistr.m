function T = TempDistr(kappa, a)    %T = T(t,x)
T1 = importdata('/home/simon/Workspace/Expfys 2 Termo/TESTdata1');      %time data for Tdata
%T2 = importdata('\\file00.chalmers.se\home\gusmol\.win\My Documents\Expfys 2 Termo\MATDATA\lordag_1206_data2');      %temperature data for cool end
T1 = smooth(T1);
T1 = T1-mean(T1(1:180));
%T2 = T2-19.97033;

f = T1;             %temperature at x=0
delay = 1;          %[s]
t = 1:length(T1)*delay;

l = .6 - 0.128;             %[m]
x = linspace(0,l, 100);

N = 20;
dfdt = gradient(f); %(assumes delay = 1)

T = 0;
for n = 1:N
    omega = pi.*(2.*n-1)./(2*l);
    A = kappa.*omega.^2 - a;
    B = 4.*(a.*f - dfdt)./((2.*n-1).*pi);
    %cn' 
   
    b = @(s) interp1(t,B,[s],'linear');    %calculates B in a point s in time
    
    fun = @(s,c) b(s) - A.*c;
    
    
    %cn' = B - A cn
    %[t(1) t(end)];
    [~,c] = ode45(fun, t', 0);
    
    Xhat = sin(omega.*x);
    
    T = T + c*Xhat;
    disp(num2str(norm(c)))
    
end
T = T + f;


end
