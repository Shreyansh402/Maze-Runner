
#include "bfs.hpp"

vector<int> generate_maze_bfs(int x_src, int y_src, int x_dest, int y_dest){

    int msize = 16;
    vector<vector<int>>adj_B = {{-1,0},{0,-1},{0,1},{1,0}};
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<int> edge_set;

    vector<vector<int>>deg(msize,vector<int>(msize,0));

    queue<pair<int,int>>q;
    bool vis[msize][msize];
    for(int i=0;i<msize;i++){
        for(int j=0;j<msize;j++){ 
            vis[i][j] = false;
            for(auto it:adj_B){
                int xi = it[0] + i,yi = it[1]  + j;
                if(xi>=0 && xi<msize && yi>=0 && yi< msize)
                    deg[i][j]++;
            }
        }

    }

    q.push(make_pair(x_src,y_src));
    for(auto itt : adj_B){
        int xi = itt[0] ,yi = itt[1]  + msize-1;
        if(xi>=0 && xi<msize && yi>=0 && yi< msize)
        deg[xi][yi]--;
                }
    vis[x_src][y_src] = true;
    while (!q.empty()){

        
        pair<int,int> p = q.front();
        q.pop();
        vector<pair<int,int>>child;
        

        for(auto it:adj_B){

            int x = it[0] + p.first;
            int y = it[1] + p.second;
            if((x<0 || x>=msize || y<0 || y>=msize) ||   vis[x][y] )continue;
            
            
            double randomVal = ((double)rand()) / RAND_MAX;

            int randomValue = randomVal>0.5 ? 1 : 0;

            if(randomValue==1 )

            {

                q.push(make_pair(x,y));
                

                vis[x][y] = true;
                // edge_set.push_back(make_pair(p,make_pair(x,y)));
                edge_set.push_back(p.first + (rank/2)*16 );
                edge_set.push_back(p.second + (rank%2)*16);
                edge_set.push_back(x + (rank/2)*16 );
                edge_set.push_back(y + (rank%2)*16);
                for(auto itt : adj_B){
                    int xi = itt[0] + x,yi = itt[1]  + y;
                    if(xi>=0 && xi<msize && yi>=0 && yi< msize)
                    deg[xi][yi]--;
                }


                child.push_back(make_pair(x,y));

            }
            

        }
        if(q.empty() && !vis[x_dest][y_dest]){
            for(int i=msize-1;i>=0;i--){
                for(int j=0;j<msize;j++){
                    if(deg[i][j]>0 && vis[i][j]){
                        q.push(make_pair(i,j));


                    }
                }
            }
        }

            
        

    }

    return edge_set;


}
