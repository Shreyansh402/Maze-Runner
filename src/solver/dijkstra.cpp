#include "dijkstra.hpp"
int Msize = 63;    
int msize = 16;

bool is_owner(int x,int y,int rank){

    if(rank==0)return (x<31.5 && y<31.5);
    if (rank==1)return (x<31.5 && y>31.5);
    if(rank==2)return (x>31.5 && y<31.5);
    if(rank==3)return (x>31.5 && y>31.5);

}

void Dijkstra(int dist[32][32]){

     vector<vector<int>>adj_D = {{-1,0},{0,-1},{0,1},{1,0}};

    

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int send_buf[3];
    int recv_buf[3];
    int sz;int total_sz;

    
    set<vector<int>>q;
    for(int i=0;i<32;i++){
        for(int j=0;j<32;j++){
            if(dist[i][j]>=0){
                q.insert({dist[i][j],i,j});
                
                
            }
        }
    }
    // cout<<"came till here2 "<<rank<<endl;
    while(1){
        // cout<<"came till here3 "<<rank<<endl;
        
            // cout<<q.size()<<"  "<<rank<<" qsize became 0"<<endl;
        
        
        set<int>p_to_send ;
        for(int p=0;p<4;p++){
            if(p==rank)continue;
            p_to_send.insert(p);
        }

        

        if(q.size()>0){

        vector<int> min_node = *q.begin();
        int x = min_node[1],y=min_node[2],val = min_node[0];
        q.erase(*q.begin());
        
        for(auto it:adj_D){
            int xi = x+it[0],yi=y+it[1];
            int xi_abs = xi + 32*(rank/2);
            int yi_abs = yi + 32*(rank%2);
            if(xi_abs<0 || xi_abs>Msize || yi_abs<0 || yi_abs> Msize )continue;
            if(is_owner(xi_abs,yi_abs,rank)){
            if(dist[xi][yi]>dist[x][y] + 1 && dist[xi][yi]>=0){
                dist[xi][yi] = val+1;
                q.insert({val+1,xi,yi});
            }
            }
            else{
                send_buf[0] = xi_abs;
                send_buf[1] = yi_abs;
                send_buf[2] = val;
                int dest ;
                if(xi_abs<31.5 && yi_abs<31.5)dest = 0;
                if(xi_abs<31.5 && yi_abs>31.5)dest = 1;
                if(xi_abs>31.5 && yi_abs<31.5)dest = 2;
                if(xi_abs>31.5 && yi_abs>31.5)dest = 3;
                // cout<<"sending to "<<dest<<" from "<<rank<<" "<<xi<<" "<<yi<<" "<<val<<endl;
                MPI_Send(send_buf, 3, MPI_INT, dest, 0, MPI_COMM_WORLD);
                p_to_send.erase(*p_to_send.find(dest));
            
            }
        }
        
        }
        for(auto p:p_to_send){
            send_buf[0] = 32*(p/2);
            send_buf[1] = 32*(p%2);
            send_buf[2] = int(1e9) + 7;
            // cout<<"sending to "<<p<<" from "<<rank<<" "<<32*(p/2)<<" "<<32*(p%2)<<" "<<int(1e9) + 7<<endl;
            MPI_Send(send_buf, 3, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
        for(int i=0;i<4;i++){
            if(i==rank)continue;
            // cout<<"receiving from "<<i<<" to "<<rank<<endl;
            MPI_Recv(recv_buf, 3, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int xi = recv_buf[0],yi=recv_buf[1],val = recv_buf[2];
            xi-=32*(rank/2);
            yi-=32*(rank%2);
            if(dist[xi][yi]>val + 1 && dist[xi][yi]>=0){
                dist[xi][yi] = val+1;
                q.insert({val+1,xi,yi});
            }
        }
        int sz = q.size();
        MPI_Allreduce(&sz, &total_sz, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        
        if(total_sz==0){

            break;}

    }
}

vector<vector<string>> solve_maze_Dijkstra(vector<vector<string>>maze,vector<vector<int>>&dist){

     vector<vector<int>>adj_S = {{-1,0},{0,-1},{0,1},{1,0}};
  
        vector<vector<string>> res(maze.size(),vector<string>(maze[0].size()));

        for (int i=0;i<maze.size();i++){
            for(int j=0;j<maze[0].size();j++){
                res[i][j] = maze[i][j];
                
            }
        }
        
        int d = dist[Msize][0],x=Msize,y=0;
        while(1){
            // cout<<"in loop"<<endl;
            // cout<<x<<" "<<y<<endl;
            res[x][y] = "O";
            if(x==0 && y==Msize)break;
            for(auto it:adj_S){
                int xi = x+it[0],yi=y+it[1];
                if(xi<0 || xi>Msize || yi<0 || yi> Msize || dist[xi][yi]<0)continue;
                if(dist[xi][yi]==d-1){

                    d--;
                    x=xi;y=yi;
                    break;
                }
            }

        }


        return res;



}
