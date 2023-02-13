# Key Finding
This directory contains the code, texfiles and key (`.pem`) files needed to recreate an attack in which a signer's private key can be extracted if the signer uses the same random nonce `k` to generate signatures.

## To Run
To run the example proving that this attack does indeed work, type `./make` in the terminal. This runs the `full_test` rule in the directory's MAKEFILE which:
1. Compiles the `pemtest.c` file into an executatble (if not already generated)
2. Runs the command `./pemt est dsa_pub.pem outfile.pem` to generate a *fake* private key given the two signed messages that use the same random nonce `k`.
3. Runs the command `openssl pkeyutl -verify -inkey outfile.pem -sigfile msg1.sign -in msg1.txt -digest sha1 -rawin` to verify that this *fake* private key does successfully impersonate the *real* private key.

You'll know everthing works if you see `Signature Verified Successfully` after running `make`.
