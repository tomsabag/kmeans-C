#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int getSize (char * s);

typedef struct {
    float *lst;
    int size;
} DataPoint;

DataPoint createDataPoint(char* arrOfStrings, int length);


typedef struct {
    float *lst;
    int size;
} Centroid;

typedef struct {

} Cluster;

int main() {
    int k = 3;
    char c[1000];
    FILE *fptr;
    fptr = fopen("C:/Users/tomsa/CLionProjects/kmeans/input.txt", "r");
    int lineCounter = 0;
    while(feof(fptr) == 0) {
        fgets(c, 1000, fptr);
        lineCounter++;
    }
    DataPoint* dataPointArr = (DataPoint *) calloc(lineCounter, sizeof(DataPoint));

    rewind(fptr);
    int i = 0;
    while (feof(fptr) == 0) {
        fgets(c, 1000, fptr);
        dataPointArr[i] = createDataPoint(c, getSize(c));
        i++;
    }
    for (int m=0; m<800; m++) {
        for (int x = 0; x < dataPointArr[m].size; x++) {
            printf("%f ", dataPointArr[m].lst[x]);
        }
        printf("\n");
    }
    fclose(fptr);
}

void initializeCentroids() {

}
int getSize (char * s) {
    char * t; // first copy the pointer to not change the original
    int size = 0;

    for (t = s; *t != '\0'; t++) {
        size++;
    }
    return size;
}

int countCommas(const char* arrOfStrings, int length) {
    int counter = 1;
    for (int i=0; i<length; i++)
        if (arrOfStrings[i]==',')
            counter++;
    return counter;
}


DataPoint createDataPoint(char* arrOfStrings, int length) {
    int size = countCommas(arrOfStrings, length);
    float *floatsPtr = (float*) calloc(size, sizeof(float));
    int j=0;
    char* piece = strtok(arrOfStrings, ",");
    while (piece != NULL) {
        floatsPtr[j] = atof(piece);
        //printf("piece:%s\n", piece);

        piece = strtok(NULL, ",");
        j++;
    }
    DataPoint dataPoint = {floatsPtr, size};
    return dataPoint;
}