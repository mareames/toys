#!/bin/sh

./sorts -k 10  -c 10000     > sorts.10k.c10k.txt
./sorts -k 100 -c 100000    > sorts.100k.c100k.txt
./sorts -m 1   -c 1000000   > sorts.1m.c1m.txt
./sorts -m 10  -c 10000000  > sorts.10m.c10m.txt
./sorts -m 100 -c 100000000 > sorts.100m.c100m.txt
./sorts -m 1   -c 10000     > sorts.1m.c10k.txt
./sorts -m 10  -c 10000     > sorts.10m.c10k.txt
./sorts -m 100 -c 10000     > sorts.100m.c10k.txt
./sorts -m 1   -c 100000    > sorts.1m.c100k.txt
./sorts -m 10  -c 100000    > sorts.10m.c100k.txt
./sorts -m 100 -c 100000    > sorts.100m.c100k.txt
