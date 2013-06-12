SRC = $(wildcard src/emq/*.c)
TESTS = $(wildcard test/*.c)
LIB = /usr/local/lib
CFLAGS = -L$(LIB)

release: $(SRC)
	$(CC) $^ -std=c99 -lm -I include -I deps -o release/$@ $(CFLAGS)

test: $(SRC) $(TESTS)
	make clean
	$(CC) $^ -std=c99 -lm -I include -I deps -o release/test $(CFLAGS) 
	./release/test
	rm -rf release/test

clean:
	rm -rf release/*

.PHONY: release test clean