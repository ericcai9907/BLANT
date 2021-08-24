README

To set up the system for use of our program:

Where to add our files:
In BLANT directory:
	generate_lookup_tables.sh
	Makefile
In src:
	bin_gen.c
	motif_gen.c
	blant.h
	blant.c
	blant-output.c
	blant-utils.c
	libblant.c

From a new clone: 
I recommend running a regression-test-all.sh -make on the official clone before adding our files.
This will ensure that the system starts at a solid foundation 
As our lookup table approach has a certain level of uncertainty
Then add our files and run make blant

For a built system: 
add the files that we provided and run make blant

To build the lookup tables that we used:
bash generate_lookup_tables.sh

After this runs you should have a motif_maps directory with motifderivmapk.txt and motifderivmapk.bin for k = 4 - 8

We have added two new output modes to BLANT

To run our print all motifs output using an edge based approach

./blant -k[k value] -n[Samples] -s[Sampling Mode] -m[e] [network]

This is the more stable version of our output and works for all values k = 4-8

To run our print all motifs output using a lookup table approach

./blant -k[k value] -n[Samples] -s[Sampling Mode] -m[r] [network]

This is available for values of k= 4-7 as the matrix for k8 overflowed the memory. 
However this output mode is a little unstable, 
we believe that it is due to an additional memory issue that we inadvertantly caused

We find that it works at times and randomly fails 
We found that a simple recomplication of make blant after a trivial change, such as deleting a semicolon and adding it again
to the modified files, blant.h, blant.c, blant-output.c, blant-utils.c, or libblant.c fixes the problem for a while

