all: init castor viewer tester cleanup

init: init.c
	gcc -o init init.c

castor: castor.c
	gcc -o castor castor.c

viewer: viewer.c
	gcc -o viewer viewer.c

tester: tester.c
	gcc -o tester tester.c
	
cleanup: cleanup.c
	gcc -o cleanup cleanup.c

clean:
	rm -f init castor viewer tester cleanup
