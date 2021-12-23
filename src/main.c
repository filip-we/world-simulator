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

#define WORLD_WIDTH 32
#define WORLD_HEIGHT 16
const int startTemperature = 20;
const int magmaTemperature = 99;

char materials[WORLD_HEIGHT][WORLD_WIDTH];
gsl_matrix *Temperature;

const char MATERIAL_AIR = 'a';
const char MATERIAL_MAGMA = 'm';

static void init(){
    Temperature = gsl_matrix_alloc(WORLD_HEIGHT, WORLD_WIDTH);
    for(size_t i = 0; i<Temperature->size1; i++){
        for(size_t j = 0; j<Temperature->size2; j++){
            gsl_matrix_set(Temperature, i, j, startTemperature);
        }
    }
    //for(size_t j = 0; j<Temperature->size2; j++){
    //    gsl_matrix_set(Temperature, WORLD_HEIGHT-1, j, magmaTemperature);
    //}
}

void printMatrix(gsl_matrix *m){
    for(size_t i = m->size1-1; i>0; i--){
        for(size_t j = 0; j<m->size2; j++){
            printf("%3g  ", gsl_matrix_get(m, i, j));
        }
        printf("\n");
    }
}

static void updateTemperature(){
    gsl_matrix temp;
    //gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Temperature, A2, 0.0, temp);

}

static void printArray(){
    for(int h=WORLD_HEIGHT - 1; h>=0; h--) {
        printf("\n");
        for(int w=0; w<WORLD_WIDTH; w++) {
            printf("%c", materials[h][w]);
        }
    }
}

int main(int argc, char *argv[]){
    init();

    for(int h=0; h<WORLD_HEIGHT; h++) {
        for(int w=0; w<WORLD_WIDTH; w++) {
            if(h<5) {
                materials[h][w] = MATERIAL_MAGMA;
            }
            else {
                materials[h][w] = MATERIAL_AIR;
            }
        }
    }
    printf("Printing matrix\n");
    printMatrix(Temperature);
}

void test(){
    printf("\n");
    printArray();

    gsl_matrix *A1;
    gsl_matrix *A2;
    gsl_matrix *R;
    A1 = gsl_matrix_alloc(2, 2);
    A2 = gsl_matrix_alloc(2, 2);
    R = gsl_matrix_alloc(2, 2);

    gsl_matrix_set(A1, 0, 0, 1);
    gsl_matrix_set(A1, 0, 1, 2);
    gsl_matrix_set(A1, 1, 0, 3);
    gsl_matrix_set(A1, 1, 1, 4);

    gsl_matrix_set(A2, 0, 0, 1);
    gsl_matrix_set(A2, 0, 1, 2);
    gsl_matrix_set(A2, 1, 0, 3);
    gsl_matrix_set(A2, 1, 1, 4);

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A1, A2, 0.0, R);
    printf("\n");
    //printMatrix(R);
}

