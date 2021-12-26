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

#define WORLD_HEIGHT 16
#define WORLD_WIDTH 20 // Origo in top left corner, high up

#define MATERIAL_AIR 1
#define MATERIAL_MAGMA 2

gsl_matrix *Material;
int staticTemperatures[10];
gsl_vector *boundaryTemperatureH0;
gsl_vector *boundaryTemperatureHMax;

const float transformsWeight = 0.2;

gsl_matrix *Temperature;
gsl_matrix *TransformH; // Horizontal
gsl_matrix *TransformV; // Vertical

static void initTemperature(){
    staticTemperatures[0] = 0;
    staticTemperatures[MATERIAL_AIR] = 300;
    staticTemperatures[MATERIAL_MAGMA] = 1500;

    boundaryTemperatureHMax = gsl_vector_alloc(WORLD_WIDTH);
    boundaryTemperatureH0 = gsl_vector_alloc(WORLD_WIDTH);
    gsl_vector_set_all(boundaryTemperatureHMax, staticTemperatures[MATERIAL_MAGMA]);
    gsl_vector_set_all(boundaryTemperatureH0, staticTemperatures[MATERIAL_AIR]);

    Temperature = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_matrix_set_all(Temperature, staticTemperatures[MATERIAL_AIR]);

    // Heat spreading tranform matices
    TransformH = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_HEIGHT);
    TransformV = gsl_matrix_alloc(WORLD_WIDTH, WORLD_WIDTH);
    gsl_matrix_set_zero(TransformH);
    gsl_matrix_set_zero(TransformV);
    for(size_t i = 0; i<WORLD_HEIGHT; i++){
        for(size_t j = 0; j<WORLD_HEIGHT; j++){
            if((j == i - 1) || (j == i + 1)){
                gsl_matrix_set(TransformH, i, j, 0.25);
            }
            if(j == i){
                gsl_matrix_set(TransformH, i, j, 0.5);
            }
            if((j == i - 1) || (j == i + 1)){
                gsl_matrix_set(TransformV, i, j, 0.25);
            }
            if(j == i){
                gsl_matrix_set(TransformV, i, j, 0.5);
            }

            switch ((int) gsl_matrix_get(Material, i, j)){
                case MATERIAL_MAGMA:
                    gsl_matrix_set(Temperature, i, j, staticTemperatures[MATERIAL_MAGMA]);
                    break;
                default:
                    gsl_matrix_set(Temperature, i, j, staticTemperatures[MATERIAL_AIR]);
            }
        }
    }
    gsl_matrix_set(TransformH, 0, WORLD_HEIGHT - 1, 0.25);
    gsl_matrix_set(TransformH, WORLD_HEIGHT - 1, 0, 0.25);
    gsl_matrix_set(TransformV, 0, WORLD_WIDTH - 1, 0.25);
    gsl_matrix_set(TransformV, WORLD_WIDTH - 1, 0, 0.25);
}

static void initMaterial(){
    Material = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_matrix_set_all(Material, MATERIAL_AIR);
    for(int h=0; h<WORLD_HEIGHT; h++) {
        for(int w=0; w<WORLD_WIDTH; w++) {
            if(h > 1) {
                gsl_matrix_set(Material, h, w, MATERIAL_MAGMA);
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

static void updateTemperature(gsl_matrix *Temperature){
    gsl_matrix *result = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_matrix *result2 = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, TransformH, Temperature, 0.0, result);
    //printMatrix(result);
    //printf("\n");
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, result, TransformV, 0.0, result2);
    //printMatrix(result2);
    //printf("\n");
    gsl_matrix_set_row(result2, 0, boundaryTemperatureH0);
    gsl_matrix_set_row(result2, WORLD_HEIGHT - 1, boundaryTemperatureHMax);
    gsl_matrix_memcpy(Temperature, result2);
}

int main(int argc, char *argv[]){
    initMaterial();
    initTemperature();

    printf("\nWorld info: Height=%ld,  Width=%ld\n\n", Temperature->size1, Temperature->size2);

    // Some start data
    gsl_matrix_set(Temperature, 1, 2, 340);

    for(int i=0; i <8; i++){
        printMatrix(Temperature);
        printf("\n");
        updateTemperature(Temperature);
    }
    printf("\n");
    printMatrix(Material);
}
