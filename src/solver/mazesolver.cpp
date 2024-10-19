#include "mazesolver.hpp"

void mazesolver(vector<vector<string>> maze, string algo){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    

    srand(time(0));
    // MPI_Init(NULL,NULL);
    std::random_device rd;

    mt19937 gen(rd());

    uniform_int_distribution<> distrib(0, 3);

    int  size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (algo=="dijkstra"){
        int Msize = 63;    
        int msize = 16;
        int local_dist[32][32];
        
        if(rank==0){
            for(int i=0;i<32;i++){
                for(int j=0;j<32;j++){
                    
                    if(maze[i][j]!="X")local_dist[i][j] = int(1e9)+7;
                    else local_dist[i][j] = -1;
                }
            }
            for(int k=1;k<4;k++){
                int local_dist_to_send[32][32];
                for(int i=0;i<32;i++){
                    for(int j=0;j<32;j++){
                        
                        if(maze[i + (k/2)*32][j + (k%2)*32]!="X")local_dist_to_send[i][j] = int(1e9)+7;
                        else local_dist_to_send[i][j] = -1;
                    }
                }
                MPI_Send(local_dist_to_send, 32*32, MPI_INT, k, 0, MPI_COMM_WORLD);
            }
        
        }
        else{
            MPI_Recv(local_dist, 32*32, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
        }
        if(rank==1){
            local_dist[0][31] = 0;
        }
        Dijkstra(local_dist);
        if(rank==1){
            local_dist[0][31] = 0;
        }
        
        if(rank==0){
            vector<vector<int>>dist(Msize+1,vector<int>(Msize+1));

            int recv_buf_dist[32*32];

            for(int i=1;i<=3;i++){
                MPI_Recv(recv_buf_dist, 32*32, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for(int j=0;j<32*32;j++){
                    dist[j/32 + (i/2)*32][j%32 + (i%2)*32] = recv_buf_dist[j];
                }
            }
            
            for(int i=0;i<32;i++){
                for(int j=0;j<32;j++){
                    dist[i][j] = local_dist[i][j];
                }
            }

            vector<vector<string>> res = solve_maze_Dijkstra(maze,dist);
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
        else{
            int send_buf_dist[32*32];
            for(int i=0;i<32;i++){
                for(int j=0;j<32;j++){
                    send_buf_dist[i*32 + j] = local_dist[i][j];
                }
            }
            MPI_Send(local_dist, 32*32, MPI_INT, 0, 0, MPI_COMM_WORLD);
       }

       
    //    MPI_Barrier(MPI_COMM_WORLD);
       
    }
    if (algo=="dfs"){

        // MPI_Init(NULL,NULL);
        int rank,size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int msize = 32;
        int Msize = 63;
            srand(time(0));

            
            
            cout<<"Rank: "<<rank<<" Starting DFS\n";
            solve_maze_dfs(maze); 


        



    }

}