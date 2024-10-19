#include "mazegenerator.hpp"


void mazegenerator(string algo){

    vector<vector<string>> maze_g(Msize+1,vector<string>(Msize+1,"X"));

    srand(time(0));
    std::random_device rd;

    mt19937 gen(rd());

    

    if (algo=="kruskal"){
        MPI_Init(NULL,NULL);
        int rank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        uniform_int_distribution<> distrib(0, 3);

        vector<int> local_edge_set = generate_matrix_kruskal(rank);
        vector<int> edge_counts(size);
        if (rank == 0) {
            vector<int>edge_set;
            for(auto it:local_edge_set){
                edge_set.push_back(it);
            }
            int recv_buf[4*((msize)*(msize)-1)];
            for(int i=0;i<3;i++){
                MPI_Recv(recv_buf, 4*((msize)*(msize)-1), MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for(int j=0;j<4*((msize)*(msize)-1);j++){
                    edge_set.push_back(recv_buf[j]);
                }
            }
            vector<pair<pair<int,int>,pair<int,int>>> edges ;
            for(int i=0;i<edge_set.size();i+=4){
                edges.push_back(make_pair(make_pair(edge_set[i],edge_set[i+1]),make_pair(edge_set[i+2],edge_set[i+3])));
            }
            int r = distrib(gen);
            
            int rx;int ry;
            if(r!=0){
                uniform_int_distribution<> distrib0to15(0, 15);
                rx= distrib0to15(gen);
                edges.push_back(make_pair(make_pair(rx,15),make_pair(rx,16)));
            }
            if(r!=2){
                    uniform_int_distribution<> distrib16to31(16, 31);
                    rx = distrib16to31(gen);
                    edges.push_back(make_pair(make_pair(rx,15),make_pair(rx,16)));
            }
            if(r!=1){
                uniform_int_distribution<> distrib20to15(0, 15);
                ry= distrib20to15(gen);
                edges.push_back(make_pair(make_pair(15,ry),make_pair(16,ry)));
            }
            if(r!=3){
                uniform_int_distribution<> distrib216to31(16, 31);
                    ry = distrib216to31(gen);
                    edges.push_back(make_pair(make_pair(15,ry),make_pair(16,ry)));
            }

                
                for (auto it: edges){
                    pair<int,int> p = it.first;
                    pair<int,int> q = it.second; 
                    int x= p.first, y = p.second, w = q.first , z = q.second;
                    x++;y++;w++;z++;
                    maze_g[2*x-1][2*y-1] = "-";
                    maze_g[2*w-1][2*z-1] = "-";
                    maze_g[x+w-1][y+z-1] = "-";


                }
        }

        else{
            int send_buf[4*((msize)*(msize)-1)];
            for(int i=0;i<local_edge_set.size();i++){
                send_buf[i] = local_edge_set[i];
            }
            MPI_Send(send_buf, 4*((msize)*(msize)-1), MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank ==0){
            maze_g[0][Msize]="-";
            maze_g[Msize][0] = "-";
            std::ofstream outfile;
            outfile.open("maze.txt");
            for (int i = 0; i < maze_g.size(); i++) {
                for (int j = 0; j < maze_g[i].size(); j++) {
                    outfile << maze_g[i][j];
                }
                outfile << "\n";
            }
            outfile.close();
        }
    }
    else if (algo=="bfs"){

        MPI_Init(NULL,NULL);
        int rank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int r;
        int rx1,ry1,rx2,ry2;

        uniform_int_distribution<> distrib(0, 3);
        uniform_int_distribution<> distrib0to15(0, 15);
        uniform_int_distribution<> distrib16to31(16, 31);
        uniform_int_distribution<> distrib20to15(0, 15);
        uniform_int_distribution<> distrib216to131(16, 31);


        if(rank==0) {
            r = distrib(gen);
            rx1 = distrib0to15(gen);
            ry1 = distrib20to15(gen);
            rx2 = distrib16to31(gen);
            ry2 = distrib216to131(gen);

        }
        MPI_Bcast(&r,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&rx1,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&ry1,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&rx2,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&ry2,1,MPI_INT,0,MPI_COMM_WORLD);
        // MPI_Barrier(MPI_COMM_WORLD);

        int src[2],dest[2];
        src[0] = 0;src[1] = 0;
        dest[0] = 15;dest[1] = 15;
        if(rank==1){
            src[0] = 0;
            src[1] = 15;
            
            }
        else if(rank==2){
            dest[0] = 15;
            dest[1] = 0;
        }
        vector<pair<pair<int,int>,pair<int,int>>> edges ;
        
        if(r!=0){
                // uniform_int_distribution<> distrib0to15(0, 15);
                //  rx= distrib0to15(gen);
                
                if(rank==0)edges.push_back(make_pair(make_pair(rx1,15),make_pair(rx1,16)));
                if(rank==0){
                    src[0] = rx1;
                    src[1] = 15;
                }
                else if(rank==1 && r/2==1){
                    dest[0] = rx1;
                    dest[1] = 0;
                }
            }
            if(r!=2){
                    // uniform_int_distribution<> distrib16to31(16, 31);
                    // rx2 = distrib16to31(gen);
                    // rx-=16;
                    if(rank==0)edges.push_back(make_pair(make_pair(rx2,15),make_pair(rx2,16)));
                    if(rank==3){
                        dest[0] = rx2-16;
                        dest[1] = 0;
                    }
                    else if(rank==2 && r/2==0){
                        src[0] = rx2-16;
                        src[1] = 15;
                    }

            }
            if(r!=1){
                // uniform_int_distribution<> distrib20to15(0, 15);
                //  ry= distrib20to15(gen);
                
                if(rank==0){
                    dest[0] = 15;
                    dest[1] = ry1;
                    }

                else if(rank==2 && r/2==1){
                    src[0] = 0;
                    src[1] = ry1;
                }
                if(rank==0)edges.push_back(make_pair(make_pair(15,ry1),make_pair(16,ry1)));
            }
            if(r!=3){
                // uniform_int_distribution<> distrib216to31(16, 31);
                //     ry = distrib216to31(gen);
                    // ry-=16;
                    if(rank==3){
                        src[0] = 0;
                        src[1] = ry2-16;
                    }
                    else if(rank==1 && r/2==0){
                        dest[0] = 15;
                        dest[1] = ry2-16;
                    }   
                    if(rank==0)edges.push_back(make_pair(make_pair(15,ry2),make_pair(16,ry2)));
            }

        vector<int>local_edge_set = generate_maze_bfs(src[0],src[1],dest[0],dest[1]);

       

        if (rank == 0 ){
            vector<int>edge_set;
            for(auto it:local_edge_set){
                edge_set.push_back(it);
            }
            for(int i=1;i<size;i++){
                int recv_buf[4*((msize)*(msize)-1)];
                MPI_Recv(recv_buf,4*((msize)*(msize)-1),MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                for(int j=0;j<4*((msize)*(msize)-1);j++){
                    // if(recv_buf[j]!=-1)break;
                    edge_set.push_back(recv_buf[j]);
                }
            }
            for(int i=0;i<edge_set.size();i+=4){
                if(edge_set[i]!=-1)
                    edges.push_back(make_pair(make_pair(edge_set[i],edge_set[i+1]),make_pair(edge_set[i+2],edge_set[i+3])));
            }

            // vector<vector<string>> maze(Msize+1,vector<string>(Msize+1,"X"));
            for (auto it: edges){
                pair<int,int> p = it.first;
                pair<int,int> q = it.second;
                int x= p.first, y = p.second, w = q.first , z = q.second;
                x++;y++;w++;z++;
                maze_g[2*x-1][2*y-1] = "-";
                maze_g[2*w-1][2*z-1] = "-";
                maze_g[x+w-1][y+z-1] = "-";


            }
            maze_g[0][Msize]="-";
            maze_g[Msize][0] = "-";
        }

        else{
            int send_buf[4*((msize)*(msize)-1)];
            for(int i=0;i<4*((msize)*(msize)-1);i++){
                if(i<local_edge_set.size())
                    send_buf[i] = local_edge_set[i];
                else
                    send_buf[i] = -1;
            }
            MPI_Send(send_buf,4*((msize)*(msize)-1),MPI_INT,0,0,MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank ==0){
            maze_g[0][Msize]="-";
            maze_g[Msize][0] = "-";
            std::ofstream outfile;
            outfile.open("maze.txt");
            for (int i = 0; i < maze_g.size(); i++) {
                for (int j = 0; j < maze_g[i].size(); j++) {
                    outfile << maze_g[i][j];
                }
                outfile << "\n";
            }
            outfile.close();
        }
    }



    // MPI_Finalize();
    

}