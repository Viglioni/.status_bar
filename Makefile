all:
	gcc -pthread src/stocks.c -o bin/stocks.exec
	gcc -pthread src/statusthread.c -o bin/status.exec
