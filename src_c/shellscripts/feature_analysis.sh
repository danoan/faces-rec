#[SHIFT STEP] [RESIZE FACTOR] [WIDTH] [HEIGHT] [FINAL FP] [FINAL DET [MAX FP] [MAX DET] [STAGES] [MAX LENGTH VS]
#./a.out -s 1 -r 1.25 -w 6 -h 6 -f 0.006 -d 0.9 -m 0.6 -t 0.99 -n 10 -v 280     -u -z -i 2100

rm log/feat_anal.txt

for s in 1 2 3 4 5
do
    for r in 1.25 1.5 1.75 2.0
    do
        for w in 6 8 10
        do
            for h in 6 8 10
            do
                echo "./a.out -s $s -r $r -w $w -h $h -f 0.006 -d 0.9 -m 0.6 -0.99 -n 10 -v 280 -u -z -i 2100"
            done
        done
    done

done
