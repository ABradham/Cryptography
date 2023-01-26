/**
* simpleRSA.c - A simple terminal based 30 bit RSA implementation
*
* input:
*   - primelist.txt: A list of all prime numbers 15 bits or less
*   - p, q: The indicies of the primes that you want to use
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
#include <stdbool.h>

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

/**
 * A function to find if two given numbers (lower and higher) are coprime
 * @param lower: the lower number whose coprimality you want to test
 * @param higher: the higher number whose coprimality you want to test
*/
bool is_coprime(unsigned int lower, unsigned int higher){
    for(int i = 2; i <= lower; i++){
        if(lower % i == 0 && higher % i == 0)
            return false;
    }
    return true;
}

/**
 * A function to find some value `e` that is coprime to (q-1)(p-1)
 * @param filename: char* containing the name of the file you want to read from
 * @param q: first prime int used to crate private key
 * @param p: second prime int used to create private key
 * @return int that is coprime with (q-1)(p-1)
*/
unsigned int find_e(unsigned int phi){
    // Randomly select an E and check for coprimality
    unsigned int chosen_E = 3 + rand() % (phi - 3);
    while(!is_coprime(chosen_E, phi)){
        chosen_E = 3 + rand() % (phi - 3);
    }
    return chosen_E;
}

/**
 * A function to find the value `d` in the expression de=1 mod phi
 * @param e: int public key
 * @param phi: private primes p and q arranged as (p-1)(q-1)
 * @return unsigned int private key `d` (multiplicative inverse of e mod phi)
*/
unsigned int extended_euclidean(unsigned int e, unsigned int phi){
    unsigned int q, a, b, r;
    int t_1, t_2, t;

    a = phi;
    b = e;
    q = a / b;
    r = a % b;
    t_1 = 0;
    t_2 = 1;
    t = t_1 - t_2 * q;
    // printf("Q: %d; A: %d; B: %d; R: %d; T_1: %d; T_2: %d; T: %d\n", q, a, b, r, t_1, t_2, t);
    while(b != 0){
        // Check if b  == 0
        // Do swapping
        a = b;
        b = r;
        t_1 = t_2;
        t_2 = t;

        // Fill out rest of table
        q = a / b;
        r = a % b;
        t = t_1 - (t_2 * q);

        // printf("Q: %d; A: %d; B: %d; R: %d; T_1: %d; T_2: %d; T: %d\n", q, a, b, r, t_1, t_2, t);
    }
    if(t_1 < 0)
        return t_1 + phi;
    return t_1;
}

int main(){
    // Create array to hold all the primes (we'll say there's 5000 for now)
    unsigned int* all_primes = calloc(NUM_PRIMES, sizeof(int));
    read_primes_from_file("primelist.txt", all_primes, NUM_PRIMES);
    
    // Ask user for indices of p and q
    unsigned int p, q, N, e, phi, d;
    printf("Enter indicies of two primes: \n");
    scanf("%d", &p);
    scanf("%d", &q);
    if(p == q){
        printf("You cannot choose equivalent primes!\n");
        exit(1);
    }
    p = all_primes[p];
    q = all_primes[q];
    N = p*q;
    phi = (p-1)*(q-1);
    e = find_e(phi);
    d = extended_euclidean(e, phi);

    printf("p = %d, q = %d\n", p, q);

    printf("e: %d\n", e);

    printf("phi: %d\n", phi);

    printf("d: %d\n", d);

    // Free memory used for primes list
    free(all_primes);
    return 0;
}