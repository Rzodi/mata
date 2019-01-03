param U_count, integer, >= 0;
param ENB_count, integer, >=0;
param wifi_count, integer, >=0;
param M, default 10**20;
param Omega, >= 0 integer, default 25; #ilość resource bloków per enb
 
set U, default {1..U_count};
set ENB, default {1..ENB_count};
set wifi, default {1..wifi_count};
 
/* Requirements */
param Alpha{u in U, e in ENB}, >= 0; # zmienna wartość w zależności od odległości od ENB im dalej ten ten współczynnik jest większy mówi ile potrzebujemy resource bloków żeby przesłać dane z prędkością X
param Beta{u in U, e in ENB} >= 0; # stała wartość resource bloków potrzebnych do wysłania czegoś w LTE per jedno enb per użytkownik
param Gamma{u in U, w in wifi} >= 0;  # procent 1 sekundy potrzebny do wysłania danych z prędkością X [Kb/s]
param Delta{u in U } >= 0;   # minimalne wymaganie użytkownika [Kb/s]
param Delta_2{u in U} >= 0; # maksymalne wymaganie użytkownika [Kb/s]
  
 
/* Decision variables */
var X{u in U, e in ENB} >= 0; #przepustowość po ENB dla użytkownika u
var Y{u in U, w in wifi} >= 0; #przepustowość po wifi dla użytkownika u
var Z{u in U, e in ENB}, binary; #zmienna binarna równa jeden gdy X[u,e] > 0
var Z_wifi{u in U, w in wifi}, binary; #zmienna binarna równa jeden gdy Y[u,e] > 0
 
/* Objective function 'z' */
maximize f: sum{u in U} (sum{e in ENB} (X[u,e])+ sum{w in wifi} (Y[u,w]));
 
/* Constraints */
 
s.t. c1{e in ENB} : sum{u in U} ((Alpha[u,e]*X[u,e])+(Beta[u,e]*Z[u,e])), <= Omega;
 
s.t. c2{w in wifi} : sum{u in U} (Gamma[u,w]*Y[u,w]), <= 1;
 
s.t. c3{u in U} : Delta_2[u], >= sum{e in ENB}(X[u,e]) + sum{w in wifi}(Y[u,w]), >= Delta[u];
 
s.t. c4{u in U, e in ENB}: X[u,e], <= Z[u,e]*M;
 
s.t. c5{u in U, w in wifi}: Y[u,w] , <= Z_wifi[u,w]*M;
 
s.t. c6{u in U, e in ENB, w in wifi}: Delta_2[u], >=  X[u,e], >= 0;
 
s.t. c7{u in U, e in ENB, w in wifi}: Delta_2[u], >=  Y[u,w], >= 0;
 
s.t. c8{u in U} : sum{e in ENB} (Z[u,e]),  <= 1;
 
s.t. c9{u in U} : sum{w in wifi} (Z_wifi[u,w]), <= 1;
 
s.t. c10{u in U, e in ENB : Alpha[u,e] > Omega} : X[u,e] = 0;
 
s.t. c11{u in U, w in wifi : Gamma[u,w] > 1} : Y[u,w] = 0;


/* Input data */
data;


param U_count := 1;
param ENB_count := 1;
param wifi_count := 1;
param Omega := 50;

param : Alpha :=
 1,1     50
;
param : Beta :=
 1,1     2
;
param : Gamma :=
 1,1     1
;
param : Delta  :=
 1     1
;
param : Delta_2  :=
 1     1
;
end;
