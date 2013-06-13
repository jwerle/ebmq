
SRC = $(wildcard src/ebmq/*.c)
SRC += $(wildcard deps/*.c)
TESTS = $(wildcard test/*.c)
DAEMON = $(wildcard ebmqd/*.c)
DAEMON_TEST = test/ebmqd.c
LIB = /usr/local/lib
CFLAGS = -L$(LIB) -lzmq
PREFIX = /usr/local
BIN = ebmqd


.PHONY: test clean $(BIN)

##
# daemon build
##

$(BIN): $(SRC) $(DAEMON)
	@make clean
	@mkdir release
	@$(CC) $^ -std=c99 -lm -I include -I deps -o release/$(BIN) $(CFLAGS) 


##
# lib test
##

test: $(SRC) $(TESTS)
	@make clean
	@mkdir release
	@$(CC) $^ -std=c99 -lm -I include -I deps -o release/test $(CFLAGS) 
	@./release/test
	@rm -rf release/test


##
# daemon test
##

$(BIN)-test: $(SRC) $(DAEMON_TEST)
	@make $(BIN)
	@$(CC) $^ -std=c99 -lm -I include -I deps -o release/test $(CFLAGS) 
	@./release/test
	@rm -rf release/test


##
# installs daemon to system
##

install: $(BIN)
	@install release/$(BIN) $(PREFIX)/bin


##
# removes daemon from system
##

uninstall:
	@rm $(PREFIX)/bin/$(BIN)


##
# cleans the repo of releases
##

clean:
	@rm -rf release