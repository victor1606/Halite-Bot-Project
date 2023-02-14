build:
	g++ -std=c++11 MyBot.cpp bot_functions.cpp -o MyBot.o

run:
	./MyBot.o

clean:
	rm -rf ./MyBot.o