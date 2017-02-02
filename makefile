cipher: cipher.o
	gcc cipher.o -o cipher
cipher.o: cipher.c

decipher: decipher.o
	gcc -o decipher Project1.c
decipher.o: Project1.c

test: test1 test2
test1: 
	./cipher 1 5 data.txt data.out
	./decipher
	diff -s data.txt results.txt	
test2:
	./cipher 1 5 data2.txt data2.out
	./decipher
	diff -s data2.txt results2.txt

