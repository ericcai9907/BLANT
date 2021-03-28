#!/bin/bash
mkdir motif_maps
make motif-map
./motif-map 4 0 64 > motif_maps/motifderivmap4.txt
./motif-map 5 0 1024 > motif_maps/motifderivmap5.txt
./motif-map 6 0 32768 > motif_maps/motifderivmap6.txt
./motif-map 7 0 2097152 > motif_maps/motifderivmap7.txt
./motif-map 8 0 30000000 > motif_maps/motifderivmap8.txt
./motif-map 8 30000000 60000000 >> motif_maps/motifderivmap8.txt
./motif-map 8 60000000 90000000 >> motif_maps/motifderivmap8.txt
./motif-map 8 90000000 120000000 >> motif_maps/motifderivmap8.txt
./motif-map 8 120000000 150000000 >> motif_maps/motifderivmap8.txt
./motif-map 8 150000000 180000000 >> motif_maps/motifderivmap8.txt
./motif-map 8 180000000 210000000 >> motif_maps/motifderivmap8.txt
./motif-map 8 210000000 240000000 >> motif_maps/motifderivmap8.txt
./motif-map 8 240000000 270000000 >> motif_maps/motifderivmap8.txt
make bin-gen
./bin-gen 4
./bin-gen 5
./bin-gen 6
./bin-gen 7
./bin-gen 8

