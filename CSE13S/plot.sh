#!/bin/bash

make clean && make collatz
echo >> datafile.dat
echo >> data.dat
echo >> placeholder.dat
for i in {2..10000}
do
	./collatz -n $i > collatz.dat;
	wc -l < collatz.dat > datafile.dat;
	sort -n -r -o sorted.dat collatz.dat;
	head -n 1 sorted.dat >> moredata.dat;
	cat datafile.dat >> data.dat;
	rm datafile.dat;
	rm collatz.dat;
	rm sorted.dat;
done
sort -g data.dat > placeholder.dat
uniq -c placeholder.dat >> histogram.dat

gnuplot <<END
	set terminal pdf
	set output "collatz.pdf"
	set title "Collatz Sequence Lengths"
	set xlabel "n"
	set ylabel "y"
	set zeroaxis
	plot "data.dat" title "" linetype 0 linecolor 4
END

gnuplot <<END
	set terminal pdf
	set output "histogram.pdf"
	set title "Collatz Sequence Length Histogram"
	set xlabel "length"
	set ylabel "frequency"
	set xrange [0:225]
	set yrange [0:200]
	set zeroaxis
	plot "histogram.dat" title "" linecolor 4 with boxes
END

gnuplot <<END
	set terminal pdf
	set output "maxCollatz.pdf"
	set title "Maximum Collatz Sequence Value"
	set xlabel "n"
	set yrange [0:100000]
	set ylabel "value"
	set zeroaxis
	plot "moredata.dat" title "" linetype 0 linecolor 4
END
rm moredata.dat
rm data.dat
rm placeholder.dat
rm histogram.dat
make clean
