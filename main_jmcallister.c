#include <stdio.h>
#include <stdlib.h>

void readFile(char* fileLocation) {
    FILE *fp = NULL;
    fp = fopen(fileLocation, "r");
    char* currentWord[20];
    while (!feof(fp)) {
        if (fscanf(fp, "%s", currentWord) != 1) {
            break;
        }
        printf("Currently in readFile!\n");
        printf("%s\n", currentWord);
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    printf("Number of arguments: %d %s", argc, "\n");
    if (argc != 2) {
        printf("ERROR: Illegal number of arguments.\n");
        exit(1);
    }
    readFile(argv[1]);
    return 0;
}