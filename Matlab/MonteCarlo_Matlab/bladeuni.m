%多个输入为随机变量
clear all;

% Lower and upper bounds of input parameters
hgas  = [1500, 4500];    % TBC-gas heat transfer coef. (W/(m^2 K))
Tgas  = [1500, 2500];    % Mixed gas temperature (K)
ktbc  = [1.0, 1.3];      % TBC   thermal conduct. (W/mK)
Ltbc  = [0.00025, 0.00075];  % TBC   thickness (m)
km    = [20.0, 22.0];    % Metal thermal conduct. (W/mK)
Lm    = [0.003, 0.005];  % Metal thickness (m)
hcool = [1000, 2000];    % Coolant-metal heat transfer
                         % coef. (W/(m^2 K)
Tcool = [ 600, 800];     % Coolant temperature (K)

% Place parameter bounds into a vector

Pbound = [hgas; ...
	  Tgas; ...
	  ktbc; ...
	  Ltbc; ...
	  km; ...
	  Lm; ...
	  hcool; ...
	  Tcool];

% Number of Monte Carlo trials
N = 25000;
% for m=1:500
for n = 1:N,
  
  % generate Ltbc values using a uniform distribution
  P(:,n) = Pbound(:,1) + (Pbound(:,2)-Pbound(:,1)).*rand(8,1);

  % Solve heat transfer problem
  [Ttbc, Tmh(n), Tmc, q] = blade1D(P(1,n), P(2,n), P(3,n), P(4,n), ...
				   P(5,n), P(6,n), P(7,n), P(8,n));
               
  if (n>1)
      mTmh(n-1)=mean(Tmh);
      sTmh(n-1)=std(Tmh);
  end

end

%保存最终方差
% mall(m)=mTmh(n-1);
% sall(m)=sTmh(n-1);
% 
% end

figure(1);
hist(Tmh,20);
xlabel('T_{mh} (K)');
figure(2);
plot(mTmh);
ylabel('Mean T_{mh} (K)');
figure(3);
plot(sTmh);
ylabel('StdDev T_{mh} (K)');

% figure(4);
% hist(mall,20);
% xlabel('Mean T_{mh} (K)');
% figure(5);
% hist(sall,20);
% xlabel('Variance T_{mh} (K)');

fprintf('Mean Tmh = %6.1f\n',mTmh(n-1));
fprintf('Sdev Tmh = %6.1f\n',sTmh(N-1));

