all: main main2 main3 main4 main5

exo9.o: exo9.c
	gcc -c -Wall exo9.c
exo8.o: exo8.c
	gcc -c -Wall exo8.c
exo7.o: exo7.c
	gcc -c -Wall exo7.c -lssl -lcrypto
exo6.o: exo6.c
	gcc -c -Wall exo6.c
exo5.o: exo5.c
	gcc -c -Wall exo5.c
exo4.o: exo4.c
	gcc -c -Wall exo4.c
exo3.o: exo3.c
	gcc -c -Wall exo3.c
exo2.o: exo2.c
	gcc -c -Wall exo2.c
exo1.o: exo1.c exo1.h
	gcc -c -Wall exo1.c

main: exo1.o exo2.o main.c
	gcc -Wall -o main main.c exo2.o exo1.o -lm

main2: exo1.o exo2.o exo3.o exo4.o main2.c
	gcc -Wall -o main2 main2.c exo4.o exo3.o \
	exo2.o exo1.o -lm

main3: exo1.o exo2.o exo3.o exo4.o exo5.o exo6.o main3.c
	gcc -Wall -o main3 main3.c exo6.o exo5.o exo4.o exo3.o \
	exo2.o exo1.o -lm

main4: exo1.o exo2.o exo3.o exo4.o exo5.o exo6.o exo7.o main4.c
	gcc -Wall -o main4 main4.c exo7.o exo6.o exo5.o \
	exo4.o exo3.o exo2.o exo1.o -lm -lssl -lcrypto

main5: exo1.o exo2.o exo3.o exo4.o exo5.o exo6.o exo7.o exo8.o exo9.o main5.c
	gcc -Wall -o main5 main5.c exo9.o exo8.o exo7.o exo6.o \
	exo5.o exo4.o exo3.o exo2.o exo1.o -lm -lssl -lcrypto 

clean:
	rm -f main main2 main3 main4 main5
	rm -f *.o 
	rm -f verify_modpow_and_modpow_naive.txt \
	random_prime_number.txt
	rm -f keys.txt declarations.txt candidates.txt
	rm -f ecrireBlock.txt
	rm -f Pending_votes.txt
	rm -f Blockchain/*