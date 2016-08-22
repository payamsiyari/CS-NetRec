#!/bin/bash
file="Data/polblogs-1491n-19090e-5717c"
#"Data/Rand-1024n-2048e-1813c"
#"Data/Heir-1024n-2048e-4000c"
#"Data/FF-1024n-1221e-2786c"
#"Data/Core-1024n-2048e-2350c" 
#"Data/BA-1000n-2000e-4824c" 
#"Data/netscience-n1589-e2742-6426c" 
#"Data/Rand-4096n-8192e-7456c" 

#Step

if [ $1 -eq 1 ]
then
	mkdir $file
	mkdir $file/Results
	echo "mkdir done"
fi

#Step2
if [ $1 -eq 2 ]
then
	./generate-nets -t:2 -g:"$file/network.txt" -a:2 -b:0.1 -f:$file/"$casc"
	echo "generate 2 done"
fi

#Step3
if [ $1 -eq 3 ]
then
	for casc in 7000 6000 5000 4000 3000 2000 1000 500 300 100
	do
		./generate-nets -t:2 -g:"$file/network.txt" -a:2 -b:0.1 -c:"$casc" -f:$file/"$casc"
	done
	echo "generate 3 done"
fi

#**************************************************Step4**********************************************
if [ $1 -eq 4 ]
then 
	rm $file/results.txt 
	
	echo "Edges	Cascs		Precision	Recall		F-measure	Time" > $file/results.txt 

	for edge in 19000 #2048 1500 1000 500 300 100
	do
		for casc in 5717 #1000 500 200
		do
			echo "$edge $casc"
			./netinf -i:"$file"/"$casc"-cascades.txt -n:"$file"/network.txt -e:"$edge" -r:"$file"/results.txt #-f:"$file"/Results/"$edge"e-"$casc"c-output.txt -a:2 
		done
		echo  >> $file/results.txt
	done
	echo "generate 4 done" 
fi
