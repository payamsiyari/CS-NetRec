#Heir.
#./GenNets -n:1024 -a:2 -b:0.5 -g:"0.962 0.107; 0.107 0.962" -e:2048 -c:4000 -f:Heir

#Random
#./GenNets -n:1024 -a:2 -b:0.4 -e:2048 -f:Rand

#core-priphery
#./GenNets -n:1024 -a:2 -b:0.1 -g:"0.962 0.535; 0.535 0.107" -e:2048 -f:Core

#FF
./GenNets -t:1 -n:1024 -g:"5;0.2;0.17;1;0;0" -e:2000 -fn:ff

#BA
#./GenNets -t:2 -g:"BA2.txt" -a:2 -b:0.1 -f:BA
