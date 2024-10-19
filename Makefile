# Compiler
CC = mpicxx

# Compiler flags
CFLAGS = -Wall -Wextra -O2

make : src/maze.cpp src/generator/mazegenerator.cpp src/solver/mazesolver.cpp src/generator/bfs.cpp src/generator/kruskal.cpp src/solver/dfs.cpp src/solver/dijkstra.cpp
	$(CC) $(CFLAGS) src/maze.cpp src/generator/mazegenerator.cpp src/solver/mazesolver.cpp src/generator/bfs.cpp src/generator/kruskal.cpp src/solver/dfs.cpp src/solver/dijkstra.cpp -o maze.out
