base="data/multiData2"
rate_prefix="rate_"
dir_suffix="run_"
prefix="latt_"
density_pfx="density_"
flux_pfx="flux_"
lims="-0.05 0.05"
vlines="64 511"
xSize="512"
#xSizes=('256','384', '512', '640', '768')
ySize="512"
ylims="0 $ySize"
binWidth="1"
binLim="128"
binCountLim="0 125"
binNetCountLim="0 375"
binDiffCountLim="0 250"
binNetDiffCountLim="0 750"
intDist="64"
mobility="5.0"
RPSMobility="5.0"
#rps_mobility="2.5"
mobilities=('0.1' '2.5' '5.0' '10.0')
#RPSMobilities=('0.01' '0.1' '1.0' '2.5' '5.0' '10.0' '100' '1000') 
RPSMobilities=('20.0' '40.0' '60.0' '80.0') 
steps="4000"
interval="5"
#interval="10"
start_t="3990"
subdiv="16"
dpi="200"
fps="30"
author="micarn"
species=('a' 'b' 'c')
units="p r"
pad=('0' '2500' '5000')

output="specData.png"
normOut="normSpecData.png"
hwhmOut="HalfWidthHalfMax.png"

for k in {0..3}; do
    targ="${base}/${rate_prefix}${k}"
    #for n in {0..499}; do
    for n in {0..99}; do
        n0=$( expr 2 \* $n )
        n1=$( expr 2 \* $n + 1 )
        n2=$( expr 2 \* $n + 2 )

        #target="$targ/${dir_suffix}${n}"
        target0="$targ/${dir_suffix}${n0}"
        target1="$targ/${dir_suffix}${n1}"
        target2="$targ/${dir_suffix}${n2}"
        #rm $target/animation.mp4
        #mkdir -p -v $target
        mkdir -p -v $target0
        mkdir -p -v $target1
        mkdir -p -v $target2
        #                  targ     o t xSize  ySize  mob       rps_mob              intDist  bin_w     steps  interval  start_t  run
        #                  1        2 3 4      5      6         7                    8        9         10     11        12       13
        ./LatticeMLRPSTest $target0 0 1 $xSize $ySize $mobility ${RPSMobilities[$k]} $intDist $binWidth $steps $interval $start_t $n0 &
        ./LatticeMLRPSTest $target1 0 1 $xSize $ySize $mobility ${RPSMobilities[$k]} $intDist $binWidth $steps $interval $start_t $n1 &
        ./LatticeMLRPSTest $target2 0 1 $xSize $ySize $mobility ${RPSMobilities[$k]} $intDist $binWidth $steps $interval $start_t $n2 &
        wait
        #python3 videoConverter.py ${target} $prefix $start_t $interval $steps -v $vlines -o animation.mp4 -a $author -f $fps --dpi $dpi

        #rm $target/density_net_newer.mp4

        #density net
        #python3 densityCalculator.py a $target 'density_net_newer.mp4' p r 0 $ySize -c -1 -p $density_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --dpi $dpi -g
    
        #binned death count A
        #python3 densityCalculator.py a $target binned_death_a.mp4 p 0 $binLim -c 0 -p binned_death_counts_ -l $binCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g
    
        #binned death count net
        #python3 densityCalculator.py a $target 'binned_death_net.mp4' p r 0 $binLim -c -1 -p binned_death_counts_ -l $binNetCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

        #binned birth count A
        #python3 densityCalculator.py a $target binned_birth_a.mp4 p 0 $binLim -c 0 -p binned_birth_counts_ -l $binCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

        #binned birth count net
        #python3 densityCalculator.py a $target 'binned_birth_net.mp4' p r 0 $binLim -c -1 -p binned_birth_counts_ -l $binNetCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

        #binned diffusion count A
        #python3 densityCalculator.py a $target binned_diffusion_a.mp4 p 0 $binLim -c 0 -p binned_diffusion_counts_ -l $binDiffCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

        #binned diffusion count net
        #python3 densityCalculator.py a $target 'binned_diffusion_net.mp4' p r 0 $binLim -c -1 -p binned_diffusion_counts_ -l $binNetDiffCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g
    done
    python corrLen.py $targ $dir_suffix autoCorr_3995.csv avg_auto_corr.csv 500

    #python3 fourierAnalysis.py $targ temporalData.csv $output $dir_suffix 100 15 s ${pad[$n]} -v $vlines --dpi $dpi -a 10.0
    #python3 fourierAnalysis.py $targ temporalData.csv $normOut $dir_suffix 100 15 s ${pad[$n]} -v $vlines --dpi $dpi -a 10.0 --abs
    #python3 fourierAnalysis.py $targ temporalData.csv $normOut $dir_suffix 100 5 s ${pad[2]} -v $vlines --dpi $dpi -a 10.0 --abs -o -w -d "spectrograph.csv"
    #python3 fourierAnalysis.py $targ temporalData.csv $normOut $dir_suffix 100 5 s ${pad[2]} -v $vlines --dpi $dpi -a 10.0 --abs -o -w -d "hwhm.csv"
    #python3 fourierAnalysis.py $targ temporalData.csv $normOut $dir_suffix 100 5 s ${pad[2]} -v $vlines --dpi $dpi -a 10.0 --abs -o -w -d "spectrograph.csv"
    #python3 fourierAnalysis.py $targ temporalData.csv $hwhmOut $dir_suffix 100 5 h ${pad[2]} -v $vlines --dpi $dpi -a 10.0 --abs -o -w -d "hwhm.csv"
    #python3 fourierAnalysis.py $targ temporalData.csv freqAnalysisPadded.png $dir_suffix 100 50 f ${pad[2]} -p 0 --dpi $dpi --abs -o
    #python3 videoConverter.py ${targ} $prefix $start_t $interval $steps -v $vlines -o animation.mp4 -a $author -f $fps --dpi $dpi
    #python3 videoConverter.py ${targ}/${dir_suffix}0 $prefix $start_t $interval $steps -o animation.mp4 -a $author -f $fps --dpi $dpi
