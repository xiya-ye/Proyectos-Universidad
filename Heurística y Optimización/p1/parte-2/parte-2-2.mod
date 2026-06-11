############################################################
# SETS
############################################################
set TALLERES;
set AUTOBUSES;
set FRANJAS;

############################################################
# PARAMS
############################################################
param u := card(TALLERES);
param m := card(AUTOBUSES);
param n := card(FRANJAS);

param num_pasajeros_misma_franja{AUTOBUSES, AUTOBUSES};
param disponibilidad_franja{FRANJAS, TALLERES};

############################################################
# VARIABLES
############################################################
var asignado_bus_franja_taller{i in AUTOBUSES, s in FRANJAS, w in TALLERES}, binary;
var buses_misma_franja_distinto_taller{i in AUTOBUSES, j in AUTOBUSES,
        s in FRANJAS, w1 in TALLERES, w2 in TALLERES}, binary;

############################################################
# OBJECTIVE
############################################################
minimize Z:
    sum{i in AUTOBUSES, j in AUTOBUSES: i < j}
    sum{s in FRANJAS}
    sum{w1 in TALLERES, w2 in TALLERES: w1 != w2}
        num_pasajeros_misma_franja[i,j] *
        buses_misma_franja_distinto_taller[i,j,s,w1,w2];

############################################################
# CONSTRAINTS
############################################################
# Cada franja por cada taller debe ser ocupada por max un bus:
s.t. max_un_bus_por_franja_taller{s in FRANJAS, w in TALLERES}:
    sum{i in AUTOBUSES} asignado_bus_franja_taller[i,s,w] <= 1;

# No deben asignarse franjas que no esten disponibles:
s.t. restriccion_disponibilidad_franja{i in AUTOBUSES, s in FRANJAS, w in TALLERES}:
    asignado_bus_franja_taller[i,s,w] <= disponibilidad_franja[s,w];

# Cada autobús debe estar asignado a sólo una franja de algún taller:
s.t. bus_asignado_unica_franja{i in AUTOBUSES}:
    sum{s in FRANJAS, w in TALLERES} asignado_bus_franja_taller[i,s,w] = 1;

# Restriccion para linealizacion, para que Yijsw1w2 (segunda var de
# decision) se cumpla:

s.t. lin1{i in AUTOBUSES, j in AUTOBUSES, s in FRANJAS, w1 in TALLERES, w2 in TALLERES: i < j and w1 != w2}:
    buses_misma_franja_distinto_taller[i,j,s,w1,w2] <= asignado_bus_franja_taller[i,s,w1];

s.t. lin2{i in AUTOBUSES, j in AUTOBUSES, s in FRANJAS, w1 in TALLERES, w2 in TALLERES: i < j and w1 != w2}:
    buses_misma_franja_distinto_taller[i,j,s,w1,w2] <= asignado_bus_franja_taller[j,s,w2];

s.t. lin3{i in AUTOBUSES, j in AUTOBUSES, s in FRANJAS, w1 in TALLERES, w2 in TALLERES: i < j and w1 != w2}:
    buses_misma_franja_distinto_taller[i,j,s,w1,w2] >=
        asignado_bus_franja_taller[i,s,w1] + asignado_bus_franja_taller[j,s,w2] - 1;

############################################################
# SOLVE AND PRINT SOLUTION
############################################################
solve;

printf "Impacto_total = %f\n", Z;
for {i in AUTOBUSES, s in FRANJAS, w in TALLERES: asignado_bus_franja_taller[i,s,w] > 0.5} {
    printf "Bus %s -> Taller %s Franja %s\n", i, w, s;
}

end;