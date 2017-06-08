%% OU4 experimentell komplexitet analys
close all
clear all

% nr of elements
n = [2500 5000 10000 15000 20000];

%% Time for Insert item speed
T1_M = [1 2 4  12 9]; %MTFtable
T1_A = [128 492 1778 4038 6943]; %Arraytable
T1_D = [1 2 3 9 12]*10^-3; %Dlisttable

%% Time for Lookups with non-existens keys speed
T2_M = [194 606 2156 4691 8340];
T2_A = [282 918 3566 8068 14532];
T2_D = [187 629 2064 4660 8770];

%% Time for Random lookups speed
T3_M = [112 303 1299 2968 5612];
T3_A = [140 471 1764 4015 7231];
T3_D = [75 277 1001 2262 4233];

%% Time for Skewed lookuos speed 
T4_M = [64 187 809 1925 3895];
T4_A = [126 445 1817 3993 7129];
T4_D = [62 261 968 2228 4147];

%% Time for remove all items speed
T5_M = [125 348 1468 3600 7785];
T5_A = [62 221 891 2032 3628];
T5_D = [66 286 1062 2500 4760];

%% Plot: Experimental komplexitet MTFtable 
figure
plot(n,T1_M./n,'*')
hold on

plot(n,T2_M./n,'*')
hold on

plot(n,T3_M./n,'*')
hold on

plot(n,T4_M./n,'*')
hold on

plot(n,T5_M./n,'*')
xlabel('n')
ylabel('T(n) [ms]')
legend('Insert item','Lookups with non-exsistents keys',...
    'Random lookups','Skewed lookups','Remove all items','Location','northwest')

%% Plot: Experimental komplexitet Arraytable 
figure
plot(n,T1_A./n,'*')
hold on

plot(n,T2_A./n,'*')
hold on

plot(n,T3_A./n,'*')
hold on

plot(n,T4_A./n,'*')
hold on

plot(n,T5_A./n,'*')
xlabel('n')
ylabel('T(n) [ms]')
legend('Insert item','Lookups with non-exsistents keys',...
    'Random lookups','Skewed lookups','Remove all items','Location','northwest')

%% Plot: Experimental komplexitet Arraytable 
figure
plot(n,T1_D./n,'*')
hold on

plot(n,T2_D./n,'*')
hold on

plot(n,T3_D./n,'*')
hold on

plot(n,T4_D./n,'*')
hold on

plot(n,T5_D./n,'*')
xlabel('n')
ylabel('T(n) [ms]')
legend('Insert item','Lookups with non-exsistents keys',...
    'Random lookups','Skewed lookups','Remove all items','Location','northwest')

%% !!! Inte delat med n !!! Plot: Experimental komplexitet MTFtable 
figure
plot(n,T1_M,'*')
hold on

plot(n,T2_M,'*')
hold on

plot(n,T3_M,'*')
hold on

plot(n,T4_M,'*')
hold on

plot(n,T5_M,'*')
xlabel('n')
ylabel('T(n) [ms]')
legend('Insert item','Lookups with non-exsistents keys',...
    'Random lookups','Skewed lookups','Remove all items','Location','northwest')

%% Plot: Experimental komplexitet Arraytable 
figure
plot(n,T1_A,'*')
hold on

plot(n,T2_A,'*')
hold on

plot(n,T3_A,'*')
hold on

plot(n,T4_A,'*')
hold on

plot(n,T5_A,'*')
xlabel('n')
ylabel('T(n) [ms]')
legend('Insert item','Lookups with non-exsistents keys',...
    'Random lookups','Skewed lookups','Remove all items','Location','northwest')

%% Plot: Experimental komplexitet Arraytable 
figure
plot(n,T1_D,'*')
hold on

plot(n,T2_D,'*')
hold on

plot(n,T3_D,'*')
hold on

plot(n,T4_D,'*')
hold on

plot(n,T5_D,'*')
xlabel('n')
ylabel('T(n) [ms]')
legend('Insert item','Lookups with non-exsistents keys',...
    'Random lookups','Skewed lookups','Remove all items','Location','northwest')
