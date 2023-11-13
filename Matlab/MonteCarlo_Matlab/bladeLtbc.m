%单个输入为随机变量

clear all;

% Nominal values of input parameters

hgas  = 3000;    % TBC-gas heat transfer coef. (W/(m^2 K))
Tgas  = 1500;    % Mixed gas temperature (K)
ktbc  = 1;       % TBC   thermal conduct. (W/mK)
km    = 20;      % Metal thermal conduct. (W/mK)
Lm    = 0.003;   % Metal thickness (m)
hcool = 1000;    % Coolant-metal heat transfer coef. (W/(m^2 K))
Tcool =  600;    % Coolant temperature (K)

% Number of Monte Carlo trials
N = 1000;
%Ltbc = zeros(N,1);
%Tmh  = zeros(N,1);

for n = 1:N,
  
  % generate Ltbc values using a uniform distribution
  Ltbc(n) = 0.00025 + 0.0005*rand;

  % Solve heat transfer problem
  [Ttbc, Tmh(n), Tmc, q] = blade1D(hgas, Tgas, ...
				   ktbc, Ltbc(n), ...
				   km, Lm, ...
				   hcool, Tcool);

    if (n>1)
          mTmh(n-1)=mean(Tmh);
          sTmh(n-1)=std(Tmh);
    end

end

figure(1);
hist(Ltbc,20);
xlabel('L_{tbc} (m)');
figure(2);
hist(Tmh,20);
xlabel('T_{mh} (K)');
figure(3);
plot(mTmh);
ylabel('Mean T_{mh} (K)');
%hold all;
figure(4);
plot(sTmh);
ylabel('Sdev T_{mh} (K)');
%hold all;
fprintf('Mean Tmh = %6.1f\n',mTmh(n-1));
fprintf('Sdev Tmh = %6.1f\n',sTmh(N-1));

