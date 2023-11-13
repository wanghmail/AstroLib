function [Ttbc, Tmh, Tmc, q] = blade1D(hgas, Tgas, ktbc, Ltbc, km, Lm, ...
				    hcool, Tcool)

% Calculate matrix
K = [    -hgas,          0,      0, -1; ...
     ktbc/Ltbc, -ktbc/Ltbc,      0, -1; ...
             0,      km/Lm, -km/Lm, -1; ...
             0,          0,  hcool, -1;];

% Calculate right-hand side
b = [-hgas*Tgas; 0; 0; hcool*Tcool];

u = K\b;

Ttbc = u(1);
Tmh  = u(2);
Tmc  = u(3);
q    = u(4);


