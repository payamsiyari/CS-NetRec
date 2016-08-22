#Heir.
./generate-nets -t:0 -n:10000 -a:2 -b:0.5 -g:"0.962 0.107; 0.107 0.962" -e:20000 -c:10 -f:Heir

#Random
#./generate-nets -n:1024 -a:2 -b:0.4 -e:2048 -f:Rand

#core-priphery
#./generate-nets -n:1024 -a:2 -b:0.1 -g:"0.962 0.535; 0.535 0.107" -e:2048 -f:Core

#FF
./generate-nets -t:1 -n:1024 -g:"5;0.2;0.17;1;0;0" -e:2000 -fn:ff

#BA
#./generate-nets -t:2 -g:"BA2.txt" -a:2 -b:0.1 -f:BA
