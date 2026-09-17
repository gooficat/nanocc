all: mycc
mycc: mycc.o clex.o vec.o parse_single.o tokens.o token_types.o
.PHONY: clean
clean:
	rm *.o mycc