done

tar -zcvf $base.tar.gz $base/*/*

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

#for l in {0..3}; do
    #for n in {0..3}; do
        #targ="${base/x/$l}${n}"
        #for m in {0..49}; do
            #target="${targ}/${dir_suffix}${m}"
            #mkdir -p -v $target
            #                  targ    o t xSize  ySize  mob               rps_mob              intDist  bin_w     steps  interval  start_t  run
            #                  1       2 3 4      5      6                 7                    8        9         10     11        12       13
            #./LatticeMLRPSTest $target 0 1 $xSize $ySize ${mobilities[$l]} ${RPSMobilities[$n]} $intDist $binWidth $steps $interval $start_t $m
        #done
        #python3 fourierAnalysis.py $targ spectralData.csv $output $dir_suffix 100 50 -v $vlines --dpi $dpi
        #python3 fourierAnalysis.py $targ normSpectralData.csv $normOut $dir_suffix 100 50 -v $vlines --dpi $dpi
        #python3 videoConverter.py ${targ}/${dir_suffix}0 $prefix $start_t $interval $steps -v $vlines -o animation.mp4 -a $author -f $fps --dpi $dpi
    #done
#done

#cd data
#tar -zcf specTest9Renders.tar.gz specTest9

#for x in {0..14}; do
    #target="${base}/${dir_suffix}${x}"
    #mkdir -p -v $target
    #./LatticeMLRPSTest $target 0 1 $xSize $ySize $mobility $RPSMobility $intDist $binWidth $steps $interval $start_t -v $vlines
#done

#python3 fourierAnalysis.py $base $dir_suffix 15
#python3 videoConverter.py ${base}/${dir_suffix}0 $prefix $start_t $interval $steps -v $vlines -o animation.mp4 -a $author -f $fps --dpi $dpi

#for x in {0..3}; do
    #target="${base}${x}"
    #mkdir -p -v $target 
    #                  targ    o t xSize  ySize  mob       rps_mob              intDist  bin_w            steps  interval  start_t
    #                  1       2 3 4      5      6         7                    8        9                10     11        12
    #./LatticeMLRPSTest $target 0 1 $xSize $ySize $mobility ${RPSMobilities[$x]} $intDist $binWidth $steps $interval $start_t

    #python3 videoConverter.py $target $prefix $start_t $interval $steps -v $vlines -o animation.mp4 -a $author -f $fps --dpi $dpi
    #python3 videoConverter.py $target $prefix $start_t $interval $steps -o animation_frameless.mp4 -a $author -f $fps --dpi $dpi -F

    #flux A
    #python3 densityCalculator.py a $target flux_a.mp4 f 0 $ySize -c 0 -p $flux_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --dpi $dpi -g

    #flux net
    #python3 densityCalculator.py a $target flux_net.mp4 f 0 $ySize -c -1 -p $flux_pfx -l $lims -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --dpi $dpi -g

    #flux_binned A
    #python3 densityCalculator.py a $target binned_flux_a.mp4 f 0 $binLim -c 0 -p binned_flux_ -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #flux_binned net
    #python3 densityCalculator.py a $target binned_flux_net.mp4 f 0 $binLim -c -1 -p binned_flux_ -l $lims -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #binned density A
    #python3 densityCalculator.py a $target binned_density_a.mp4 p 0 $binLim -c 0 -p binned_density_ -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #binned density net
    #python3 densityCalculator.py a $target binned_density_net.mp4 p 0 $binLim -c -1 -p binned_density_ -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #density A
    #python3 densityCalculator.py a $target density_a.mp4 p 0 $ySize -c 0 -p $density_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --dpi $dpi -g

    #density net
    #python3 densityCalculator.py a $target 'density_net_newer.mp4' p r 0 $ySize -c -1 -p $density_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --dpi $dpi -g

    #binned death count A
    #python3 densityCalculator.py a $target binned_death_a.mp4 p 0 $binLim -c 0 -p binned_death_counts_ -l $binCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #binned death count net
    #python3 densityCalculator.py a $target binned_death_net.mp4 p 0 $binLim -c -1 -p binned_death_counts_ -l $binNetCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #binned birth count A
    #python3 densityCalculator.py a $target binned_birth_a.mp4 p 0 $binLim -c 0 -p binned_birth_counts_ -l $binCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #binned birth count net
    #python3 densityCalculator.py a $target binned_birth_net.mp4 p 0 $binLim -c -1 -p binned_birth_counts_ -l $binNetCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #binned diffusion count A
    #python3 densityCalculator.py a $target binned_diffusion_a.mp4 p 0 $binLim -c 0 -p binned_diffusion_counts_ -l $binDiffCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g

    #binned diffusion count net
    #python3 densityCalculator.py a $target binned_diffusion_net.mp4 p 0 $binLim -c -1 -p binned_diffusion_counts_ -l $binNetDiffCountLim -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps --binned -g
#done

#cd data
#tar -zcf plane_wave_renders_2_1.tar.gz planeWaveRenders/*/density_net_newer.mp4

