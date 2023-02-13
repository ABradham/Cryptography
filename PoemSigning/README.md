# Poem Signing
This directory contains two messages (`jabber1.txt` and `jabber2.txt`) and a signature (`jabber.sign`) that corresponds to one of the messages.

## Results
By using the OpenSSL command line tools, I've found that `jabber2.txt` was the message that was signed with `pub-key.pem`.

## To Run
To verify that `jabber2.txt` is indeed the file that was signed, run `make` in terminal. Running `make` implicitly runs the `verify-2` rule which uses the OpenSSL command line tool to verify the signature against `jabber2.txt`. The exact command is `openssl pkeyutl -verify -inkey pub-key.pem -pubin -sigfile jabber.sign -in jabber2.txt -digest sha256 -pkeyopt rsa_padding_mode:pss -rawin`.

You'll know everything is working if you see `Signature Verified Successfully` after running `make`.