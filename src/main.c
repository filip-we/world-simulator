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
#include <gsl/gsl_sf_bessel.h>

#define WORLD_WIDTH 128
#define WORLD_HEIGHT 32

char materials[WORLD_WIDTH][WORLD_HEIGHT];
float temperatures[WORLD_WIDTH][WORLD_HEIGHT];

const char MATERIAL_AIR = 'a';
const char MATERIAL_MAGMA = 'm';

static void printArray(){
    for(int h=WORLD_HEIGHT - 1; h>=0; h--) {
        printf("\n");
        for(int w=0; w<WORLD_WIDTH; w++) {
            printf("%c", materials[w][h]);
        }
    }
}

static void updateTemperature(){
}

int main(int argc, char *argv[]){
    for(int h=0; h<WORLD_HEIGHT; h++) {
        for(int w=0; w<WORLD_WIDTH; w++) {
            if(h<5) {
                materials[w][h] = MATERIAL_MAGMA;
            }
            else {
                materials[w][h] = MATERIAL_AIR;
            }
        }
    }
    printf("\n");
    printArray();
}

