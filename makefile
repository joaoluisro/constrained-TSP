all:
	g++ src/main.cpp src/tsp.cpp src/graph.cpp -o main -std=c++11
clean:
	rm ./main.exe
