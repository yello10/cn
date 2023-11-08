Compilation and execution guidelines:
--------------------------------------
1) For server.c

gcc -o ser server.c
./ser <1> (for no packet dropping)
./ser <0> (for random packet dropping)

2) For client.c

gcc -o cli client.c
./cli localhost <input file name> <timeout in microseconds> <ssthresh>

timeout in microseconds should be > 100000


Output files:
--------------
1) out.txt contains the data of input file <input file name> which has been successfully transmitted by the programs.
2) All other output files that are generated were used for making plots. Kindly ignore these files.