all: mycc
mycc: mycc.o clex.o vec.o parse_single.o tokens.o
.PHONY: clean
clean:
	rm *.o mycc
