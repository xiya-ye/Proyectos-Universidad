############################################################
# SETS
############################################################
set AUTOBUSES;
set FRANJAS;

############################################################
# PARAMS
############################################################
param m:= card(AUTOBUSES);
param n:= card(FRANJAS);

param Distancia{i in AUTOBUSES};
param Pasajeros{i in AUTOBUSES};
param k_d;
param k_p;

############################################################
# VARIABLES
############################################################
var asignado_bus_franja{i in AUTOBUSES, s in FRANJAS}, binary;
var bus_no_asignado{i in AUTOBUSES}, binary;

############################################################
# OBJECTIVE
############################################################
minimize COST:
    sum{i in AUTOBUSES, s in FRANJAS} k_d * Distancia[i] *
        asignado_bus_franja[i, s]
    + sum{i in AUTOBUSES} k_p * Pasajeros[i] * bus_no_asignado[i];


############################################################
# CONSTRAINTS
############################################################
# Cada franja sólo puede ser ocupado como mucho un autobús:
s.t. unico_bus_por_franja{s in FRANJAS}:
    sum{i in AUTOBUSES} asignado_bus_franja[i, s] <= 1;

# Cada autobús se asigna a una franja o ninguna:
s.t. asignacion{i in AUTOBUSES}:
    sum{s in FRANJAS} asignado_bus_franja[i, s] + bus_no_asignado[i] = 1;


############################################################
# SOLVE AND PRINT SOLUTION
############################################################
solve;



printf {i in AUTOBUSES, s in FRANJAS: asignado_bus_franja[i, s] > 0.5}
    "Bus %s asignado a franja %s\n", i, s;

printf {i in AUTOBUSES: bus_no_asignado[i] > 0.5}
    "Bus %s sin asignar\n", i;

printf "Coste_real = %g\n", COST;

display COST;
end;
