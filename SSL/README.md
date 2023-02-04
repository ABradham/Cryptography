# SSL
The `SSL/` directory contains the code and scripts I used to generate my OpenSSL RSA keypair and some code verifying that `pq=N` and that `de MOD phi = 1`. 

In general, by running `make` in the `SSL/` directory and running `./simpleSSL`, you should be able to verify that my command line generated SSL keys were equivalent to the parameters read out by the SSL C library. 

For more information on how to run and verify this, check the Makefile located at `SSL/Makefile`.