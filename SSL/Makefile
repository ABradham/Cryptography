simpleSSL: simpleSSL.c
	gcc -Wall simpleSSL.c -o $@ -lcrypto 

clean:
	rm -rf *.o simpleSSL


# The public key corresponds to the RSA modulus and the public exponent.
# Running this command will extract the public key from the key.pem file and
# put it into the public.pem file.
extract_pub:
	openssl rsa -in key.pem -pubout > public.pem

# Running this command will allow you to view the contents of the private key 
# as well as the RSA parameters used to generate the public and private keys
view_key_contents:
	openssl rsa -in key.pem -text -noout