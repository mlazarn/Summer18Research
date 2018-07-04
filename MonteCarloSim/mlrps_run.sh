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


target="data/RPSDiffRateWaveTests/Testx"
prefix="MCStep_"
xSize="256"
ySize="512"
intDist="64"
mobility="5.0"
RPSMobilities=('0.1' '1.0' '5.0' '10.0') 
steps="5000"
interval="5"
start_t="5"
author="micarn"
species=('a' 'b' 'c')

for n in {0..3}; do
    targ=${target/%x/$n}
    ./LatticeMLRPSTest $targ 0 1 $xSize $ySize $mobility ${RPSMobilities[$n]} $intDist $steps $interval $start_t

    python3 videoConverter.py $targ $prefix $start_t $interval $steps -o animation.mp4 -a $author

    for s in {0..2}; do
        destination="--dest_movie density_${species[$s]}.mp4"
        python3 densityCalculator.py m $targ $intDist $destination -c $s -t 1 -r 256 -p $prefix -s $start_t -i $interval -S $steps -a $author
    done
done
