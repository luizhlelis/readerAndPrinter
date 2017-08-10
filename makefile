all: readerAndPrinter

readerAndPrinter: readerAndPrinter.cpp
	g++ -pthread -std=c++11 readerAndPrinter.cpp -o readerAndPrinter