base_ml="data/isolated_render/run_x"
base_rps="data/isolated_render/rps/run_x"

prefix="latt_"

size="256"
mobilities=('0.1' '1.25' '5.0' '10.0')

steps="2500"
interval="5"
start_t="1000"

author="micarn"

for n in {0..3}; do
    targ_ml="${base_ml/x/$n}"
    targ_rps="${base_rps/x/$n}"

    mkdir -p -v $targ_ml $targ_rps
    mobility="${mobilities[$n]}"

    ./RenderIsolated $targ_ml $targ_rps 0 $size $mobility $steps $interval $start_t

    python3 videoConverter.py $targ_ml $prefix $start_t $interval $steps -d 200 -o ani_ml.mp4
    python3 videoConverter.py $targ_ml $prefix $start_t $interval $steps -d 200 -o ani_ml_frameless.mp4 -F

    python3 videoConverter.py $targ_rps $prefix $start_t $interval $steps -d 200 -o ani_rps.mp4
    python3 videoConverter.py $targ_rps $prefix $start_t $interval $steps -d 200 -o ani_rps_frameless.mp4 -F
done
