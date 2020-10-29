function [a]=getk5_n1024()
n=1024;

b=sscanf(urlread('http://subproxy1.fy.chalmers.se/k5/current.txt'),'%d',[2 8192])';

channelnr=linspace(0,n,n);
data=sum(reshape(b(:,2),8192/n,n));
a=[channelnr;data]';



