#!/bin/bash
echo "calculando Memory bandwidth"
cpu=3

# 32 50 64 100 128 200 256 300 400 512 1000
# for i in 10 32 50 64 100 128 200 256 300 400 512 1000
# do
#     ./perfctr.sh 3 L3 ./sistemalinear < Entradas/input$1 | grep "L3 bandwidth" | awk '{print  "'$i'", $6}' >> results/LUajusteMem.txt
# done

# for i in 10 32 50 64 100 128 200 256 300 400 512 1000
# do
#     ./perfctr.sh 3 L2CACHE ./sistemalinear < Entradas/input$1 | grep "L2 miss ratio" | awk '{print  "'$i'", $6}' >> results/LUajusteCache.txt
# done

for i in 10 32 50 64 100 128 200 256 300 400 512 1000
do
    ./perfctr.sh 3 FLOPS_DP ./sistemalinear < Entradas/input$1 | grep "DP" | sed '/^R/d' | sed 's/|//g' | awk '{print $NF}'|  awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}' | awk '{print  "'$i'", $1, $2}' >> results/LUajusteFlops.txt
done

# for i in 10 32 50 64 100 128 200 256 300 400 512 1000
# do
#     ./perfctr.sh 3 CPU_CLK_UNHALTED_CORE:FIXC1 ./sistemalinear < Entradas/input$1 | grep "Runtime" | sed 's/|//g' | awk '{print $NF}' |  awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}' | awk '{print "'$i'", $2*1000}' >> results/LUajusteTime.txt
# done