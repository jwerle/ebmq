SRC = $(wildcard src/ebmq/*.c)
SRC += $(wildcard deps/*.c)
TESTS = $(wildcard test/*.c)
DAEMON = $(wildcard ebmqd/*.c)
DAEMON_TEST = test/ebmqd.c
LIB = /usr/local/lib
CFLAGS = -L$(LIB) -lzmq

##
# daemon build
##

ebmqd: $(SRC) $(DAEMON)
	@make clean
	@mkdir release
	@$(CC) $^ -std=c99 -lm -I include -I deps -o release/ebmqd $(CFLAGS) 


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

ebmqd-test: $(SRC) $(DAEMON_TEST)
	@make ebmqd
	@$(CC) $^ -std=c99 -lm -I include -I deps -o release/test $(CFLAGS) 
	@./release/test
	@rm -rf release/test

clean:
	@rm -rf release

.PHONY: test clean ebmqd