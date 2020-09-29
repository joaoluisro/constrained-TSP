all:
	g++ src/main.cpp src/tsp.cpp src/graph.cpp -o ordem -std=c++11
clean:
	rm ./main.exe
