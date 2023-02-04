#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <openssl/decoder.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/core_names.h>

int main(){
    OSSL_DECODER_CTX *dctx;
    EVP_PKEY *pkey = NULL;
    const char *format = "PEM";   /* NULL for any format */
    const char *structure = NULL; /* any structure */
    const char *keytype = "RSA";  /* NULL for any key */
    BIGNUM* mod = NULL;
    BIGNUM* e = NULL;
    BIGNUM* q = NULL;
    BIGNUM* p = NULL;
    BIGNUM* d = NULL;


    FILE* pemfile = fopen("key.pem", "r");

    dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, format, structure,
                                         keytype,
                                         OSSL_KEYMGMT_SELECT_KEYPAIR,
                                         NULL, NULL);

    OSSL_DECODER_from_fp(dctx, pemfile);

    EVP_PKEY_get_bn_param(pkey, "e", &e);
    printf("E:");
    BN_print_fp(stdout, e);
    printf("\n");

    EVP_PKEY_get_bn_param(pkey, "d", &d);
    printf("d:");
    BN_print_fp(stdout, d);
    printf("\n");

    EVP_PKEY_get_bn_param(pkey, "n", &mod);
    printf("N:");
    BN_print_fp(stdout, mod);
    printf("\n");

    EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_RSA_FACTOR1, &q);
    printf("q:");
    BN_print_fp(stdout, q);
    printf("\n");

    EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_RSA_FACTOR2, &p);
    printf("p:");
    BN_print_fp(stdout, p);
    printf("\n\n");

    printf("Verifying if p*q = n\n");
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* res = BN_new();
    BN_mul(res, p, q, ctx);
    if(strcmp(BN_bn2hex(res), BN_bn2hex(mod)) == 0){
        printf("P and Q are equal!\n\n");
    }

    printf("Verifying de = 1 mod(p-1)(q-1)\n");

    // Calc DE
    BN_CTX* ctx2 = BN_CTX_new();
    BIGNUM* de = BN_new();
    BN_mul(de, d, e, ctx2);

    // Calc P-1 and Q-1
    BIGNUM* one = BN_new();
    BN_one(one);
    
    BIGNUM* p_1 = BN_new();
    BN_sub(p_1, p, one);

    BIGNUM* q_1 = BN_new();
    BN_sub(q_1, q, one);

    // Calc (P-1)(Q-1)
    BIGNUM* phi = BN_new();
    ctx2 = BN_CTX_new();
    BN_mul(phi, p_1, q_1, ctx2);

    // Check that de mod phi = 1
    ctx2 = BN_CTX_new();
    res = BN_new();
    BN_mod(res, de, phi, ctx2);

    if(strcmp(BN_bn2hex(res), BN_bn2hex(one)) == 0){
        printf("de mod (p-1)(q-1) is 1!\n");
    }
    
}