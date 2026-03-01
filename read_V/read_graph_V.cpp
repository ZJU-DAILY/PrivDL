#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "utils.h"



int sample(std::vector<std::vector<std::pair<std::vector<int>, std::vector<int>>>>& neighbor_list, std::vector<std::vector<std::pair<std::vector<int>, std::vector<int>>>>& sample_neighbor_list, int T, int node_num, double sample_rate)
{

    int i;
     //every timestamps
     for (i = 0; i < T; i++)
     {
        sample_neighbor_list.emplace_back(std::vector<std::pair<std::vector<int>, std::vector<int>>>());
     }   


    int sample_num = (int)(node_num * sample_rate);
    std::vector<int> vertex_map;
    std::map<int, int> reverse_vertex_map;
    std::vector<int> rndperm;
    make_rnd_perm(rndperm, node_num, node_num);
    
    for (i = 0; i < node_num; i++)
    {
        if (rndperm[i] < sample_num)
        {
            vertex_map.emplace_back(i);
            reverse_vertex_map[i] = vertex_map.size() - 1;
            for (int j = 0; j < T; j++)
            {
                sample_neighbor_list[j].emplace_back();
            }
        }
    }

    for (int j = 0; j < T; j++)
    {
        for (i = 0; i < sample_num; i++)
        {
            int raw_ver_num = vertex_map[i];

            int k;

            for (k = 0; k < neighbor_list[j][raw_ver_num].first.size(); k++)
            {
                int pos_nei = neighbor_list[j][raw_ver_num].first[k];
                if (rndperm[pos_nei] < sample_num)
                {
                    sample_neighbor_list[j][i].first.emplace_back(reverse_vertex_map[pos_nei]);
                }
            }

            for (k = 0; k < neighbor_list[j][raw_ver_num].second.size(); k++)
            {
                int neg_nei = neighbor_list[j][raw_ver_num].second[k];
                if (rndperm[neg_nei] < sample_num)
                {
                    sample_neighbor_list[j][i].second.emplace_back(reverse_vertex_map[neg_nei]);
                }
            }
        }
    }
    return sample_num;
}





int read_graph_V(std::vector<std::vector<std::pair<std::vector<int>, std::vector<int>>>>& sample_neighbor_list, std::string filename, int T, double sample_rate, double& avg_max_pos_deg, double& avg_max_neg_deg)
{

    std::vector<std::vector<std::pair<std::vector<int>, std::vector<int>>>> neighbor_list;


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
                
    }
    


    int sample_num = sample(neighbor_list, sample_neighbor_list, T, node_num, sample_rate);


    avg_max_pos_deg = 0;
    avg_max_neg_deg = 0;


    for (i = 0; i < T; i++)
    {
       
        int max_pos_deg = 0;
        int max_neg_deg = 0;
        for (j = 0; j < sample_num; j++)
        {
            int cur_pos_deg =  sample_neighbor_list[i][j].first.size();
            max_pos_deg = std::max(max_pos_deg, cur_pos_deg);
        }
        avg_max_pos_deg += max_pos_deg;
        for (j = 0; j < sample_num; j++)
        {
            int cur_neg_deg =  sample_neighbor_list[i][j].second.size();
            max_neg_deg = std::max(max_neg_deg, cur_neg_deg);
        }
        avg_max_neg_deg += max_neg_deg;
    }
    avg_max_pos_deg /= T;
    avg_max_neg_deg /= T;


    return sample_num;
}