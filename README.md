# CS-NetRec
Network Reconstruction under Compressive Sensing

This is the implementation of the CS-NetRec framework presented in the paper [1] and [2].

## Installation & Usage
* To generate cascades for a network file:
	* Change the directory to ```Generate Cascades/GenerateCascades``` and run ```./DNS``` (See DNS usage)

* To Extract Cascade A Matrix & y Vector:
	* Run ```extractCascMatrix```

* To run the optimization:
	* Run ```CSNetRec```
* Make sure to compile the code in repeats1 directory by simply running ```make```.
* Make sure to install NetworkX by running ```pip install networkx```.

## References
* [1] P. Siyari, H. R. Rabiee, M. Salehi, M. Eslami. "Network Reconstruction under Compressive Sensing", Proceedings of ASE/IEEE International Conference on Social Informatics, pp. 19-25, IEEE Comp. Soc., Dec. 2012.
* [2] P. Siyari, H. R. Rabiee, M. Salehi, M. Eslami. "Network Reconstruction under Compressive Sensing", ASE Human Journal, Vol. 1, No. 3, pp. 130-143, 2012.