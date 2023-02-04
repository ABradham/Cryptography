/**
* simpleElGamal.c - A simple terminal based 30 bit ElGamal implementation
*
* compile: make
* usage: ./simpleElGamal
*
* Alphonso Bradham, Winter 2023
*
* ------------------------------[[INSTRUCTIONS]]------------------------------
* To run this program in a way that showcases the "Mental Poker" El Gamal scheme,
* set #define POKER 1. To get an interactive terminal for encrypting and decrypting
* arbitrary messages with El Gamal, set #define POKER 0. For the Mental Poker example,
* code in the `void mental_poker()` function explains how a deck is shuffled and read
* from without any player (Alice or Bob) knowing which cards the other player has drawn.
* ------------------------------[[END INSTRUCTIONS]]------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Pairs of pre-defined primes and primitives
#define P_1 892086827
#define G_1 2

#define P_2 750443147
#define G_2 2

#define POKER 1

/** 
 * @brief A structure to hold the value of an ElGamal encrypted message.
 * C_1: unsigned long long holding the C_1 field of the El Gamal Message
 * C_2: unsigned long long holding the C_2 field of the El Gamal Message
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
 * @param e: number whose inverse you want to find
 * @param phi: number whose field your in (the number you are MODing (%) by)
 * @return multiplicative inverse of e mod phi
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
 * @param y: unsigned int representing the first part of the public key
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

/**
 * A function to perfom ElGamal encryption on a plaintext int given a K value.
 * @param pt: unsigned int representing the plaintext number to encrypt.
 * @param g: unsigned int representing the first part of the public key
 * @param p: unsigned int representing second part of the public key
 * @param k: unsigned long long representing random k value for encryption
 * @return: unsigned int result of encryption
*/
void ElGamal_encrypt_with_k(unsigned long long pt, unsigned long long y, unsigned long long k,ElGamalMessage* message){
    
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

/**
 * A function to perfom ElGamal decryption on an ElGamalMessage struct.
 * @param message: An ElGamalMessage* containing the encrypted message.
 * @param x: Unisigned long long El Gamal private key
 * @return: Result of decryption
*/
unsigned long long ElGamal_decrypt(ElGamalMessage* message, unsigned long long x){
    unsigned long long c_1_prime = extended_euclidean(modular_exponentiation(message->c_1, x, P_1), P_1);
    unsigned long long product = c_1_prime * message->c_2;
    return product % P_1;
}

unsigned long long ElGamal_decrypt_parts(unsigned long long c_1, unsigned long long c_2, unsigned long long x){
    unsigned long long c_1_prime = extended_euclidean(modular_exponentiation(c_1, x, P_1), P_1);
    unsigned long long product = c_1_prime * c_2;
    return product % P_1;
}


void mental_poker(){
    // Bob's private key x, public key y, and random number k
    unsigned long long bob_x = 15;
    unsigned long long bob_y = find_y(bob_x, P_1, G_1);
    unsigned long long bob_k = 7;

    // Alice's private key x, public key y, and list of random numbers k
    unsigned long long alice_x = 20;
    unsigned long long alice_y = find_y(alice_x, P_1, G_1);
    unsigned long long alice_ks[16] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

    // Bob's initial card shuffle
    unsigned long long Bob_shuffle[16] = {13, 17, 4, 6, 14, 2, 12, 15, 5, 3, 7, 10, 8, 11, 16, 9};
    printf("Bob's starting shuffle: {13, 17, 4, 6, 14, 2, 12, 15, 5, 3, 7, 10, 8, 11, 16, 9}\n\n");

    // Bob's encrypted card shuffle (all cards are now encrypted ElGamalMessage*s)
    //ElGamalMessage** messages = calloc(16, sizeof(ElGamalMessage));
    ElGamalMessage** messages = calloc(16, sizeof(ElGamalMessage*));
    for(int i = 0; i<16; i++){
        ElGamalMessage* new_message = malloc(sizeof(ElGamalMessage));
        ElGamal_encrypt_with_k(Bob_shuffle[i], bob_y, bob_k, new_message);
        messages[i] = new_message;
    }
    printf("Encrypted Bob's deck!\n");

    // Alice does her own round of encryption
    ElGamalMessage** alice_messages = calloc(16, sizeof(ElGamalMessage*));
    for(int i = 0; i<16; i++){
        ElGamalMessage* new_message = malloc(sizeof(ElGamalMessage));
        ElGamal_encrypt_with_k(messages[i]->c_2, alice_y, alice_ks[i], new_message);
        alice_messages[i] = new_message;
    }
    printf("Encrypted Alice's deck!\n");

    // Alice shuffles the deck using Fisher-Yates (https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle)
    for (int i = 0; i < 16-1; ++i){
        int j = rand() % (16-i) + i;

        ElGamalMessage* temp_M = messages[i];
        ElGamalMessage* temp_AM = alice_messages[i];

        messages[i] = messages[j];
        messages[j] = temp_M;

        alice_messages[i] = alice_messages[j];
        alice_messages[j] = temp_AM;
    }
    printf("Shuffled Alice's deck!\n\n");


    /**
     * 1. Bob chooses card 3. 
     * 2. Alice decrypts using her private key and passes the result to Bob.
     *    Since the card was encrypted by Bob when Alice originally got it,
     *    Alice only sees the result of Bob's encryption after decrypting the card for Bob.
     * 3. Bob decrypts the card using his private key and reads off the value!
    */
    printf("----Bob chooses card #3----\n");
    unsigned long long res1 = ElGamal_decrypt_parts(alice_messages[3]->c_1, alice_messages[3]->c_2, alice_x);
    printf("After asking Alice to decrypt card 3 with her private key, the result is %lld\n", res1);

    printf("Finally, Bob decrypts the result of Alice's decryption to find that....\n");
    unsigned long long res2 = ElGamal_decrypt_parts(messages[3]->c_1, res1, bob_x);
    printf("3rd card is: %lld\n\n", res2);

    /**
     * 1. Alice chooses card 7. 
     * 2. Bob decrypts using his private key and passes the result to Alice.
     *    Since the card was encrypted by Alice and the deck was shuffled, Bob
     *    has no clue which card he has (partially) decrypted for alice.
     * 3. Alice decrypts the card using her private key and reads off the value!
    */
    printf("----Alice chooses card #7----\n");
    res1 =  ElGamal_decrypt_parts(messages[7]->c_1, alice_messages[7]->c_2, bob_x);
    printf("After asking Bob to decrypt card 3 with his private key, the result is %lld\n", res1);

    printf("Finally, Alice decrypts the result of Bob's decryption to find that....\n");
    res2 = ElGamal_decrypt_parts(alice_messages[7]->c_1, res1, alice_x);
    printf("7th card is: %lld\n", res2);
}

int main(){
    if(POKER){
        mental_poker();
    }else{
        unsigned long long x;
        printf("Choose a private key 2 <= x < %d\n", P_1-1);
        scanf("%lld", &x);
        
        // Compute public key y
        unsigned long long y = find_y(x, P_1, G_1);

        // Allocate memory for encrypted message
        ElGamalMessage* message = calloc(1, sizeof(ElGamalMessage));

        // Encrypt message and save into memory
        unsigned long long plaintext;
        while(1){
            printf("Enter a message (must be a positive NUMBER < %d) to encrypt [Choose 0 to exit]: \n", P_1);
            scanf("%lld", &plaintext);

            if(plaintext == 0)
                break;
            printf("\nPlaintext: %lld\n", plaintext);

            ElGamal_encrypt(plaintext, y, message);
            printf("Encrypted Message C_1: %lld, C_2: %lld\n", message->c_1, message->c_2);

            // Decrypt message
            unsigned long long decrypted = ElGamal_decrypt(message, x);
            printf("Decrypted Message: %lld\n\n", decrypted);
        }
        
        // Free message memory
        printf("Exiting program!\n");
        free(message);
    }

    return 0;
}