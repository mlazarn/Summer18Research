#base_dir="data/multi_test/type/"
#base="/density_runx"
#
#sizes=('256' '512' '1024')
#int_dists=('64' '128' '256')
#mobilities=('1.25' '2.5' '5.0' '10.0')
#
#steps="2000000000"
#interval="1000000"
#start_t="500000000"
#swap_t="1000000000"
#swap_interval="500000"
#
#author="micarn"
#species=('a' 'b' 'c')
#
#for mobility in "${mobilities[@]}"; do
    #for n in {0..2}; do
        #size=${sizes[$n]}
        #int_dist=${int_dists[$n]}
        #
        #prefix="S${size}_"
        #norm_dir="${base_dir/type/norm}${prefix}${base/%x/${mobility/./_}}"
        #flipped_dir="${base_dir/type/flipped}${prefix}${base/%x/${mobility/./_}}"
#
        #mkdir -p -v $norm_dir $flipped_dir
#
        ##echo $norm_dir 1 $size $mobility $int_dist $steps $interval $start_t $swap $swap_interval
                           ##1        2  3     4         5         6      7         8        9       10 
        #./LatticeMLRPSTest $norm_dir 1 $size $mobility $int_dist $steps $interval $start_t $swap_t $swap_interval
#
        #python3 videoConverter.py $norm_dir $prefix $start_t $interval $steps -w $swap_t -W $swap_interval -f 30 -a $author
#
        #for s in {0..2}; do
            #destination="--dest_movie movie_${species[$s]}.mp4"
            #python3 densityCalculator.py m $norm_dir $int_dist $destination -c $s -p $prefix -s $start_t -i $interval -S $steps -w $swap_t -I $swap_interval -f 30 -a $author
        #done
#
        #./LatticeMLRPSTest $flipped_dir 0 ${sizes[$n]} $mobility ${int_dists[$n]} $steps $interval $start_t $swap_t $swap_interval
#
        #python3 videoConverter.py $flipped_dir $prefix $start_t $interval $steps -w $swap_t -W $swap_interval -f 30 -a $author
    #done
#done

#target="data/animTest"
#prefix="latt_"
#density_pfx="density_"
#flux_pfx="flux_"
#xSize="256"
#ySize="512"
#intDist="64"
#mobility="5.0"
#RPSMobility="0.1"
#steps="2000"
#interval="5"
#start_t="1000"
#author="micarn"
#species=('a' 'b' 'c')

#mkdir -p -v $target
#./LatticeMLRPSTest $target 0 1 $xSize $ySize $mobility $RPSMobility $intDist $steps $interval $start_t

#python3 videoConverter.py $target $prefix $start_t $interval $steps -o animation.mp4 -a $author

#flux A
#python3 densityCalculator.py a $target flux_lims_test_a.mp4 f 0 $ySize -c 0 -p $flux_pfx -v 64 -s $start_t -i $interval -S $steps -a $author

#flux net
#python3 densityCalculator.py a $target flux_lims_test_net.mp4 f 0 $ySize -c -1 -p $flux_pfx -l -0.05 0.05 -v 32 64 96 -s $start_t -i $interval -S $steps -a $author

#density A
#python3 densityCalculator.py a $target density_a.mp4 p 0 $ySize -c 0 -p $density_pfx -s $start_t -i $interval -S $steps -a $author

#density net
#python3 densityCalculator.py a $target density_net.mp4 p 0 $ySize -c -1 -p $density_pfx -s $start_t -i $interval -S $steps -a $author

base="data/currentTest2/mob_x/"
targ="rps_mob_y"
prefix="latt_"
density_pfx="density_"
flux_pfx="flux_"
lims="-0.05 0.05"
vlines="64 96 128 160 192"
xSize="512"
ySize="768"
ylims="0 $ySize"
intDist="128"
mobilities=('1.25' '2.5' '5.0')
RPSMobilities=('0.1' '1.0' '5.0' '10.0') 
steps="10000"
interval="1"
start_t="5000"
fps="60"
author="micarn"
species=('a' 'b' 'c')

for mobility in "${mobilities[@]}"; do
    for rps_mobility in "${RPSMobilities[@]}"; do
        mob="${mobility/./_}"
        rps_mob="${rps_mobility/./_}"
        target="${base/x/$mob}${targ/y/$rps_mob}"

        mkdir -p -v $target

        #                  targ    o t xSize  ySize  mob       rps_mob       intDist  steps  interval  start_t
        #                  1       2 3 4      5      6         7             8        9      10        11
        ./LatticeMLRPSTest $target 0 1 $xSize $ySize $mobility $rps_mobility $intDist $steps $interval $start_t

        python3 videoConverter.py $target $prefix $start_t $interval $steps -v $vlines -o animation.mp4 -a $author -f $fps

        #flux A
        python3 densityCalculator.py a $target flux_lims_test_a.mp4 f 0 $ySize -c 0 -p $flux_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps

        #flux net
        python3 densityCalculator.py a $target flux_lims_test_net.mp4 f 0 $ySize -c -1 -p $flux_pfx -l $lims -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps

        #density A
        python3 densityCalculator.py a $target density_a.mp4 p 0 $ySize -c 0 -p $density_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps

        #density net
        python3 densityCalculator.py a $target density_net.mp4 p 0 $ySize -c -1 -p $density_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps

    done
done

cd data
tar -zcvf currentTest2Renders.tar.gz currentTest2/*/*/*.mp4 currentTest2/*/*/metadata.txt
#for n in {0..3}; do
#    targ=${target/%x/$n}
#    ./LatticeMLRPSTest $targ 0 1 $xSize $ySize $mobility ${RPSMobilities[$n]} $intDist $steps $interval $start_t
#
#    python3 videoConverter.py $targ $prefix $start_t $interval $steps -o animation.mp4 -a $author
#    python3 densityCalculator.py cm $targ $intDist net_current.mp4 -c -1 -t 1 -r $xSize -p $prefix -s $start_t -i $interval -S $steps -a $author
#    for s in {0..2}; do
#        destination="--dest_movie density_${species[$s]}.mp4"
#        curr_destination="--dest_movie current_${species[$s]}.mp4"
#        python3 densityCalculator.py m $targ $intDist $destination -c $s -t 1 -r $xSize -p $prefix -s $start_t -i $interval -S $steps -a $author

#        python3 densityCalculator.py cm $targ $intDist $curr_destination -c $s -t 1 -r $xSize -p $curr_prefix -s $start_t -i $interval -S $steps -a $author
#    done
#done
