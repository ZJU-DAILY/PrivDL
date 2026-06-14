#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

int read_graph(std::vector<std::vector<std::pair<std::vector<int>, std::vector<int>>>>& neighbor_list, std::string filename, int T)
{
    int i;
    int j;
    int node_num;
    for (i = 0; i < T; i++)
    {
        std::ifstream f;
        std::string line;
        f.open(filename + "_" + std::to_string(i) + ".txt", std::ios::in);


        std::getline(f, line);
        std::stringstream ss(line);
        

        ss >> node_num;
        
        neighbor_list.emplace_back(std::vector<std::pair<std::vector<int>, std::vector<int>>>());
        
        for (j = 0; j < node_num; j++)
        {
            neighbor_list[i].emplace_back(std::pair<std::vector<int>, std::vector<int>>());
        }

        int count = 0;
        while (std::getline(f, line))
        {
            std::stringstream ss(line);
            int u, v;
            ss >> u;
            ss >> v;
            neighbor_list[i][u].first.emplace_back(v);
            neighbor_list[i][v].second.emplace_back(u);
            count++;
        }
        f.close();

        
        int max_pos_deg = 0;
        int max_neg_deg = 0;
        for (j = 0; j < node_num; j++)
        {
            int cur_pos_deg =  neighbor_list[i][j].first.size();
            max_pos_deg = std::max(max_pos_deg, cur_pos_deg);
        }
        for (j = 0; j < node_num; j++)
        {
            int cur_neg_deg =  neighbor_list[i][j].second.size();
            max_neg_deg = std::max(max_neg_deg, cur_neg_deg);
        }        
    }
    return node_num;
}