all: clean release debug


debug:
	mkdir -p debug
	gcc -Wall -g -O0 -DDEBUG -DFULL -pthread honeypot.c -o debug/hp
	gcc -Wall -g -O0 -DDEBUG -DLITE -pthread honeypot.c -o debug/hpLite
	gcc -Wall -g -O0 -DDEBUG portScanner.c -o debug/pscan


release:
	mkdir -p release
	gcc -Wall -DFULL -pthread honeypot.c -o release/hp
	gcc -Wall -DLITE -pthread honeypot.c -o release/hpLite
	gcc -Wall portScanner.c -o release/pscan


#Single thread honeypot Version
single:
	mkdir -p single
	gcc -Wall -DFULL honeypot_single.c -o single/hp
	gcc -Wall -DLITE honeypot_single.c -o single/hpLite
	gcc -Wall portScanner.c -o single/pscan
	

clean:
	-rm -rf debug release single