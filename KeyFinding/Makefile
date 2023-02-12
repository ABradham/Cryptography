full_test: pemtest
	./pemtest dsa_pub.pem outfile.pem
	openssl pkeyutl -verify -inkey outfile.pem -sigfile msg1.sign -in msg1.txt -digest sha1 -rawin

pemtest: pemtest.c
	gcc -g -Wall pemtest.c -lcrypto -o $@ 

hsr: hsr.c
	gcc -Wall hsr.c -lcrypto -o $@ 

clean:
	rm -rf *.o hsr pemtest outfile.pem