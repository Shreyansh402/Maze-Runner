#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;


bool is_owner(int x,int y,int rank);

void Dijkstra(int dist[32][32]);

vector<vector<string>> solve_maze_Dijkstra(vector<vector<string>>maze,vector<vector<int>>&dist);