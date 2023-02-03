#include <stdio.h>
#include <stdlib.h>
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


    FILE* pemfile = fopen("key.pem", "r");

    dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, format, structure,
                                         keytype,
                                         OSSL_KEYMGMT_SELECT_KEYPAIR,
                                         NULL, NULL);

    OSSL_DECODER_from_fp(dctx, pemfile);

    EVP_PKEY_get_bn_param(pkey, "n", &mod);

    EVP_PKEY_get_bn_param(pkey, "e", &e);

    // EVP_PKEY_print_private_fp(stdout, pkey, 0, NULL);
    // EVP_PKEY_print_params_fp(stdout, pkey, 0, NULL);
    // EVP_PKEY_print_public_fp(stdout, pkey, 0, NULL);
    EVP_PKEY_get_bn_param(pkey, "e", &e);

    printf("E:");
    BN_print_fp(stdout, e);
    printf("\n");

    printf("N:");
    BN_print_fp(stdout, mod);
    printf("\n");

    EVP_PKEY_get_bn_param(pkey, "q", &q);
    printf("q:");
    BN_print_fp(stdout, q);
    printf("\n");
}