CC := gcc
FLAGS := -pthread

FIRST := testCIO
SECOND := testKernelIO
THIRD := task3

RESULT := results.txt

first:
	$(CC) $(FIRST).c -o $(FIRST).out

first_threads:
	$(CC) $(FLAGS) $(FIRST)_threads.c -o $(FIRST)_threads.out


second:
	$(CC) $(SECOND).c -o $(SECOND).out

second_threads:
	$(CC) $(FLAGS) $(SECOND)_threads.c -o $(SECOND)_threads.out


third:
	rm $(RESULT)
	$(CC) $(THIRD).c -o $(THIRD).out

third_threads:
	rm $(RESULT)
	$(CC) $(FLAGS) $(THIRD)_threads.c -o $(THIRD)_threads.out


clean:
	rm *.out
