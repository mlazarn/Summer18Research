base="data/densityTest/test_x"
size="512"
mobilities=('0.1' '1.25' '2.5' '10.0')
steps="3000"
interval="1"
start="0"

for i in {0..3}; do
    targ="${base/x/$i}"
    mkdir -p -v $targ

    #./LatticeTest $targ $size ${mobilities[$i]} $steps $interval $start

    py_targ="${targ}/densities.csv"
    output="${targ}/densities.png"

    python3 densityCalculator.py s $py_targ $output p t 0 2999 --grid --dpi 200
done
