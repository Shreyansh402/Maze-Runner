#include "kruskal.hpp"


pair<int,int> find_parent(vector<vector<pair<int,int>>>&parent, int x, int y){
    if(parent[x][y] == make_pair(x,y))return parent[x][y];
    return parent[x][y] = find_parent(parent,parent[x][y].first,parent[x][y].second);
    
}
vector<int> generate_matrix_kruskal(int pid){

    int msize = 16;
    

    vector<vector<int>>rank(msize,vector<int>(msize,1));

    vector<vector<int>>group(msize,vector<int>(msize,0));
    vector<vector<pair<int,int>>>parent(msize,vector<pair<int,int>>(msize));

    for(int i=0;i<msize;i++){
        for(int j=0;j<msize;j++){
            parent[i][j] = make_pair(i,j);
        }
    }
    

    vector<pair<pair<int,int>,pair<int,int>>> edge_set;
    vector<int> res;
    for(int i=0;i<msize;i++){
        for(int j=0;j<msize;j++){
            if((i+1)>=0 && i+1<msize ){
                edge_set.push_back(make_pair(make_pair(i,j),make_pair(i+1,j)));
            }
            if((j+1)>=0 && j+1<msize ){
                edge_set.push_back(make_pair(make_pair(i,j),make_pair(i,j+1)));
            }

        }
    }
    
    random_shuffle(edge_set.begin(), edge_set.end()); 
    for(auto it = edge_set.rbegin(); it != edge_set.rend(); ++it){
        pair<int,int> p = it->first, q = it->second;
        int x1 = p.first,y1 = p.second ,x2 = q.first, y2= q.second;
        if (find_parent(parent,x1,y1) == find_parent(parent,x2,y2))continue;
        pair<int,int>p1 = find_parent(parent,x1,y1), p2 = find_parent(parent,x2,y2);
        if(rank[p1.first][p1.second]>rank[p2.first][p2.second]){
            parent[p2.first][p2.second] = p1;
            rank[p1.first][p1.second]+= rank[p2.first][p2.second];
        }
        else{
             parent[p1.first][p1.second] = p2;
             rank[p2.first][p2.second]+= rank[p1.first][p1.second];
        }
        res.push_back(x1+16*(pid/2));
        res.push_back(y1+16*(pid%2));
        res.push_back(x2+16*(pid/2));
        res.push_back(y2+16*(pid%2));
        
    }
    return res;




}