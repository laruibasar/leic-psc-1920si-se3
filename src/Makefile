CC = cc
CFLAGS =-Wall -Werror -Og -g
LDFLAGS =
DEPS = books.h
OBJ = books.o http_get_to_file.o http_get_json_data.o
TEST_OBJ = test_1.o

CURL_CFLAGS != curl-config --cflags
CURL_LDFLAGS != curl-config --libs

JSON-C_CFLAGS != pkg-config --cflags json-c
JSON-C_LDFLAGS != pkg-config --libs json-c

CFLAGS += $(CURL_CFLAGS) $(JSON-C_CFLAGS)
LDFLAGS += $(CURL_LDFLAGS) $(JSON-C_LDFLAGS)

TESTDIR = ../tests
BUILD_DIR = ../build

books.o: books.c $(DEPS)
	$(CC) -c $(CFLAGS) books.c

http_get_to_file.o: http_get_to_file.c $(DEPS)
	$(CC) -c $(CFLAGS) http_get_to_file.c

http_get_json_data.o: http_get_json_data.c $(DEPS)
	$(CC) -c $(CFLAGS) http_get_json_data.c

test1: test_1.o $(OBJ)
	$(CC) -o $(TESTDIR)/test1 $(LDFLAGS) test_1.o $(OBJ)

test_1.o: test_1.c $(DEPS)
	$(CC) -c $(CFLAGS) test_1.c

test2: test_2.o $(OBJ)
	$(CC) -o $(TESTDIR)/test2 $(LDFLAGS) test_2.o $(OBJ)

test_2.o: test_2.c $(DEPS)
	$(CC) -c $(CFLAGS) test_2.c

.PHONY: all
all: build-tests

.PHONY: clean
clean: 
	@echo "Cleaning all *.o files"
	rm $(OBJ)

.PHONY: clean-tests
clean-tests:
	@echo "Cleaning tests *.o"
	rm $(TEST_OBJ)
	@echo "Cleaning txt files"
	rm $(TESTDIR)/*.txt
	@echo "Cleaning tests executable files"
	rm $(TESTDIR)/*

.PHONY: build-tests
build-tests: test1 test2

