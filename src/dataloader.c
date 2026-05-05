#include <stdio.h>
#include <time.h>
#include <stdlib.h>

char* load_file(char* filename, long* size){
	FILE* file = fopen(filename, "rb");

	fseek (file,0,SEEK_END);
	*size = ftell(file);
	fseek (file , 0 , SEEK_SET);

	char* text = malloc(*size + 1);

	fread(text, 1 , *size , file);
	return text;
}

void build_vocab(char* text, long size, int* char_to_id, char* id_to_char, int* vocab_size){
	int seen[256] = {0};
	for(int i = 0 ; i < size ; i++){
		seen[(unsigned)text[i]] = 1;
	}
	for (int i = 0 ; i < 256; i++){
		if(seen[i] == 1){
			id_to_char[*vocab_size] = i;
			char_to_id[i] = *vocab_size;
			*vocab_size = *vocab_size + 1;
			printf("char: '%c' = %d\n", i , i);
		}
		
	}
}

int* encode(char* text , long size , int* char_to_id){
    int* encoded = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        encoded[i] = char_to_id[(unsigned char)text[i]];
        
    }
	return encoded;
	
}

void sample_batch(int* encoded, long size, int* X, int* Y, int batch_size, int block_size) {
    for (int i = 0; i < batch_size; i++) {
        int start = rand() % (size - block_size - 1);
        for (int j = 0; j < block_size; j++) {
            X[i * block_size + j] = encoded[start + j];
            Y[i * block_size + j] = encoded[start + j + 1];
        }
    }
}
	


int main() {
    long size;
    char* text = load_file("data/shakespeare.txt", &size);
    printf("File size: %ld bytes\n", size);
    printf("First 100 chars:\n%.100s\n", text);
    
    int char_to_id[256];
    char id_to_char[256];
    int vocab_size = 0;
    build_vocab(text, size, char_to_id, id_to_char, &vocab_size);
    printf("Vocab size: %d\n", vocab_size);

    int* encoded = encode(text, size, char_to_id);
    printf("First 20 encoded values:\n");
    for (int i = 0; i < 20; i++) {
        printf("%d ", encoded[i]);
    }
    printf("\n");

    int X[32 * 64];
    int Y[32 * 64];
    sample_batch(encoded, size, X, Y, 32, 64);
    
    printf("First sequence X: ");
    for (int t = 0; t < 64; t++) printf("%d ", X[t]);
    printf("\n");
    
    free(text);
    return 0;

}
