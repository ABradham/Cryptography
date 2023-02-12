output.txt: readBits
	./readBits list.txt > $@

readBits: readBits.c
	gcc -Wall -g readBits.c -o $@ 

clean:
	rm -rf *.o readBits
	rm output.txt