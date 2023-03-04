CC = gcc
FLAGS = -I -g -Wall

all: test_assign1

test_assign1: test_assign1_1.c test_helper.h dberror.c dberror.h storage_mgr.c storage_mgr.h
	$(CC) $(FLAGS) dberror.c storage_mgr.c test_assign1_1.c -o test_assign1

clean:
	rm -rf test_assign1.exe

run:
	./test_assign1