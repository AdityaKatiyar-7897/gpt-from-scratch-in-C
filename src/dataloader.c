#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main() {
    long size;

    // 1. Open the file
    FILE* file = fopen("data/shakespeare.txt", "rb");

    // 2. Move cursor to end
    fseek(file, 0, SEEK_END);

    // 3. Get the size
    size = ftell(file);

    // 4. Move cursor back to start
    fseek(file, 0, SEEK_SET);

    // 5. Print the size
    printf("File size: %ld bytes\n", size);

    // 6. Allocate memory for the text
    char* text = malloc(size + 1);

    // 7. Read the file into memory
    fread(text, 1, size, file);

    // First: walk the text and mark seen characters
    int seen[256] = {0};
    for (int i = 0; i < size; i++) {
        seen[(unsigned char)text[i]] = 1;
    }
    
    // Second: print which characters were seen
    for (int i = 0; i < 256; i++) {
        if (seen[i] == 1) {
            printf("char: '%c' = %d\n", i, i);
       }
    }

    int char_to_id[256] = {0};
    char id_to_char[65] = {0};
    int vocab_size = 0;

    for (int i = 0 ; i < 256; i++){
    	if (seen[i] == 1){
    		id_to_char[vocab_size] = i;
    		char_to_id[i] = vocab_size;
    		vocab_size++;
    	}
    	
    }

   printf("Vocab size: %d\n", vocab_size);    

    for (int i = 0; i < vocab_size; i++) {
        printf("id %d -> '%c'\n", i, id_to_char[i]);
    }

    int*encoded = malloc(sizeof(int) * size);

    for (int i = 0 ; i < size; i++)
    {
    	encoded[i] = char_to_id[(unsigned char)text[i]];
    }

    printf("First 20 encoded values:\n");

    
    for (int i = 0; i < 20; i++) {
        printf("%d ", encoded[i]);
    }
    printf("\n");
    
    int block_size = 64;
    int batch_size = 32 ; 

    int* X = malloc(sizeof(int) * (batch_size * block_size));
    int* Y = malloc(sizeof(int) * (batch_size * block_size));

    srand(time(NULL));

    for (int b = 0 ; b< batch_size ; b++)
    {
        int start = rand() % (size - block_size - 1);
    	for (int t = 0 ; t < block_size ; t++){

    		X[b * block_size + t] = encoded[start + t];
    		Y[b * block_size + t] = encoded[start + t + 1];
    	}
    }

    printf("First batch, first sequence:\n");
    printf("X: ");
    for (int t = 0; t < block_size; t++) printf("%d ", X[t]);
    printf("\nY: ");
    for (int t = 0; t < block_size; t++) printf("%d ", Y[t]);
    printf("\n");

    
    
    // 8. Null terminate it
    text[size] = '\0';

    // 9. Print first 100 characters to verify
    printf("First 100 chars:\n%.100s\n", text);

    // 10. Free the memory
    free(text);

    // 11. Close the file
    fclose(file);

    return 0;
}
