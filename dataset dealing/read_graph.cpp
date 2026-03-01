#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>




//注意是邻接向量, 有0 也有1
void read_graph(std::vector<std::set<int>>& neighbors, int& node_num, int& edge_num, std::string dataset)
{
    std::ifstream f;
    std::string line;
    f.open(dataset, std::ios::in);
    int count = 0;
    while (std::getline(f, line))
    {
        if (count % 10 == 0)
        {
            std::cout << count << std::endl;
        }
        std::set<int> neighbor;
        std::stringstream ss(line);
        int t;
        int i = 0;
        while (ss >> t)
        {
            if (t == 1)
            {
                neighbor.emplace(i);
            }
            i++;
        }
        neighbors.emplace_back(neighbor);
        count += 1;
    }
    node_num = neighbors.size();
    std::cout << "node_num:" << node_num << std::endl;
    edge_num = 0;
    for (int i = 0; i < node_num; i++)
    {
        edge_num += (neighbors[i].size());
    }
    std::cout << "edge_num:" << edge_num << std::endl;
}

