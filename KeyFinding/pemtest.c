#include <stdio.h>
#include <openssl/dsa.h>
#include <openssl/pem.h>
#include <openssl/decoder.h>
#include <openssl/encoder.h>
#include <openssl/param_build.h>
#include <openssl/evp.h>
#include <openssl/core_names.h>

/*
  Sample code to:
    - read in a dsa pem file
    - examine the parameters
    - create a new dsa keypair based on that, but with a new private key
  ...all using the (imho) overly complicated non-deprecated calls in openssl :(

  use:  
     dsatest1 infile.pem
  or dsatest1 infile.pem outfile.pem

  sws, feb 2023

 */


#define MSG_1_HASH "D0A7827AC24B1C7B2BFA8AE59DE893DFFF0573E9"
#define MSG_1_R "6D229F0BC85CAF289E9BF14EAABF8090E77C4254"
#define MSG_1_S "CC518760E82515CBE127BB381E877CBF3080029D"

#define MSG_2_HASH "C0FA01E4E813E42131F49D514183AA901BDC4C0A"
#define MSG_2_R "6D229F0BC85CAF289E9BF14EAABF8090E77C4254"
#define MSG_2_S "CC01872EEB2D601D4572F421CE703FAA61B3557D"

BIGNUM* calc_k(BIGNUM* h_1, BIGNUM* s_1, BIGNUM* h_2, BIGNUM* s_2){
  BIGNUM* hash_sub_res = BN_new();
  BN_sub(hash_sub_res, h_1, h_2);

  BIGNUM* sig_sub_res = BN_new();
  BN_sub(sig_sub_res, s_1, s_2);

  BIGNUM* k = BN_new();
  BN_CTX* ctx = BN_CTX_new();
  BN_div(k, NULL, hash_sub_res, sig_sub_res, ctx);

  return k;
}

int main(int argc, char *argv[]) {

  FILE  *pemfile;
  int rc;
  OSSL_DECODER_CTX *dctx;
  EVP_PKEY *pkey = NULL;  // must be NULL for OSSL_DECODER_CTX call
  BIGNUM
    *p = NULL,
    *q = NULL,
    *g = NULL,
    *y = NULL,
    *a = NULL;  // all must be NULL, for get_bn_param calls



  if( argc < 2) {
    printf("needs arguments: the pem file\n");
    return -1;
  }

  pemfile = fopen(argv[1], "r");


  dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, "PEM", NULL, "DSA",0,NULL, NULL);

 
  if (NULL == dctx) {
      printf("OSSL_DECODER_CTX_new_for_pkey failed\n");
      return -1;
    }

  if ( 0 == OSSL_DECODER_from_fp(dctx, pemfile)) {
    printf("OSSL_DECODER_from_fp failed\n");
    return -1;
  }


  if (EVP_PKEY_get_bn_param(pkey, "priv", &a))
    printf("a %s\n",BN_bn2hex(a));
  if (EVP_PKEY_get_bn_param(pkey, "pub",  &y))
    printf("y %s\n",BN_bn2hex(y));      
  if (EVP_PKEY_get_bn_param(pkey, "p",    &p))
    printf("p %s\n",BN_bn2hex(p));
  if (EVP_PKEY_get_bn_param(pkey, "q",    &q))
    printf("q %s\n",BN_bn2hex(q));
  if (EVP_PKEY_get_bn_param(pkey, "g",    &g))
    printf("g %s\n",BN_bn2hex(g));

    
  // NOW example code to create and save new keypair
  // that's just like this one...
  // except with a guessed private key
  if (3 == argc) {
    FILE *pemfile2;
    BIGNUM *a2= BN_new(),*y2,*p2,*q2,*g2;
    EVP_PKEY_CTX *pctx = NULL;
    EVP_PKEY *pkey2 = NULL;
    OSSL_ENCODER_CTX *ectx;
    OSSL_PARAM_BLD *tmpl = NULL;
    OSSL_PARAM *params = NULL;

    /*---------MY CODE STARTS HERE---------*/

    //STEP 1: Calculate a value for the reused nonce K
    BIGNUM* h_1 = BN_new();
    BIGNUM* s_1 = BN_new();
    
    BIGNUM* h_2 = BN_new();
    BIGNUM* s_2 = BN_new();

    BN_hex2bn(&h_1, MSG_1_HASH);
    BN_hex2bn(&s_1, MSG_1_S);

    BN_hex2bn(&h_2, MSG_2_HASH);
    BN_hex2bn(&s_2, MSG_2_S);

    BIGNUM* k = calc_k(h_1, s_1, h_2, s_2);

    printf("K as a bignum: %s\n", BN_bn2hex(k));

    // STEP 2. Calculate r = (g^k MOD P) MOD q
    BIGNUM* g_k = BN_new();
    BN_CTX* ctx = BN_CTX_new();
    BN_exp(g_k, g, k, ctx);
    
    BIGNUM* mod_p_res  = BN_new();
    BN_nnmod(mod_p_res, g_k, p, ctx);

    BIGNUM* r  = BN_new();
    BN_nnmod(r, mod_p_res, q, ctx);

    //STEP 3: Use found value of K to calculate priv key a2
    BIGNUM* mult_res = BN_new();
    ctx = BN_CTX_new();
    BN_mul(mult_res, s_1, k, ctx);

    BIGNUM* sub_res = BN_new();
    BN_sub(sub_res, mult_res, h_1);

    BIGNUM* mod_inv_r = BN_new();
    BN_mod_inverse(mod_inv_r, r, q, ctx);

    BN_mul(a2, sub_res, mod_inv_r, ctx); // the guess (a2)
    /*---------MY CODE ENDS HERE---------*/
    p2 = BN_dup(p);
    y2 = BN_dup(y);
    q2 = BN_dup(q);
    g2 = BN_dup(g);

    //--------------------
    // this part adapted from https://github.com/openssl/openssl/blob/5540855bda5e58f4b33f2306feb6ff4e93c0af81/apps/testdsa.h#L239-L268
    if ((pctx = EVP_PKEY_CTX_new_from_name(NULL, "DSA", NULL)) == NULL)  {
      printf("trouble\n");
      return -1;
    }

    if ((tmpl = OSSL_PARAM_BLD_new()) == NULL
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_FFC_P,
                                   p2)
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_FFC_Q,
                                   q2)
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_FFC_G,
                                   g2)
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_PRIV_KEY,
                                   a2)
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_PUB_KEY,
                                   y2)
        || (params = OSSL_PARAM_BLD_to_param(tmpl)) == NULL) {
      printf("more trouble\n");
      return -1;
    }

    if (EVP_PKEY_fromdata_init(pctx) <= 0
        || EVP_PKEY_fromdata(pctx, &pkey2, EVP_PKEY_KEYPAIR,
                             params) <= 0) {
      printf("still more trouble\n");
      return -1;
      
    }
    //----------------------

    ectx = OSSL_ENCODER_CTX_new_for_pkey(pkey2, EVP_PKEY_KEYPAIR, "PEM", NULL, NULL);

    if (NULL == ectx) {
      printf("even more trouble\n");
      return -1;
    }
    
    pemfile2 = fopen(argv[2],"w");
    
    if (0==OSSL_ENCODER_to_fp(ectx,pemfile2)) {
      printf("OSSL_ENCODER_to_fp failed\n");
      return -1;
    }
  }
}



