/*napisz podobny program ale z uzyciem buffora danych buffsize zamiast malloc tym bardziej ze potrzebuje wczytywac ploki typu action.php wiec buffet 1519 wystarczy */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buffer 1519
char *loadFile(const char *filename) {
    FILE *file;
    char *str;
    size_t len = 0;
    file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
  str = (char*)malloc(file_size + 1); 
    fread(str, 1, file_size, file);
    str[file_size] = '\0';
    fclose(file);  
    return str;
     free(str); 
}
int main() {
    char *content = loadFile("input.html");
        printf("%s", content);
    return 0;
}
