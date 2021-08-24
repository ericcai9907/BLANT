#include <sys/file.h>
#include <sys/mman.h>
#include <assert.h>

#include "blant.h"

#define K8_PERM 268436456
#define K8_EDGE 28

char* _BLANT_DIR = DEFAULT_BLANT_DIR;
char* _MOTIF_DIR = "motif_maps";

static int _uploaded_mapping[K8_PERM][K8_EDGE];


static int numBitValues, edges, verbose, k;

void writeMatrix(char *BUF, int derive_mapping[K8_PERM][K8_EDGE], int k)
{
    sprintf(BUF, "%s/%s/motifderivmap%d.bin", _BLANT_DIR, _MOTIF_DIR, k);
    FILE *fp = fopen(BUF, "wb");
    for(int i = 0; i<numBitValues; i++)
    {
        fwrite(derive_mapping[i],sizeof(int), edges, fp);
    }
    fclose(fp);
}



void motifPopulate(char *BUF, int derive_mapping[K8_PERM][K8_EDGE], int k) 
{
    sprintf(BUF, "%s/%s/motifderivmap%d.txt", _BLANT_DIR, _MOTIF_DIR, k);
    FILE *fp_ord=fopen(BUF, "r");
    if(!fp_ord) Fatal("cannot find %s\n", BUF);
    int numPerm, i, j, edges = k*(k-1)/2;
    assert(1==fscanf(fp_ord, "%d",&numPerm));
    for(i =0; i < numPerm; i++) for(j=0; j <edges; j++)
    {
        assert(1==fscanf(fp_ord, "%d", &derive_mapping[i][j]));
    }
    fclose(fp_ord);
    return;
}

static char USAGE[] = "USAGE: $0 k";

int main(int argc, char* argv[]){
    if(argc != 2){fprintf(stderr, "expecting exactly one argument, which is k\n%s\n",USAGE); exit(1);}
    k = atoi(argv[1]);
    if(k<=8) numBitValues = 1 << k*(k-1)/2;
    edges = k*(k-1)/2;
    char BUF[BUFSIZ];
    motifPopulate(BUF, _uploaded_mapping,k);
    writeMatrix(BUF,_uploaded_mapping,k);
    return 0;
}
