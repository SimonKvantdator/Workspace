function onlinek5(logscale)

if (nargin < 1)
	logscale = 0;
end
n=1024;

range = 1:n;

while(1)
	cur=getk5_rebin();
	if (logscale)
		semilogy(range,cur(range,2));

	else
		plot(range,cur(range,2));
	end
	title(datestr(now));
	drawnow;
	pause(1);
end

