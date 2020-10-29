clc;    clear;  close
TempT = importdata('\\file00.chalmers.se\home\gusmol\.win\My Documents\Expfys 2 Termo\extrauppg_testT');
TempK = importdata('\\file00.chalmers.se\home\gusmol\.win\My Documents\Expfys 2 Termo\extrauppg_testK');

plot(TempT)
hold
plot(TempK)



