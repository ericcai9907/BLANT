#include <stdio.h>
#include <sys/file.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "blant.h"

static TINY_GRAPH *G;
static int k, numBits, start, stop; // number of bits required to store ints up to 2^(k choose 2)

#if LOWER_TRIANGLE

unsigned long long bitArrayToDecimal(int bitMatrix[k][k], char Permutations[], int numBits){
    unsigned long long num=0;
    int lf=0;
    for(int i = 1; i < k; i++)
		for(int j=0; j < i; j++){
		    num+=(((unsigned long long)bitMatrix[(int)Permutations[i]][(int)Permutations[j]]) << (numBits-1-lf));
		    lf++;
		}
    return num;
}

void decimalToBitArray(int bitMatrix[k][k], unsigned long long D){
    for(int i=k-1; i>=1; i--)
		for(int j=i-1; j>=0; j--){
		    bitMatrix[i][j] = D%2;
		    bitMatrix[j][i]=bitMatrix[i][j];
		    D = D/2;
		}
}


#else

unsigned long long bitArrayToDecimal(int bitMatrix[k][k], char Permutations[], int numBits){
    unsigned long long num=0;
    int lf=0;
    for(int i = 0; i < k-1; i++)
		for(int j=i+1; j < k; j++){
		    num+=(((unsigned long long)bitMatrix[(int)Permutations[i]][(int)Permutations[j]]) << (numBits-1-lf));
		    lf++;
		}
    return num;
}

void decimalToBitArray(int bitMatrix[k][k], unsigned long long D){
    for(int i=k-2; i>=0; i--)
		for(int j=k-1; j>i; j--){
		    bitMatrix[i][j] = D%2;
		    bitMatrix[j][i]=bitMatrix[i][j];
		    D = D/2;
		}
}

#endif


static unsigned long long numBitValues; // the actual value 2^(k choose 2)



void perm_lookup(void)
{
    FILE *fcanon = stdout;

    numBits = (k*(k-1))/2;
    int bitMatrixloc[numBits];
    int bitMatrix[k][k];
    //printf("In perm_lookup!\n");
    if(start == 0) fprintf(fcanon, "%d\n", numBitValues);
    int loc = 0;
    for(int x = 0; x < k; x++)
    {
    	for(int y = 0; y < x; y++)
    	{
    		bitMatrixloc[loc] = x*k+y;
    		loc++;
    	}
    }

    for(int i=start; i<stop; i++)
    {
    	//printf("At Loop %d!\n", i);
    	int derivatives[numBits];
    	int count = 0;
    	decimalToBitArray(bitMatrix,i);
    	for(int j = numBits-1; j >= 0; j--)
    	{	
    		int x = bitMatrixloc[numBits-1-j]/k, y = bitMatrixloc[numBits-1-j]%k;
    		if(i > (1<<j) && bitMatrix[x][y])
    		{
    			int perm = i - (1 << j);
    			G = TinyGraphAlloc(k);
		    	Int2TinyGraph(G, perm);
		    	int nodeArray[k], distArray[k], connected = (TinyGraphBFS(G, 0, k, nodeArray, distArray) == k);
		    	if(connected)
		    	{
		    		derivatives[count] = perm;
		    		count++;
		    	} 
    		}
    		
    	}
    	if(count <= numBits-1)
    		{
    			//printf("At Extras!\n");
    			for(int rest = count; rest <=numBits-1; rest++) derivatives[rest] = 0;
    		}
    	for( int der = 0; der < numBits; der++)
    	{
    		//printf("Printing stuffs!\n");
    		char sep = ' ';
    		fprintf(fcanon, "%d%c", derivatives[der],sep);
    	}
    	fprintf(fcanon, "\n");
    }

}

static char USAGE[] = "USAGE: $0 k start stop";

int main(int argc, char* argv[]){
    if(argc != 4){fprintf(stderr, "expecting exactly one argument, which is k\n%s\n",USAGE); exit(1);}
    k = atoi(argv[1]);
    start = atoi(argv[2]);
    stop = atoi(argv[3]);
    if(k<=8) numBitValues = 1 << k*(k-1)/2;
    else numBitValues = 1LL << k*(k-1LL)/2;
    if(stop > numBitValues) stop = numBitValues;
    //data = malloc(sizeof(xChar)*numBitValues);
    //done = malloc(sizeof(bool)*numBitValues);
    perm_lookup();
    return 0;
}
