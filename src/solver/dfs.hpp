#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;


void solve_maze_dfs(vector<vector<string>>maze);

bool dfs(vector<vector<string>>&maze,int x,int y,vector<vector<bool>>&vis);