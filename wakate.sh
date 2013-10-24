#!/bin/bash

run_name=DLA1
workdir=$PWD
home_dir=$PWD
mcs_dir="MCS"
time="00:20:00"

#q="small1" #small ---> 12x16, large-----> 72x16
q="large" #small ---> 12x16, large-----> 72x16

#if [ $q = "large" ]; then
#node="72"
#else
node="6"
#fi

#MN1="08"
#MN2="09"

step_list="1"
MC_thermal_list="1000" #"400000 500000 600000 700000 800000 900000 1000000" #"10 100 1000 10000 50000 100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000" 
B_list="32.0" # 4.0 8.0"
Nx_list="16"
#Ny_list="128"
Nz_list="1"
Kb_list="0"
Kf_list="0"
Ubb_list="0.0"
Vb1_list="3.0"
Vb2_list="0.0"
Ubf_list="0.0"
Uff_list="0.0"
tb_list="1.0"
tf_list="1.0"
mu_list="3.0" #"-2.0 -1.0 0.0 1.0 2.0 3.0 4.0 5.0" #"-4.0 -3.0 -2.0 -1.0 0.0 1.0 2.0 3.0 4.0 5.0" #-0.6 -0.4 -0.2 0.0 0.2 0.4 0.6 0.8 1.0 1.2 1.4 1.6 1.8 2.0 2.2 2.4 2.6 2.8 3.0 3.2 3.4 3.6 3.8 4.0 4.2 4.4 4.6 4.8 5.0 5.2 5.4 5.6 5.8 6.0 6.2 6.4 6.6 6.8 7.0 7.2 7.4 7.6 7.8 8.0 8.2 8.4 8.6 8.8 9.0" # 10.2 10.4 10.6 10.8 11.0 11.2 11.4 11.6 11.8 12.0 12.2 12.4 12.6 12.8 13.0 13.2 13.4 13.6 13.8 14.0" # 14.2 14.4 14.6 14.8 15.0 15.2 15.4 15.6 15.8 16.0 16.2 16.4 16.6 16.8 17.0 17.2 17.4 17.6 17.8 18.0" # 18.2 18.4 18.6 18.8 19.0 19.2 19.4 19.6 19.8 20.0"
seed_list="13" #103 13 173 223 37 317 149 211 253 83"
V_list="0.0"
obs_list="oooxxoxoo" # winding number, density, energy, susceptibility, local density, the number of worms, density-density correlation, green function
D_list="0.0"
A_list="0.0"
E_list="1.0"
nmax_list="1"
Htr_list="0.1" #"0.5 1.0 2.0" #"0.02 0.04 0.06 0.08 0.1 0.25 1.5"
xdep="1" #0(mu),1(Htr),2(B),3(Nthermal), 4(Ntdiv), default(mu)


Ntdiv_list="16"
Nxdiv_list="1"
Nydiv_list="1"
Nzdiv_list="1"

Npara=$node

#machinepath=/home/masaki/machines/machinefile$MN1$MN2


for step in $step_list; do
for MC_thermal in $MC_thermal_list; do
for B in $B_list; do
for Nx in $Nx_list; do
#for Ny in $Ny_list; do
for Nz in $Nz_list; do
for Kb in $Kb_list; do
for Kf in $Kf_list; do
for Ubb in $Ubb_list; do
for Vb1 in $Vb1_list; do
for Vb2 in $Vb2_list; do
for Ubf in $Ubf_list; do
for Uff in $Uff_list; do
for tb in $tb_list; do
for tf in $tf_list; do
for mu in $mu_list; do
for seed in $seed_list; do
for V in $V_list; do
for obs in $obs_list; do
for D in $D_list; do
for A in $A_list; do
for E in $E_list; do
for nmax in $nmax_list; do
for Htr in $Htr_list; do
for Ntdiv in $Ntdiv_list; do
for Nxdiv in $Nxdiv_list; do
for Nydiv in $Nydiv_list; do
for Nzdiv in $Nzdiv_list; do

p_num=`echo $Npara*$Ntdiv*$Nxdiv*$Nydiv*$Nzdiv |bc` #=q*core

Ny=$Nx

if [ $Nz -gt "1" ]; then
d="3"
elif [ $Ny -gt "1" ]; then
d="2"
else
d="1"
fi


cur_dir=$PWD


