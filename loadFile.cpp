#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#define MAX_FILE_SIZE 1024 
//	char buffer[MAX_FILE_SIZE];
char *loadFile(const char *filename) {
char* buffer;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    size_t len = fread(buffer, 1, MAX_FILE_SIZE, file);
    if (ferror(file)) {
        perror("Error reading file");
        fclose(file);
        return NULL;
    }
    fclose(file);
    buffer[len] = '\0';
    return buffer;
}


int main(){
 char *content = loadFile("input.php");

printf("%s",content);
return 0;
}
