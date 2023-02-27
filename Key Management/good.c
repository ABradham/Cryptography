#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <openssl/decoder.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/core_names.h>

BIGNUM* good(BIGNUM* input){
    // Get user input
    printf("Enter the name of the keyfile:\n");
    char fileName[128];
    scanf("%s", fileName);
    
    OSSL_DECODER_CTX *dctx;
    EVP_PKEY *pkey = NULL;
    const char *format = "PEM";   /* NULL for any format */
    const char *structure = NULL; /* any structure */
    const char *keytype = "RSA";  /* NULL for any key */
    BIGNUM* n = NULL;
    BIGNUM* d = NULL;


    FILE* pemfile = fopen(fileName, "r");
    if(pemfile == NULL){
        printf("Could not open designated keyfile!\n");
        exit(0);
    }

    dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, format, structure,
                                         keytype,
                                         OSSL_KEYMGMT_SELECT_KEYPAIR,
                                         NULL, NULL);

    OSSL_DECODER_from_fp(dctx, pemfile);

    EVP_PKEY_get_bn_param(pkey, "d", &d);
    printf("D From Key.Pem: %s\n\n", BN_bn2dec(d));

    EVP_PKEY_get_bn_param(pkey, "n", &n);

    // Modular Exponentiation
    BN_CTX* ctx =  BN_CTX_new();
    BIGNUM*  output = BN_new();
    BN_mod_exp(output, input, d, n, ctx);
    // printf("Answer: %s\n", BN_bn2dec(output));
    return output;
}

void evil(){
    char someLocalEvil[128];
    BIGNUM* d;
    BIGNUM* of_interest_2;
    const char *format;   /* NULL for any format */
    const char *structure; /* any structure */
    const char *keytype;  /* NULL for any key */
    BIGNUM* n;
    BIGNUM* of_interest;
    BN_CTX* ctx_2;
    BIGNUM* output;

    printf("D From Evil(): %s\n", BN_bn2dec(d));
    fprintf(stderr, "\n--------OTHER CAPTURED / UNINITIALIZED ADDRESSES--------\n");
    fprintf(stderr, "%p\n", someLocalEvil);
    fprintf(stderr, "%p\n", of_interest_2);
    fprintf(stderr, "%p\n", format);
    fprintf(stderr, "%p\n", structure);
    fprintf(stderr, "%p\n", keytype);
    fprintf(stderr, "%p\n", keytype);
    fprintf(stderr, "%p\n", of_interest);
    fprintf(stderr, "%p\n", ctx_2);
    fprintf(stderr, "%p\n", output);
}

int main(){
    BIGNUM* input = BN_new();
    BN_dec2bn(&input, "2");
    BIGNUM* output = good(input);

    evil();
}