if [ $xdep -eq "0" ]; then
target_para=$mu
target_dir=$workdir/data_mu/B${B}/Nx${Nx}_Ny${Ny}/V1_${Vb1}/Htr_${Htr}/MCS${MC_thermal}Nt${Ntdiv}Nx${Nxdiv}Ny${Nydiv}Nz${Nzdiv} #mu
elif [ $xdep -eq "1" ]; then
target_para=$Htr
target_dir=$workdir/data_Htr/B${B}/Nx${Nx}_Ny${Ny}/V1_${Vb1}/mu_${mu}/MCS${MC_thermal}Nt${Ntdiv}Nx${Nxdiv}Ny${Nydiv}Nz${Nzdiv}Q${q} #worm
elif [ $xdep -eq "2" ]; then
target_para=$B
target_dir=$workdir/data_B/Nx${Nx}_Ny${Ny}/V1_${Vb1}/mu_${mu}_Htr_${Htr}/MCS${MC_thermal}Nt${Ntdiv}Nx${Nxdiv}Ny${Nydiv}Nz${Nzdiv} #B
elif [ $xdep -eq "3" ]; then
target_para=$MC_thermal
target_dir=$workdir/data_Nthermal/B${B}/Nx${Nx}_Ny${Ny}/V1_${Vb1}/mu_${mu}_Htr_${Htr}Nt${Ntdiv}Nx${Nxdiv}Ny${Nydiv}Nz${Nzdiv} #Nthermal
elif [ $xdep -eq "4" ]; then
target_para=$Ntdiv
target_dir=$workdir/data_Ntdiv/B${B}/Nx${Nx}_Ny${Ny}/V1_${Vb1}/mu_${mu}_Htr_${Htr}/MCS${MC_thermal}Nx${Nxdiv}Ny${Nydiv}Nz${Nzdiv} #Nthermal
else
target_para=$mu
target_dir=$workdir/debug_mu/B${B}/Nx${Nx}_Ny${Ny}/V1_${Vb1}/Htr_${Htr}/MCS${MC_thermal}Nt${Ntdiv}Nx${Nxdiv}Ny${Nydiv}Nz${Nzdiv} #mu
fi

if [ ! -d $target_dir ]; then
mkdir -p $target_dir
fi

cp $run_name $target_dir/$run_name.$target_para

echo $step $MC_thermal $B $Nx $Ny $Nz $Kb $Kf $Ubb $Vb1 $Ubf $Uff $tb $tf $mu $seed $V $obs $D $A $E $nmax $d $Vb2 $xdep $Htr $Ntdiv $Nxdiv $Nydiv $Nzdiv > conf.$target_para
mv conf.$target_para $target_dir/.

cd  $target_dir

if [ ! -d $mcs_dir ]; then
mkdir -p $mcs_dir
fi


#time mpiexec -np ${p_num} ./$run_name.$target_para < conf.$target_para

ddir=$PWD
wdir=$PBS_O_WORKDIR

echo "#!/bin/sh" > a1_run$target_para.sh

echo "#----- pjsub option --------" >> a1_run$target_para.sh

echo '#PJM -L "rscgrp='$q'"' >> a1_run$target_para.sh
echo '#PJM -L "node='$node'"' >> a1_run$target_para.sh
echo '#PJM --mpi "proc='$p_num'"' >> a1_run$target_para.sh
echo '#PJM -L "elapse='$time'"' >> a1_run$target_para.sh
echo '#PJM -s' >> a1_run$target_para.sh

echo "#----- Program execution --------" >> a1_run$target_para.sh

echo 'PROF_HOME=/k/home/tcs/opt_sparc64/opt/FJSVfxlang/1.2.1' >> a1_run$target_para.sh
echo 'MPI_HOME=/opt/FJSVfxlang/1.2.0' >> a1_run$target_para.sh
echo 'TOFU_LIB=/opt/FJSVpxtof/sparc64fx/lib64' >> a1_run$target_para.sh
echo 'export PATH="${MPI_HOME}/bin:${PATH}"' >> a1_run$target_para.sh
echo 'export LD_LIBRARY_PATH=${PROF_HOME}/lib64/profex:${MPI_HOME}/lib64:${TOFU_LIB}:${LD_LIBRARY_PATH}' >> a1_run$target_para.sh
echo 'THREAD_STACK_SIZE=65536' >> a1_run$target_para.sh
echo 'FLIB_FASTOMP=TRUE' >> a1_run$target_para.sh

echo '#PJM -L "rscgrp='$q'"' >> a1_run$target_para.sh




echo 'cd ${PJM_O_WORKDIR:-.}' >> a1_run$target_para.sh

echo "fipp -C -Ihwm -d prof$target_para_$$ mpiexec -stdin conf.$target_para ./$run_name.$target_para" >> a1_run$target_para.sh

#echo "fapp -C -Impi -d prof$target_para mpiexec -stdin conf.$target_para ./$run_name.$target_para" >> a1_run$target_para.sh

pjsub a1_run$target_para.sh


cd $home_dir

#done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
done
