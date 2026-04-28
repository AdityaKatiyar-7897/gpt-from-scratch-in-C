#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    // => token embedding
    float* token_emb = malloc(sizeof(float) * 65 * 64);

    srand(time(NULL));

    for (int i = 0 ; i < (65 * 64) ; i ++){
    	token_emb[i] = ((float)rand() / RAND_MAX) * 0.2f - 0.1f;
    }

    printf("Embedding for char ID 18: "); // to check if its working
    for (int i = 0; i < 8; i++) {
        printf("%.4f ", token_emb[18 * 64 + i]);
    }
    printf("\n");

 
    // => position embedding    
    float* pos_emb = malloc(sizeof(float) * 64 * 64);


    for(int i = 0 ; i < (64 * 64) ; i++)
    {
    	pos_emb [i] = ((float)rand() / RAND_MAX) * 0.2f - 0.1f;
    }

    printf("Embedding for position 0: ");
    for (int i = 0; i < 8; i++) {
        printf("%.4f ", pos_emb[0 * 64 + i]);
    }
    printf("\n");

    // => building embedding table

    float* embedded = malloc(sizeof(float) * 32 * 64 * 64);

    int X[32 * 64];
    for (int i = 0; i < 32 * 64; i++) X[i] = i % 65; // building a sample x array

    for (int b = 0 ; b < 32 ; b++)
    {
    	for (int t = 0 ; t < 64 ; t++)
    	{
    		for (int d = 0 ; d < 64 ; d++){
    			embedded[b * 64 * 64 + t * 64 + d] = token_emb[X[b * 64 + t] * 64 + d] + pos_emb[t * 64 + d];
    		}
    	}
    }

    printf("token_emb[0][0]: %.4f\n", token_emb[0 * 64 + 0]);
    printf("pos_emb[0][0]: %.4f\n", pos_emb[0 * 64 + 0]);

    printf("First 8 values of embedded[0][0]: ");
    for (int d = 0; d < 8; d++) {
        printf("%.4f ", embedded[0 * 64 * 64 + 0 * 64 + d]);
    }
    printf("\n");

    

    
    return 0;
}
