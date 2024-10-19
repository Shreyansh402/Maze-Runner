#include <bits/stdc++.h>
using namespace std;
#include <mpi.h>

#include "generator/mazegenerator.hpp"
#include "solver/mazesolver.hpp"

void print_maze(vector<vector<string>>&maze){
        for (const auto& row : maze) {
        for (const auto& cell : row) {
            cout << cell;
        }
        cout << endl; 
        }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " -g [bfs/kruskal] -s [dfs/dijkstra]" << std::endl;
        return 1;
    }

    std::unordered_map<std::string, std::string> options;
    for (int i = 1; i < argc; i += 2) {
        std::string flag = argv[i];
        std::string value = argv[i + 1];
        options[flag] = value;
    }

    if (options.count("-g") == 0 || options.count("-s") == 0) {
        std::cerr << "Usage: " << argv[0] << " -g [bfs/kruskal] -s [dfs/dijkstra]" << std::endl;
        return 1;
    }

    std::string generatorFlag = options["-g"];
    std::string solverFlag = options["-s"];

    if (generatorFlag != "bfs" && generatorFlag != "kruskal") {
        std::cerr << "Invalid generator option. Use '-g bfs' or '-g kruskal'" << std::endl;
        return 1;
    }

    if (solverFlag != "dfs" && solverFlag != "dijkstra") {
        std::cerr << "Invalid solver option. Use '-s dfs' or '-s dijkstra'" << std::endl;
        return 1;
    }
    
    
    

    if (generatorFlag == "bfs") {
        mazegenerator("bfs");
        
        MPI_Barrier(MPI_COMM_WORLD);
    } else if (generatorFlag == "kruskal") {
        mazegenerator("kruskal");
        
        MPI_Barrier(MPI_COMM_WORLD);
    }

    vector<vector<string>> maze;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank==0){
        ifstream inputFile("maze.txt");
        if (inputFile.is_open()) {
            string line;
            while (getline(inputFile, line)) {
                vector<string> row;
                for (char c : line) {
                    string cell(1, c);
                    row.push_back(cell);
                }
                maze.push_back(row);
            }
            inputFile.close();
        } else {
            cerr << "Failed to open maze.txt" << endl;
            return 1;
        }
        }


    
    MPI_Barrier(MPI_COMM_WORLD);
    

    if (solverFlag == "dfs") {
        mazesolver(maze,"dfs");
    } else if (solverFlag == "dijkstra") {
        mazesolver(maze,"dijkstra");


    }

    MPI_Barrier(MPI_COMM_WORLD);
    cout<<"Rank: "<<rank<<" Done\n";

    vector<vector<string>> maze_new;

     if (rank==0){
        ifstream inputFile("maze_new.txt");
        if (inputFile.is_open()) {
            string line;
            while (getline(inputFile, line)) {
                vector<string> row;
                for (char c : line) {
                    string cell(1, c);
                    row.push_back(cell);
                }
                maze_new.push_back(row);
            }
            inputFile.close();
        } else {
            cerr << "Failed to open maze.txt" << endl;
            return 1;
        }
        }

    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank==0){
    for (int i=0;i<64;i++) {
        for (int j=0;j<64;j++) {
            if (maze_new[i][j]=="X")maze_new[i][j]="*";
            else if (maze_new[i][j]=="-")maze_new[i][j]=" ";
            else if (maze_new[i][j]=="O")maze_new[i][j]="P";
        }
    }
    maze_new[0][63]="S";
    maze_new[63][0]="E";
    }
    MPI_Finalize();

    print_maze(maze_new);

    return 0;
}


