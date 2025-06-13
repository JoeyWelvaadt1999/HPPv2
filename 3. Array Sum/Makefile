UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	OPT_FLAGS=-lomp
else
	OPT_FLAGS=
endif

all: arraySum

arraySum:
		g++ -O2 -fopenmp $(OPT_FLAGS) -o arraySum arraySum.cc

run: arraySum 10k.txt 100k.txt 10m.txt 100m.txt
	./arraySum 10k.txt
	./arraySum 100k.txt
	./arraySum 10m.txt
	./arraySum 100m.txt

10k.txt:
	python ./gen_array.py 10000 10k.txt

100k.txt:
	python ./gen_array.py 100000 100k.txt

10m.txt:
	python ./gen_array.py 1000000 10m.txt

100m.txt:
	python ./gen_array.py 10000000 100m.txt
