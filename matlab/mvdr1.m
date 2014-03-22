clearvars;
index = 1;

for phi=-85:5:85

  command=strcat('cat ../out/signal_', num2str(phi));
  command=strcat(command, '.out');

  clearvars x y;

  [st,x]=system(command);
  x=str2num(x);

  if size(x) == 0
    continue
  end

  [st,z]=system('cat ../out/mutual.out');
  z=str2num(z);

  for i=1:size(x)
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
                'DOAOutputPort',true,'NumSignals',1);

  az(1, index) = phi;
  [~,doas1] = step(hMVDR,x);
  az(2, index) = doas1;
  [~,doas2] = step(hMVDR,y);
  az(3, index) = doas2;

  phiv(index) = phi;
  err(1, index) = az(2, index) - az(1, index);
  err(2, index) = az(3, index) - az(1, index);

  az
  err

  index = index + 1;
end

plot (phiv, err(1,:), phiv, err(2,:), '--');
title('MVDR DOA error vs. azimuth');
xlabel('Azimuth (degree)');
ylabel('DOA error (degree)');
legend('Uncorrected DOA', 'Corrected DOA');
grid on
