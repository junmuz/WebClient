all: clean build

clean:
	rm main

build:
	gcc -o main main.c

