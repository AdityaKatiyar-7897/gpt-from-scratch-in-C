#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// => GELU function

float gelu(float x){
	return 0.5f * x * (1.0f + tanhf(0.7978845608f * (x + 0.044715f * x * x * x)));
}

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

    // => building embedding lookup tableis

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

    // => Transformer

    float* Wq = malloc(sizeof(float) * 64 * 64);
    float* Wk = malloc(sizeof(float) * 64 * 64);
    float* Wv = malloc(sizeof(float) * 64 * 64);

    for (int i = 0 ; i < (64 * 64) ; i ++)
    {
    	  Wq[i] =  ((float)rand() / RAND_MAX) * 0.2f - 0.1f;
    }

    for (int j = 0 ;j < (64 * 64) ; j ++)
        {
        	  Wk[j] = ((float)rand() / RAND_MAX) * 0.2f - 0.1f;
        }


    for (int k = 0 ; k < (64 * 64) ; k ++)
        {
        	  Wv[k] = ((float)rand() / RAND_MAX) * 0.2f - 0.1f;
        }


    float* Q = malloc(sizeof(float) * 64 * 64);
    float* K = malloc(sizeof(float) * 64 * 64);
    float* V = malloc(sizeof(float) * 64  * 64);


    for (int i = 0; i < 64 * 64; i++) Q[i] = 0.0f;

    for (int t = 0; t < 64 ; t++){
    	for (int d = 0 ; d < 64; d++){
    		for (int i = 0 ; i < 64 ; i++)
    		{
    			Q[t * 64 + d] += embedded[t * 64 + i] * Wq[i * 64 + d];
    		}
    	}
    }    



    for (int i = 0; i < 64 * 64; i++) K[i] = 0.0f;
    for (int t = 0; t < 64; t++){
        for (int d = 0; d < 64; d++){
            for (int i = 0; i < 64; i++){
                K[t * 64 + d] += embedded[t * 64 + i] * Wk[i * 64 + d];
            }
        }
    }
    
    for (int i = 0; i < 64 * 64; i++) V[i] = 0.0f;
    for (int t = 0; t < 64; t++){
        for (int d = 0; d < 64; d++){
            for (int i = 0; i < 64; i++){
                V[t * 64 + d] += embedded[t * 64 + i] * Wv[i * 64 + d];
            }
        }
    }

    printf("\nAttention scores for position 13 ('B'):\n"); // sample testing
    for (int s = 0; s <= 13; s++) {
        float score = 0.0f;
        for (int d = 0; d < 64; d++) {
            score += Q[13 * 64 + d] * K[s * 64 + d];
        }
        score /= 8.0f; // scale by sqrt(64)
        printf("position %d: %.4f\n", s, score);
    }

    printf("Character ID at position 13: %d\n", X[13]);


    // => Softmax

   
   float scores[64] = {0.0f};
   
 
   for (int s = 0; s <= 13; s++) {
       float score = 0.0f;
       for (int d = 0; d < 64; d++) {
           score += Q[13 * 64 + d] * K[s * 64 + d];
       }
       scores[s] = score / 8.0f;
   }
   
   
   float max_score = scores[0];
   for (int s = 1; s <= 13; s++)
       if (scores[s] > max_score) max_score = scores[s];
   
   float sum = 0.0f;
   for (int s = 0; s <= 13; s++) {
       scores[s] = expf(scores[s] - max_score);
       sum += scores[s];
   }
   for (int s = 0; s <= 13; s++)
       scores[s] /= sum;
   
  
   printf("\nSoftmax attention weights for position 13:\n");
   for (int s = 0; s <= 13; s++)
       printf("position %d: %.4f\n", s, scores[s]);


   // => weighted sum of V vectors
   float attn_out[64] = {0.0f};
   for (int s = 0; s <= 13; s++) {
       for (int d = 0; d < 64; d++) {
           attn_out[d] += scores[s] * V[s * 64 + d];
       }
   }
   
   printf("\nAttention output for position 13 (first 8 values):\n");
   for (int d = 0; d < 8; d++) {
       printf("%.4f ", attn_out[d]);
   }
   printf("\n");


   // => Feedforward layer 

   // => feedforward layer
   float* W1 = malloc(sizeof(float) * 64 * 256);
   float* W2 = malloc(sizeof(float) * 256 * 64);

   
   for (int i = 0; i < 64 * 256; i++){
       W1[i] = ((float)rand() / RAND_MAX) * 0.2f - 0.1f;
   }
    
   for (int i = 0; i < 256 * 64; i++){
   	
       W2[i] = ((float)rand() / RAND_MAX) * 0.2f - 0.1f;
   }


   float hidden[256] = {0.0f};
   
   for (int d = 0; d < 256 ; d++){
   	for (int i = 0; i < 64 ; i++){
   		hidden[d] += attn_out[i] * W1[i * 256 + d];
   	}
   }

   for(int d = 0 ; d < 256 ; d++){
       hidden[d] = gelu(hidden[d]);
   }

   float ff_out[64] = {0.0f};
   for (int d = 0 ; d < 64 ; d++){
   	    for (int i = 0 ; i < 256 ; i++){
   	    	ff_out[d] += hidden[i] * W2[i * 64 + d];
   	    }
   }

   printf("\nFeedforward output (first 8 values):\n");
   for (int d = 0; d < 8; d++) printf("%.4f ", ff_out[d]);
   printf("\n");
   
    return 0;
}
