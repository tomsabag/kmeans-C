#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    float *lst;
    int size;
} DataPoint;


typedef struct {
    float *lst;
    int size;
} Centroid;

typedef struct {
    Centroid centroid;
    float * sumVector;
    int size;
} Cluster;

int getSize (char * s);
Cluster initializeClusters(DataPoint* dataPointArr, int i);
float euclidean(Centroid centroid, DataPoint dataPoint);
DataPoint createDataPoint(char* arrOfStrings, int length);
void sumLists(Cluster cluster, DataPoint dataPoint);
int updateCentroids(Cluster* clusters, int k);
Cluster* kmean(int maxIterations, DataPoint* dataPointArr, Cluster* clusters, int dataPointSize, int k);
int countCommas(const char* arrOfStrings, int length);
void printCentroids(Cluster* finalClusters, int k);
int linesOfFileCounter(FILE* fptr, char* c);
int initializeDataPoints(FILE* fptr, char* c, DataPoint* dataPointArr);


int main(int argc, char *argv[]) {
    int k = atoi(argv[1]);
    int iterations = 200;
    if (argc == 3) {
        iterations = atoi(argv[2]);
    }
    char input[15];
    scanf("%s",input);
    
    char c[1000]; //fix later
    FILE *fptr;
    
    fptr = fopen(input, "r");
    
    if (fptr == NULL){
        printf("FILE NOT OPENED");
    } 
    int lineCounter = linesOfFileCounter(fptr, c);

    DataPoint* dataPointArr = (DataPoint *) calloc(lineCounter, sizeof(DataPoint));
    rewind(fptr);

    int dataPointSize = initializeDataPoints(fptr, c, dataPointArr);

    fclose(fptr);
    Cluster clusters[k];
    for (int j=0; j < k; j++) {
        clusters[j] = initializeClusters(dataPointArr, j);
    }

    Cluster* finalClusters = kmean(iterations, dataPointArr, clusters, dataPointSize, k);
    printCentroids(finalClusters, k);
}

int initializeDataPoints(FILE* fptr, char* c, DataPoint* dataPointArr) {
    int dataPointSize = 0;
    while (feof(fptr) == 0) {
        fgets(c, 1000, fptr);
        dataPointArr[dataPointSize] = createDataPoint(c, getSize(c));
        dataPointSize++;
    }
    return dataPointSize;
}


int linesOfFileCounter(FILE* fptr, char* c) {
    int lineCounter = 0;
    while(feof(fptr) == 0) {
        fgets(c, 1000, fptr);
        lineCounter++;
    }
    return lineCounter;
}

void printCentroids(Cluster* finalClusters, int k) {
    for (int a=0; a<k; a++) {
        //printf("a");
        for (int b=0; b<5; b++) {
            printf("%.4f  ", finalClusters[a].centroid.lst[b]);
        }
        printf("\n");
    }
}

Cluster* kmean(int maxIterations, DataPoint* dataPointArr, Cluster* clusters, int dataPointSize, int k) {
    int centroidsChanged = 0;
    int iterations = 0;
    int first = 1;
    while (iterations < maxIterations && (centroidsChanged > 0 || first)) {
        first = 0;
        iterations++;
        for (int i=0; i< dataPointSize; i++) {
            float minDistance = 9999999; // fix to max value
            int closestClusterIndex = -1;
            for (int j=0; j<k; j++) {
                float distance = euclidean(clusters[j].centroid, dataPointArr[i]);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestClusterIndex = j;
                }
            }
            clusters[closestClusterIndex].size++;
            sumLists(clusters[closestClusterIndex], dataPointArr[i]);
        }
        centroidsChanged = updateCentroids(clusters, k);
    }
    return clusters;
}

int updateCentroids(Cluster* clusters, int k) {
    int counter = 0;
    for (int i=0; i<k; i++) {
        // copy centroid lst
        float *prevCentroid = (float*) calloc(clusters[i].centroid.size, sizeof(float));
        for (int j=0; j<clusters[i].centroid.size; j++)
            prevCentroid[j] = clusters[i].centroid.lst[j];
        // averaging and initializing sumVector
        if (clusters[i].size != 0) {
            for (int m = 0; m < clusters[i].centroid.size; m++) {
                clusters[i].centroid.lst[m] = clusters[i].sumVector[m] / (float)clusters[i].size;
                clusters[i].sumVector[m] = 0;
            }
        }

        clusters[i].size = 0;
        for (int n=0; n<clusters[i].centroid.size; n++) {
            if (prevCentroid[n] != clusters[i].centroid.lst[n]) {
                counter++;
                break;
            }
        }
    }
    return counter;
}


void sumLists(Cluster cluster, DataPoint dataPoint) {
    for (int i=0; i<dataPoint.size; i++) {
        cluster.sumVector[i] += dataPoint.lst[i];
    }

}


float euclidean(Centroid centroid, DataPoint dataPoint) {
    float distance = 0;
    for (int i=0; i<centroid.size; i++)
        distance += (centroid.lst[i] - dataPoint.lst[i]) * (centroid.lst[i] - dataPoint.lst[i]);
    return distance;
}


Cluster initializeClusters(DataPoint* dataPointArr, int i) {
    float* lst = calloc(dataPointArr[i].size, sizeof(float));
    for (int j=0; j<dataPointArr[i].size; j++)
        lst[j] = dataPointArr[i].lst[j];
    Centroid centroid = {lst, dataPointArr[i].size }; // shallow copying?
    float* sumVector = (float*) calloc(dataPointArr[i].size, sizeof(float));
    Cluster cluster = {centroid, sumVector,0};
    return cluster;
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