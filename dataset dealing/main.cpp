#include <string>
#include <filesystem>
#include "read_graph.cpp"
#include "../stats/include/stats.hpp"
#include "utils.h"

int main()
{
    std::string dataset = "As-Caida_.txt";
    std::vector<std::set<int>> graph_neighbor;
    int node_num;
    int edge_num;
    read_graph(graph_neighbor, node_num, edge_num, dataset);


    int T = 100;
    double add_rate = 0.02;
    double delete_rate = 0.02;
    int add_num = (int)(add_rate * edge_num);
    int del_num = (int)(delete_rate * edge_num);


    int i, j;
    
    int avg_edge = 0;
    int avg_max_pos = 0;
    int avg_max_neg = 0;

    for (i = 0; i < T; i++)
    {
        std::vector<std::set<int>> copy_graph_neighbor = graph_neighbor;
        for (j = 0; j < add_num; j++)
        {
            while (1)
            {
                int u = genrand_int32() % node_num;
                int v = genrand_int32() % node_num;
                if (u != v && !graph_neighbor[u].count(v) && !copy_graph_neighbor[u].count(v))
                {
                    copy_graph_neighbor[u].insert(v);
                    break;
                }
            }
        }

        for (j = 0; j < del_num; j++)
        {
            while (1)
            {
                int u = genrand_int32() % node_num;
                if (copy_graph_neighbor[u].size() > 0 && graph_neighbor[u].size() > 0)
                {
                    int v = genrand_int32() % (graph_neighbor[u].size());
                    std::set<int>::iterator it = graph_neighbor[u].begin();
                    std::advance(it, v);
                    if (copy_graph_neighbor[u].count(*it))
                    {
                        copy_graph_neighbor[u].erase(*it);
                        break;
                    }
                }
            }
        }

        std::ofstream d_f;
        d_f.open("./" + dataset + "_" + std::to_string(i) + ".txt");
        d_f << node_num << std::endl;
        int cur_max_pos = 0;
        std::vector<int> cur_neg(node_num, 0);

        int u;
        for (u = 0; u < node_num; u++)
        {
            for (std::set<int>::iterator it = copy_graph_neighbor[u].begin(); it != copy_graph_neighbor[u].end(); it++)
            {
                d_f << u << " " << *it << std::endl; 
                cur_neg[*it] += 1; 
            }
            avg_edge += copy_graph_neighbor[u].size();
            if (copy_graph_neighbor[u].size() > cur_max_pos)
            {
                cur_max_pos = copy_graph_neighbor[u].size();
            }  
         }
        avg_max_pos += cur_max_pos;
        int cur_max_neg = 0;
        for (u = 0; u < node_num; u++)
        {
            if (cur_neg[u] > cur_max_neg)
            {
                cur_max_neg = cur_neg[u];
            }
        }
        avg_max_neg += cur_max_neg;
        d_f.close();
    }
    std::cout << "avg_edge:" << (double)avg_edge / (double)T << std::endl;
    std::cout << "avg_max_pos:" << (double)avg_max_pos / (double)T << std::endl;
    std::cout << "avg_max_neg:" << (double)avg_max_neg / (double)T << std::endl;

    return 0;
}