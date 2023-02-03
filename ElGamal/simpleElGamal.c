/**
* simpleElGamal.c - A simple terminal based 30 bit ElGamal implementation
*
* input:
*
* output:
*
* compile: make
* usage: ./simpleElGamal
*
* Alphonso Bradham, Winter 2023
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Pairs of pre-selected primes and primitives
#define P_1 892086827
#define G_1 2

#define P_2 750443147
#define G_2 2


/**
 * A structure to hold the value of an ElGamal encrypted message
*/
typedef struct ElGamalMessage_t{
    unsigned long long c_1;
    unsigned long long c_2;
} ElGamalMessage;

/**
 * A function to generate a random integer in a specified range
 * @param lowest: unsigned int lower bound of possible numbers
 * @param highest: unsigned int upper bound of possible numbers
*/
unsigned long long random_in_range(unsigned long long lowest, unsigned long long highest){
    unsigned long long chosen_num = lowest + rand() % (highest - lowest);
    return chosen_num;
}

/**
 * A function to find the value `d` in the expression de=1 mod phi
 * @param e: int public key
 * @param phi: private primes p and q arranged as (p-1)(q-1)
 * @return unsigned int private key `d` (multiplicative inverse of e mod phi)
*/
unsigned long long extended_euclidean(unsigned long long e, unsigned long long phi){
    unsigned long long q, a, b, r;
    int t_1, t_2, t;

    a = phi;
    b = e;
    q = a / b;
    r = a % b;
    t_1 = 0;
    t_2 = 1;
    t = t_1 - t_2 * q;
    while(b != 0){
        // Check if b  == 0
        // Do swapping
        a = b;
        b = r;
        t_1 = t_2;
        t_2 = t;

        if(b == 0){
            // Prevents divide by 0 errors
            if(t_1 < 0)
                return t_1 + phi;
            return t_1;            
        }

        // Fill out rest of table
        q = a / b;
        r = a % b;
        t = t_1 - (t_2 * q);
    }
    if(t_1 < 0)
        return t_1 + phi;
    return t_1;
}

unsigned long long exponentiate(unsigned long long a, unsigned long long b){
    for(int i = 0; i < b; i++){
        printf("Exponentiated A: %lld\n", a);
        a *= a;
    }
    return a;
}

/**
 * A function to perfom modular exponentiation of the form a^x mod b.
 * @paragraph This function works by sucessively squaring a and taking 
 *            the mod b until x (the exponent of interest) is depleted.
 *            In the case where x is odd, we begin by simply multiplying
 *            the result variable by x (as opposed to squaring it).
 * @param a: long int representing the number you want to exponentiate
 * @param x: int representing the power you want to raise `a` to
 * @param b: int representing the modular field you want to stay in
 * @return: unsigned int result of modular exponentiation
*/
unsigned long long modular_exponentiation(long long a, unsigned long long x, unsigned long long b){
    unsigned long long result = 1;
    a = a % b;
    while(x > 0){
        // If exponent is odd, multiply by A once and take mod B
        if(x % 2 != 0)
            result = (result * a) % b;
        
        // Square A (since exponent is now even)
        a = (a*a) % b;

        // Decrement exponent by 1/2
        x  /= 2;
    }

    return result % b;
}

// unsigned int modular_exponentiation_with_p(long long a, unsigned int x, unsigned int b, unsigned int p){
//     unsigned int result = 1;
//     a = a % b;
//     while(x > 0){
//         // If exponent is odd, multiply by A once and take mod B
//         if(x % 2 != 0)
//             result = (result * a) % b;
        
//         // Square A (since exponent is now even)
//         a = (a*a) % b;

//         // Decrement exponent by 1/2
//         x  /= 2;
//     }
//     result = result % b;
//     printf("Mod Exp result: %d\n", result);
//     printf("P: %d\n", p);
//     result *= p;
//     printf("Mod Exp result * pt: %d\n", result);
//     printf("Mod Exp result * pt mod p: %d\n", result % b);
//     return result % b;
// }


/**
 * A function to calculate a suitable value y for the ElGamal public key.
 * 
 * @param x: unsigned int containing the value of the private key
 * @param p: unsigned int containing prime number part of the public key
 * @param g: unsigned int containing `primitive element` of p
 * @return unsigned int y suitable as an ElGamal private key
*/
unsigned long long find_y(unsigned long long x, unsigned long long p, unsigned long long g){
    return modular_exponentiation(g, x, p);
}


/**
 * A function to perfom ElGamal encryption on a plaintext int.
 * @param pt: unsigned int representing the plaintext number to encrypt.
 * @param g: unsigned int representing the first part of the public key
 * @param p: unsigned int representing second part of the public key
 * @return: unsigned int result of encryption
*/
void ElGamal_encrypt(unsigned long long pt, unsigned long long y, ElGamalMessage* message){
    // Generate a random number k
    unsigned long long k = random_in_range(2, 100);
    
    // Generate first part of ElGamal ciphertext
    unsigned long long c_1 = modular_exponentiation(G_1, k, P_1);

    // Generate second part of ElGamal ciphertext
    unsigned long long c_2 = modular_exponentiation(y, k, P_1);
    c_2 *= pt;
    c_2 = c_2 % P_1;

    // Save message into object
    message->c_1 = c_1;
    message->c_2 = c_2;

}

unsigned long long ElGamal_decrypt(ElGamalMessage* message, unsigned long long x){
    unsigned long long c_1_prime = extended_euclidean(modular_exponentiation(message->c_1, x, P_1), P_1);
    unsigned long long product = c_1_prime * message->c_2;
    return product % P_1;
}

int main(){
    unsigned long long x;
    printf("Choose a private key 2 <= x < %d\n", P_1-1);
    scanf("%lld", &x);
    
    // Compute public key y
    unsigned long long y = find_y(x, P_1, G_1);

    // Allocate memory for encrypted message
    ElGamalMessage* message = calloc(1, sizeof(ElGamalMessage));

    // Encrypt message and save into memory
    ElGamal_encrypt(3321321, y, message);
    printf("Encrypted Message C_1: %lld, C_2: %lld\n", message->c_1, message->c_2);

    // Decrypt message
    unsigned long long decrypted = ElGamal_decrypt(message, x);
    printf("Decrypted Message: %lld\n", decrypted);
    
    // Free message memory
    free(message);

    return 0;
}