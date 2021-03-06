app: main.o
	g++ build/main.o -o build/app

main.o: src/main.cpp
	g++ -c -o build/main.o src/main.cpp

clean:
	rm build/*.o build/app 

$(shell mkdir -p build/)

