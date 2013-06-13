SRC = $(wildcard src/emq/*.c)
SRC += $(wildcard deps/*.c)
TESTS = $(wildcard test/*.c)
DAEMON = $(wildcard emqd/*.c)
DAEMON_TEST = test/emqd.c
LIB = /usr/local/lib
CFLAGS = -L$(LIB) -lzmq

##
# daemon build
##

emqd: $(SRC) $(DAEMON)
	@make clean
	@mkdir release
	@$(CC) $^ -std=c99 -lm -I include -I deps -o release/emqd $(CFLAGS) 


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

emqd-test: $(SRC) $(DAEMON_TEST)
	@make emqd
	@$(CC) $^ -std=c99 -lm -I include -I deps -o release/test $(CFLAGS) 
	@./release/test
	@rm -rf release/test

clean:
	@rm -rf release

.PHONY: test clean emqd