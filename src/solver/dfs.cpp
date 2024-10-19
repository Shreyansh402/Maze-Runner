#include "dfs.hpp"

bool dfs(vector<vector<string>>&maze,int x,int y,vector<vector<bool>>&vis){
    int Msize = 63;
    int msize = 32;
    vector<vector<int>>adj = {{-1,0},{0,-1},{0,1},{1,0}};
    
    vis[x][y] = true;
    if(x==Msize && y==0){
        maze[x][y] = "O";
        return true;
    }
    for(auto it : adj ){
        int xi = x+it[0],yi=y+it[1];
        if(xi<0 || xi>Msize || yi<0 || yi> Msize)continue;
        
        if(!vis[xi][yi] && maze[xi][yi] != "X"){
            if(dfs(maze,xi,yi,vis)){
                maze[xi][yi] = "O";
                return true;
            }
        }

    }
    if(maze[x][y]=="O")
        maze[x][y] = "X";
    return false;
}




void solve_maze_dfs(vector<vector<string>>maze){

    int Msize = 63;
    int msize = 32;

    int rank ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0){

        maze[0][Msize]="-";
            maze[Msize][0] = "-";


        vector<vector<string>> res(maze.size(),vector<string>(maze[0].size()));
        for (int i=0;i<maze.size();i++){
            for(int j=0;j<maze[0].size();j++)
                res[i][j] = maze[i][j];
        }
        vector<vector<bool>>vis(maze.size(),vector<bool>(maze[0].size(),false));
        dfs(res,0,Msize,vis);


    std::ofstream outfile;
            outfile.open("maze_new.txt");
            for (int i = 0; i < res.size(); i++) {
                for (int j = 0; j < res[i].size(); j++) {
                    outfile << res[i][j] ;
                }
                outfile << "\n";
            }
            outfile.close();
    }

}