#include "include/io.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *filename) {
    char* buff = 0;
    long len;
    FILE *f = fopen(filename, "rb");

    // check if file exists and is a .st file
    if(f && strstr(filename, ".st")) {
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        fseek(f, 0, SEEK_SET);

        buff = calloc(len, len);
        if (buff) fread(buff, 1, len, f);

        fclose(f);
        return buff;
    }

    if(!strstr(filename, ".st")) {
        printf("[Error]: file '%s' is not a .st file.\n", filename);
        exit(2);
    }

    printf("[Error]: could not read file '%s'.\n", filename);
    exit(2);
}