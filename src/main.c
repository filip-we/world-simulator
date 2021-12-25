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

#define WORLD_WIDTH 32 // Origo in top left corner, high up
#define WORLD_HEIGHT 16
#define transformNr 2
const float transformsWeight = 0.2;

const int startTemperature = 20;
const int magmaTemperature = 99;

char materials[WORLD_HEIGHT][WORLD_WIDTH];
gsl_matrix *Temperature;
gsl_matrix *Transforms[4]; // Left, up, right, down

const char MATERIAL_AIR = 'a';
const char MATERIAL_MAGMA = 'm';

static void init(){
    Temperature = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_matrix_set_all(Temperature, 0.0);
    gsl_matrix_set(Temperature, 3, 11, startTemperature);
    gsl_matrix_set(Temperature, 0, 0, startTemperature);

    Transforms[0] = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_HEIGHT);
    gsl_matrix_set_all(Transforms[0], 0.0);
    Transforms[1] = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_HEIGHT);
    gsl_matrix_set_all(Transforms[1], 0.0);

    for(size_t i = 0; i<WORLD_HEIGHT; i++){
        for(size_t j = 0; j<WORLD_HEIGHT; j++){
            if(j == i - 1){
                gsl_matrix_set(Transforms[0], i, j, transformsWeight);
            }
            if(j == i + 1){
                gsl_matrix_set(Transforms[1], i, j, transformsWeight);
            }
        }
    }
   Transforms[2] = gsl_matrix_alloc(WORLD_WIDTH, WORLD_WIDTH);
   gsl_matrix_set_all(Transforms[2], 0.0);
   Transforms[3] = gsl_matrix_alloc(WORLD_WIDTH, WORLD_WIDTH);
   gsl_matrix_set_all(Transforms[3], 0.0);

    for(size_t i = 0; i<WORLD_WIDTH; i++){
        for(size_t j = 0; j<WORLD_WIDTH; j++){
            if(j == i - 1){
                gsl_matrix_set(Transforms[2], i, j, transformsWeight);
            }
            if(j == i + 1){
                gsl_matrix_set(Transforms[3], i, j, transformsWeight);
            }
        }
    }
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
    gsl_matrix *temp  = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_matrix *result = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_matrix_memcpy(result, Temperature);
    gsl_matrix_scale(result, transformsWeight);

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Transforms[0], Temperature, 0.0, temp);
    gsl_matrix_add(result, temp);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Transforms[1], Temperature, 0.0, temp);
    gsl_matrix_add(result, temp);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Temperature, Transforms[2], 0.0, temp);
    gsl_matrix_add(result, temp);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Temperature, Transforms[3], 0.0, temp);
    gsl_matrix_add(result, temp);

    gsl_matrix_memcpy(Temperature, result);
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

    for(int i=0; i <4; i++){
        updateTemperature(Temperature);
        printMatrix(Temperature);
        printf("\n");
    }
}
