all: phrasesim wordsim

phrasesim: src/pserver.cpp build/reader.o build/wikinet.o 
	g++ src/pserver.cpp build/reader.o build/wikinet.o -o phrasesim -std=c++0x

wordsim: src/server.cpp build/reader.o build/wikinet.o 
	g++ src/server.cpp build/reader.o build/wikinet.o -o wordsim -std=c++0x

build/reader.o: src/reader.cpp src/reader.h
	g++ -c src/reader.cpp -o build/reader.o -std=c++0x

build/wikinet.o: src/wikinet.cpp src/wikinet.h src/sparsevecimpl.h
	g++ -c src/wikinet.cpp -o build/wikinet.o -std=c++0x

clean:
	rm phrasesim
	rm wordsim
	rm build/*.o
