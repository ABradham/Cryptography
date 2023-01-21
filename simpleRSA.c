/**
* simpleRSA.c - A simple terminal based 30 bit RSA implementation
*
* input:
*   - primelist.txt: A list of all prime numbers 15 bits or less
*   - p, q: The indicies of the primes in primeslist you want to use
*   - message: The message you want to encryopt
*
* output:
*
* compile: make
* usage: ./simpleRSA
*
* Alphonso Bradham, Winter 2023
*
*/

#include <stdio.h>
#include <stdlib.h>

#define NUM_PRIMES 3432
/**
 * Read primes from file and save to int[]
 * @param filename: char* containing the name of the file you want to read from
 * @param output: int[] where you want the read primes to go
 * @param count: int representing the max size of your output int[]
*/
void read_primes_from_file(char* filename, unsigned int* output, int count){
    // Open the file
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        printf("Could not open file with primes :( \n");
        exit(1);
    }

    // Read line by line, ignoring non-integer characters, and add ints to output
    for(int i = 0; i < count; i++){
        if(fscanf(file, "%d", &output[i]) != 1){
            if(feof(file)){
                printf("Reached the end of the file before reading all %d primes.\n", count);
                return;
            }
        }
    }

    // Close the file
    if(fclose(file) != 0){
        printf("Error closing the file!\n");
    }
}

int main(){
    // Create array to hold all the primes (we'll say there's 5000 for now)
    unsigned int* all_primes = calloc(NUM_PRIMES, sizeof(int));
    read_primes_from_file("primelist.txt", all_primes, NUM_PRIMES);
    
    // Ask user for indices of p and q
    int p, q, N, phi;
    printf("Enter indicies of two primes: \n");
    scanf("%d", &p);
    scanf("%d", &q);
    N = p*q;

    printf("p = %d, q = %d\n", p, q);

    // Free memory used for primes list
    free(all_primes);
    return 0;
}