#include <stdio.h>
#include <sys/file.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "blant.h"

static int k, numBits; // number of bits required to store ints up to 2^(k choose 2)
static unsigned long long numBitValues; // the actual value 2^(k choose 2)

static TINY_GRAPH *G;
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



typedef unsigned char xChar[5];//40 bits for saving index of canonical decimal and permutation

static xChar* data;
static bool* done;
static unsigned long long canonicalDecimal[274668];//274668 canonical graphettes for k=9

unsigned long long power(int x, int y){
    if(y==0)return 1;
    return (unsigned long long)x*power(x,y-1);
}

void encodeChar(xChar ch, long indexD, long indexP){

    unsigned long long x=(unsigned long long)indexD+(unsigned long long)indexP*power(2,19);//19 bits for canonical decimal index
    unsigned long long z=power(2,8);
    for(int i=4; i>=0; i--){
	ch[i]=(char)(x%z);
	x/=z;
    }
}

void decodeChar(xChar ch, long* indexD, long* indexP){

    unsigned long long x=0,m;
    int y=0,w;

    for(int i=4; i>=0; i--){
	w=(int)ch[i];
	m=power(2,y);
	x+=w*m;
	y+=8;
    }
    unsigned long long z=power(2,19);
    *indexD=x%z;
    *indexP=x/z;
}

long factorial(int n) {
    if(n==0)return 1;
    return (long)n*factorial(n-1);
}

bool nextPermutation(int permutation[]) {
    int t;
    for(int i=k-1;i>0;i--) {
	if(permutation[i]>permutation[i-1]) {
	    for(int j=k-1;j>i-1;j--){
		if(permutation[i-1]<permutation[j]){
		    int t=permutation[i-1];
		    permutation[i-1]=permutation[j];
		    permutation[j]=t;
		    break;
		}
	    }
	    int l=i;
	    for(int j=k-1;j>l;j--) {
		if(i<j){
		    t=permutation[i];
		    permutation[i]=permutation[j];
		    permutation[j]=t;
		    i++;
		}
	    }
	    return 1;
	}
    }
    return 0;
}

void canon_map(void){
    FILE *fcanon = stdout;

    numBits = (k*(k-1))/2;
    unsigned long long D;
    int bitMatrix[k][k];

    for(unsigned long long i=0; i<numBitValues; i++)done[i]=0;
    canonicalDecimal[0]=0;
    long f=factorial(k);
    char Permutations[f][k];
    int tmpPerm[k];
    for(int i=0;i<k;i++)tmpPerm[i]=i;

    //saving all permutations
    for(long i=0;i<f;i++){
	for(int j=0; j<k; j++)
	    Permutations[i][j]=tmpPerm[j];
	nextPermutation(tmpPerm);
    }
    done[0]=1;
    encodeChar(data[0],0,0);
    long num_canon=0;

    //finding canonical forms of all graphettes
    for(unsigned long long t=1; t<numBitValues; t++){
	if(done[t]) continue;
	done[t]=1; // this is a new canonical, and it the lowest by construction
	encodeChar(data[t],++num_canon,0);
	canonicalDecimal[num_canon]=t;

	unsigned long long num = 0;
	decimalToBitArray(bitMatrix, t);
	for(long nP=1; nP<f; nP++) // now go through all the permutations to compute the non-canonicals of t.
	{
	    num=bitArrayToDecimal(bitMatrix, Permutations[nP], numBits);
	    if(!done[num]){
		done[num]=true;
		encodeChar(data[num],num_canon,nP);
	    }
	}

    }

    // It's easiest to generate the permumations from the non-canonical to the canonical,
    // but the inverse is far more useful. Here we invert all the permutations so that
    // they map from canonical nodes to the non-canonical. That way, we can spend most of
    // our time thinking in "canonical space", and if we want to know where to find canonical
    // node j in a particular non-canonical, we use perm[j].
    if(PERMS_CAN2NON){
	int tmp[k];
	for(int i=0; i<f; i++){
	    for(int j=0; j<k; j++)
		tmp[(int)Permutations[i][j]]=j;
	    for(int j=0; j<k; j++)
		Permutations[i][j]=tmp[j];
	}
    }

    //saving canonical decimal and permutation in the file
    long canonDec, canonPerm;
    for(unsigned long long i=0; i<numBitValues; i++){
	canonDec=0;canonPerm=0;
	decodeChar(data[i],&canonDec,&canonPerm);
	assert(canonDec >= 0);
	assert(canonPerm >= 0);
	fprintf(fcanon,"%llu\t%llu\t", i,canonicalDecimal[canonDec]);
	for(int p=0;p<k;p++)
	    fprintf(fcanon,"%d", Permutations[canonPerm][p]);
	if(canonPerm == 0) {
	    G = TinyGraphAlloc(k);
	    Int2TinyGraph(G, i);
	    int nodeArray[k], distArray[k], connected = (TinyGraphBFS(G, 0, k, nodeArray, distArray) == k);
	    fprintf(fcanon, "\t%d %d", connected, TinyGraphNumEdges(G));
	    int u,v,sep='\t';
	    for(u=0;u<k;u++)for(v=u+1;v<k;v++) if(TinyGraphAreConnected(G,u,v)) {
		fprintf(fcanon, "%c%d,%d",sep,u,v); sep=' ';
	    }
	}
	fprintf(fcanon,"\n");
    }
    //fprintf(fcanon,"%lld",canonicalDecimal[0]);
}

static char USAGE[] = "USAGE: $0 k";

int main(int argc, char* argv[]){
    if(argc != 2){fprintf(stderr, "expecting exactly one argument, which is k\n%s\n",USAGE); exit(1);}
    k = atoi(argv[1]);
    if(k<=8) numBitValues = 1 << k*(k-1)/2;
    else numBitValues = 1LL << k*(k-1LL)/2;
    data = malloc(sizeof(xChar)*numBitValues);
    done = malloc(sizeof(bool)*numBitValues);
    canon_map();
    return 0;
}
