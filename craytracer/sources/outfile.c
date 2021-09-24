#include "outfile.h"

#include <stdio.h>
#include <stdlib.h>

void writeToPPM(const char * filename, int width, int height, 
                const vec3* arr){

    FILE *fptr = fopen(filename, "w");
    
    if(fptr == NULL){
        printf("ERROR: File not found.\n");
        exit(1);
    }

    fprintf(fptr, "P3\n");
    fprintf(fptr,"%d %d\n", width, height);
    fprintf(fptr,"255\n");

    for(int i = 0; i < width*height; i++){
        fprintf(fptr, "%d %d %d\n", (int)arr[i].x, (int)arr[i].y, (int)arr[i].z);
    }

    fclose(fptr);
}
