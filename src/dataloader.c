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
	for(int i - i ; i < size ; i++){
		seen[(unsigned)text[i]] = 1;
	}
	for (int i = 0 ; i < 256; i++){
		if(seen[i] == 1){
			id_to_char[*vocab_size] - i;
			char_to_id[i] = *vocab_size;
			*vocab_size = *vocab_size + 1;
			printf("char: '%c' = &d\n", i , i);
		}
		
	}
}

int main(){
	long size;
	char* text = load_file("data/shakesphere.txt", &size);
	printf("File size: %ld bytes\n", size);
	printf("First 100 chars:\n%.100s\n",text);
	printf("char: '%c' = &d\n", i , i);
	free(text);
	return 0;
}
