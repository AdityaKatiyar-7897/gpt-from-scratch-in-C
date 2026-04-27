#include <stdio.h>
#include <stdlib.h>

int main() {

    	long size;
    // 1. Open the file
    FILE* file = fopen("data/shakespeare.txt", "r");
    
    // 2. Move cursor to end
    // YOUR LINE HERE — fseek
    fseek(file, 0 , SEEK_END);
    
    // 3. Get the size
    // YOUR LINE HERE — ftell, store result in a variable called 'size'
    size = ftell(file);
    
    // 4. Move cursor back to start
    // YOUR LINE HERE — fseek again
    fseek(file, 0 , SEEK_SET);
    // 5. Print the size
    printf("File size: %ld bytes\n", size);
    
    // 6. Close the file
    fclose(file);
    
    return 0;
}
