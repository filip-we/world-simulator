/* main.c */
/*
 * Simulate formation of different minerals in a 2D environment.
 * Magma -> Igneous Intrusive/Extrusive
 * Any rock or other material -> sedimentary rock
 * Igneous/sedimentary -> Metamorphic
 *
 * set initial temperatures 
 * Set temperatures for given tiles (air and magma)
 * Iterate over other tiles and set their distance to another set temperature
 * update other temperatures
 * Matricies: material
 */

#include <stdio.h>
#include <gsl/gsl_blas.h>

#define WORLD_WIDTH 5 // Origo in top left corner, high up
#define WORLD_HEIGHT 5

const float transformsWeight = 0.2;

const int initialTemperature = 300;
const int magmaTemperature = 1000;

char materials[WORLD_HEIGHT][WORLD_WIDTH];
gsl_matrix *Temperature;
gsl_matrix *TransformH; // Horizontal
gsl_matrix *TransformV; // Vertical

const char MATERIAL_AIR = 'a';
const char MATERIAL_MAGMA = 'm';

static void init(){
    Temperature = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_matrix_set_all(Temperature, initialTemperature);

    TransformH = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_HEIGHT);
    gsl_matrix_set_zero(TransformH);
    for(size_t i = 0; i<WORLD_HEIGHT; i++){
        for(size_t j = 0; j<WORLD_HEIGHT; j++){
            if((j == i - 1) || (j == i + 1)){
                gsl_matrix_set(TransformH, i, j, 0.25);
            }
            if(j == i){
                gsl_matrix_set(TransformH, i, j, 0.5);
            }
        }
    }
    gsl_matrix_set(TransformH, 0, WORLD_HEIGHT - 1, 0.25);
    gsl_matrix_set(TransformH, WORLD_HEIGHT - 1, 0, 0.25);

    TransformV = gsl_matrix_alloc(WORLD_WIDTH, WORLD_WIDTH);
    gsl_matrix_set_zero(TransformV);
    for(size_t i = 0; i<WORLD_WIDTH; i++){
        for(size_t j = 0; j<WORLD_WIDTH; j++){
            if((j == i - 1) || (j == i + 1)){
                gsl_matrix_set(TransformV, i, j, 0.25);
            }
            if(j == i){
                gsl_matrix_set(TransformV, i, j, 0.5);
            }
        }
    }
    gsl_matrix_set(TransformV, 0, WORLD_WIDTH - 1, 0.25);
    gsl_matrix_set(TransformV, WORLD_WIDTH - 1, 0, 0.25);
}

void printMatrix(gsl_matrix *m){
    for(size_t i = 0; i<m->size1; i++){
        for(size_t j = 0; j<m->size2; j++){
            printf("%3g  ", gsl_matrix_get(m, i, j));
        }
        printf("\n");
    }
}

static void printArray(){
    for(int h=0; h<WORLD_HEIGHT; h++) {
        printf("\n");
        for(int w=0; w<WORLD_WIDTH; w++) {
            printf("%c", materials[h][w]);
        }
    }
}

static void updateTemperature(gsl_matrix *Temperature){
    gsl_matrix *result = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_matrix *result2 = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, TransformH, Temperature, 0.0, result);
    //printMatrix(result);
    //printf("\n");
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, result, TransformV, 0.0, result2);
    //printMatrix(result2);
    //printf("\n");
    gsl_matrix_memcpy(Temperature, result2);
}

int main(int argc, char *argv[]){
    init();

    for(int h=0; h<WORLD_HEIGHT; h++) {
        for(int w=0; w<WORLD_WIDTH; w++) {
            if(h>(WORLD_HEIGHT-5)) {
                materials[h][w] = MATERIAL_MAGMA;
            }
            else {
                materials[h][w] = MATERIAL_AIR;
            }
        }
    }
    printf("\nHeight: %ld,  Width: %ld\n", Temperature->size1, Temperature->size2);
    printArray(materials);
    printf("\n");

    // Some start data
    gsl_matrix_set(Temperature, 1, 2, 340);
    //gsl_matrix_set(Temperature, 0, 0, 340);

    for(int i=0; i <8; i++){
        printMatrix(Temperature);
        printf("\n");
        updateTemperature(Temperature);
    }
}
