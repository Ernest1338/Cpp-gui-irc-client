app: main.o irc.o
	g++ -lpthread build/main.o build/irc.o -o build/app

main.o: src/main.cpp
	g++ -c -o build/main.o src/main.cpp

irc.o: src/irc.cpp
	g++ -c -o build/irc.o src/irc.cpp

clean:
	rm build/*.o build/app 

$(shell mkdir -p build/)
