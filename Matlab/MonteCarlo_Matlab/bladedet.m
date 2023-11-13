% Nominal values of input parameters

hgas  = 3000;    % TBC-gas heat transfer coef. (W/(m^2 K))
Tgas  = 1500;    % Mixed gas temperature (K)
ktbc  = 1;       % TBC   thermal conduct. (W/mK)
Ltbc  = 0.0005;  % TBC   thickness (m)
km    = 20;      % Metal thermal conduct. (W/mK)
Lm    = 0.003;   % Metal thickness (m)
hcool = 1000;    % Coolant-metal heat transfer coef. (W/(m^2 K))
Tcool =  600;    % Coolant temperature (K)

% Solve heat transfer problem

[Ttbc, Tmh, Tmc, q] = blade1D(hgas, Tgas, ...
			      ktbc, Ltbc, ...
			      km, Lm, ...
			      hcool, Tcool);

fprintf('Tmh = %6.1f K\n',Tmh);