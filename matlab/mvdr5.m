[st,x]=system('cat ../out/signal_300hz_400hz_45_azimut_minus30.out');
x=str2num(x);

[st,z]=system('cat ../out/mutual.out');
z=str2num(z);

for i=1:8001
  y(i,:)=(z*x(i,:)')';
end

fc = 430e6;
c=physconst('LightSpeed');
lambda=c/fc;
space=0.2/lambda;

ha = phased.ULA('NumElements',9,'ElementSpacing', 0.2);
noise = 1e-7/sqrt(2)*(randn(size(x))+1i*randn(size(x)));

hMVDR = phased.MVDREstimator('SensorArray',ha,...
        'OperatingFrequency',fc,'ScanAngles',-90:90,...
                'DOAOutputPort',true,'NumSignals',2);
[~,ang] = step(hMVDR,x)
plotSpectrum(hMVDR);
[~,ang] = step(hMVDR,y)
plotSpectrum(hMVDR);