#for mobility in "${mobilities[@]}"; do
#    for rps_mobility in "${RPSMobilities[@]}"; do
#        mob="${mobility/./_}"
#        rps_mob="${rps_mobility/./_}"
#        target="${base/x/$mob}${targ/y/$rps_mob}"
#
#        mkdir -p -v $target
#
#        #                  targ    o t xSize  ySize  mob       rps_mob       intDist  steps  interval  start_t
#        #                  1       2 3 4      5      6         7             8        9      10        11
#        ./LatticeMLRPSTest $target 0 1 $xSize $ySize $mobility $rps_mobility $intDist $steps $interval $start_t
#
#        python3 videoConverter.py $target $prefix $start_t $interval $steps -v $vlines -o animation.mp4 -a $author -f $fps
#
#        #flux A
#        python3 densityCalculator.py a $target flux_lims_test_a.mp4 f 0 $ySize -c 0 -p $flux_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps
#
#        #flux net
#        python3 densityCalculator.py a $target flux_lims_test_net.mp4 f 0 $ySize -c -1 -p $flux_pfx -l $lims -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps
#
#        #density A
#        python3 densityCalculator.py a $target density_a.mp4 p 0 $ySize -c 0 -p $density_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps
#
#        #density net
#        python3 densityCalculator.py a $target density_net.mp4 p 0 $ySize -c -1 -p $density_pfx -v $vlines -s $start_t -i $interval -S $steps -a $author -f $fps
#
#    done
#done
#
#cd data
#tar -zcvf currentTest2Renders.tar.gz currentTest2/*/*/*.mp4 currentTest2/*/*/metadata.txt
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
#
#        python3 densityCalculator.py cm $targ $intDist $curr_destination -c $s -t 1 -r $xSize -p $curr_prefix -s $start_t -i $interval -S $steps -a $author
#    done
#done
