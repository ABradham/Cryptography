verify-2:
	openssl pkeyutl -verify -inkey pub-key.pem -pubin -sigfile jabber.sign -in jabber2.txt -digest sha256 -pkeyopt rsa_padding_mode:pss -rawin

verify-1:
	openssl pkeyutl -verify -inkey pub-key.pem -pubin -sigfile jabber.sign -in jabber1.txt -digest sha256 -pkeyopt rsa_padding_mode:pss -rawin