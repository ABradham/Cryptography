simpleSSL: simpleSSL.c
	gcc -Wall simpleSSL.c -o $@ -lcrypto 

clean:
	rm -rf *.o simpleSSL


# The public key corresponds to the RSA modulus and the public exponent.
# Running this command will extract the public key from the key.pem file and
# put it into the public.pem file.
extract_pub:
	openssl rsa -in key.pem -pubout > public.pem