#****************************************BA-1000n-2000e********************
#./DNS -gg:1 -in:Synthetic_Dataset/BA-1000n-2000e/network.txt -gd:0 -m:0 -a:1 -b:0.5 -c:-50 -s:1 -sr:0.5 -dn:Synthetic_Dataset/BA-1000n-2000e/DiffNet.txt -dm:0 -o:Synthetic_Dataset/BA-1000n-2000e/Structural_DNS.txt -cr:Synthetic_Dataset/BA-1000n-2000e/Structure_Criteria_0.5.txt

#./DNS -gg:1 -in:Synthetic_Dataset/BA-1000n-2000e/network.txt -gd:0 -m:0 -a:1 -b:0.5 -c:-50 -s:1 -sr:0.5 -dn:Synthetic_Dataset/BA-1000n-2000e/DiffNet.txt -dm:1 -o:Synthetic_Dataset/BA-1000n-2000e/Diffusion_DNS.txt -cr:Synthetic_Dataset/BA-1000n-2000e/Diffusion_Criteria_0.5.txt


#***************************************FF***********************************
#./DNS -gg:0 -n:5000 -g:"1,0.2,0.17,1,0,0" -fn:Synthetic_Dataset/FF-1024n-1421e/network.txt -gd:0 -m:0 -a:1 -b:0.5 -c:-50 -s:1 -sr:0.5 -dn:Synthetic_Dataset/FF-1024n-1421e/DiffNet.txt -dm:0 -o:Synthetic_Dataset/FF-1024n-1421e/Structural_DNS.txt -cr:Synthetic_Dataset/FF-1024n-1421e/Structure_Criteria.txt

#./DNS -gg:1 -fn:Synthetic_Dataset/FF-1024n-1421e/network.txt -gd:0 -m:0 -a:1 -b:0.5 -c:-50 -s:1 -sr:0.5 -dn:Synthetic_Dataset/FF-1024n-1421e/DiffNet.txt -dm:1 -o:Synthetic_Dataset/FF-1024n-1421e/Diffusion_DNS.txt -cr:Synthetic_Dataset/FF-1024n-1421e/Diffusion_Criteria.txt


#./DNS -gg:1 -n:Synthetic_Dataset/Heir-1024n-1475e/network.txt -gd:0 -m:0 -a:1 -b:0.5 -c:-50 -s:1 -sr:0.5 -dn:Synthetic_Dataset/Heir-1024n-1475e/DiffNet.txt -dm:0 -o:Synthetic_Dataset/FF-1024n-1421e/Structural_DNS.txt -cr:Synthetic_Dataset/Heir-1024n-1475e/Structure_Criteria.txt

#./DNS -gg:1 -n:Synthetic_Dataset/Heir-1024n-1475e/network.txt -gd:0 -m:0 -a:1 -b:0.5 -c:-50 -s:1 -sr:0.5 -dn:Synthetic_Dataset/Heir-1024n-1475e/DiffNet.txt -dm:1 -o:Synthetic_Dataset/FF-1024n-1421e/Diffusion_DNS.txt -cr:Synthetic_Dataset/Heir-1024n-1475e/Diffusion_Criteria.txt



# ./DNS -n:Synthetic_Dataset/Core-1024n-2048e/network.txt -m:0 -a:1 -b:5 -c:-50 -f:Synthetic_Dataset/Core-1024n-2048e/Results/FF -o:Synthetic_Dataset/Core-1024n-2048e/Results/DiffNet

# ./DNS -n:Synthetic_Dataset/Rand-1024n-2048e/network.txt -m:0 -a:1 -b:5 -c:-50 -f:Synthetic_Dataset/Rand-1024n-2048e/Results/FF -o:Synthetic_Dataset/Rand-1024n-2048e/Results/DiffNet

#************************************Heir*********************************

#./DNS -n:Synthetic_Dataset/Heir-4096n-4550e/network.txt -c:-50 -a:2 -b:0.5 -dn:Synthetic_Dataset/Heir-4096n-4550e/DiffNet.txt -s:1 -sr:0.5 


#************************************Rand*********************************

./DNS -n:Synthetic_Dataset/Rand-8192n-15000e/network.txt -c:-50 -a:2 -b:0.5 -dn:Synthetic_Dataset/Rand-8192n-15000e/DiffNet-1.txt -s:0 -sr:0.1
