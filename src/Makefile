CC = cc

LIB = libbooks.so
APP = books

CFLAGS = -c -Wall -Werror -Og -g
LIB_CFLAGS = -fpic

LDFLAGS =
LIB_LDFLAGS = -shared
APP_LDFLAGS = -rdynamic

DEPS = books.h
OBJ = books.o http_get_to_file.o http_get_json_data.o \
      google_books_search_by_author.o google_books_get_urls.o \
      create_volume.o clean_volume.o

TEST_OBJ = test_1.o test_2.o test_3.o test_4.o

CURL_CFLAGS != curl-config --cflags
CURL_LDFLAGS != curl-config --libs

JSON-C_CFLAGS != pkg-config --cflags json-c
JSON-C_LDFLAGS != pkg-config --libs json-c

CFLAGS += $(CURL_CFLAGS) $(JSON-C_CFLAGS)
LDFLAGS += $(CURL_LDFLAGS) $(JSON-C_LDFLAGS)

TEST_DIR = ../tests
BUILD_DIR = ../build

.PHONY: all
all: lib app tests

.PHONY: lib
lib: $(OBJ)
	@echo "Linking and generate libbooks.so"
	$(CC) $(LIB_LDFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$(LIB) $(OBJ)  

.PHONY: app
app:	lib gbooks

.PHONY: tests
tests: test1 test2 test3 test4

.PHONY: clean-all
clean-all: clean clean-tests

.PHONY: clean
clean: clean-lib clean-app
	@echo "Cleaning all *.o files"
	rm $(OBJ)

.PHONY: clean-lib
clean-lib:
	@echo "Cleaning libbooks.so"
	rm $(BUILD_DIR)/libbooks.so

.PHONY: clean-app
clean-app:
	@echo "Removing gbooks.o"
	rm gbooks.o
	@echo "Removing app gbooks"
	rm $(BUILD_DIR)/gbooks

.PHONY: clean-tests
clean-tests:
	@echo "Cleaning tests *.o"
	rm $(TEST_OBJ)
	@echo "Cleaning tests executable files"
	rm $(TEST_DIR)/*

books.o: books.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) $(LIB_CFLAGS) -o $@ $<

http_get_to_file.o: http_get_to_file.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) $(LIB_CFLAGS) -o $@ $<

http_get_json_data.o: http_get_json_data.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) $(LIB_CFLAGS) -o $@ $<

google_books_search_by_author.o: google_books_search_by_author.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) $(LIB_CFLAGS) -o $@ $<

google_books_get_urls.o: google_books_get_urls.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) $(LIB_CFLAGS) -o $@ $<

create_volume.o: create_volume.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) $(LIB_CFLAGS) -o $@ $<

clean_volume.o: clean_volume.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) $(LIB_CFLAGS) -o $@ $<

gbooks: gbooks.o $(OBJ)
	@echo "Linking and create executable " $@
	$(CC) $(APP_LDFLAGS) -o $(BUILD_DIR)/$@ gbooks.o 

gbooks.o: gbooks.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) -o $@ $<

test1: test_1.o $(OBJ)
	@echo "Linking and create executable " $@
	$(CC) -o $(TEST_DIR)/$@ $(LDFLAGS) $(OBJ) test_1.o

test_1.o: test_1.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) -o $@ $<

test2: test_2.o $(OBJ)
	@echo "Linking and create executable " $@
	$(CC) -o $(TEST_DIR)/$@ $(LDFLAGS) $(OBJ) test_2.o

test_2.o: test_2.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) -o $@ $<

test3: test_3.o $(OBJ)
	@echo "Linking and create executable " $@
	$(CC) -o $(TEST_DIR)/$@ $(LDFLAGS) $(OBJ) test_3.o

test_3.o: test_3.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) -o $@ $<

test4: test_4.o $(OBJ)
	@echo "Linking and create executable " $@
	$(CC) -o $(TEST_DIR)/$@ $(LDFLAGS) $(OBJ) test_4.o

test_4.o: test_4.c $(DEPS)
	@echo "Compiling " $<
	$(CC) $(CFLAGS) -o $@ $<

