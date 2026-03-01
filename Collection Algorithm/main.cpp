#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "read_graph.cpp"
#include "degree_collection.cpp"
#include "post_processing.cpp"

int main()
{
    std::vector<std::vector<std::pair<std::vector<int>, std::vector<int>>>> neighbor_list;
    std::string filename = "Wiki-vote_.txt";
    int T = 100;
    int node_num = read_graph(neighbor_list, filename, T);
    int window_size;
  
    std::pair<int, int> d_max = std::make_pair(893, 434);

    double epsilon = 1.0;

    for (window_size = 9; window_size >= 5; window_size -= 1)
    {
        
        double report_p_lone_norm = 0;
        double report_p_lzero_norm = 0;
        double report_n_lone_norm = 0;
        double report_n_lzero_norm = 0;
        double report_time = 0;

        int repeat_time = 10;

        for (int repeat = 0; repeat < repeat_time; repeat++)
        {
            int t = 0;
            

            double threhold = log(node_num);

            double avg_p_lone_norm = 0;
            double avg_p_lzero_norm = 0;
            double avg_n_lone_norm = 0;
            double avg_n_lzero_norm = 0;
   
            double p_lone_norm;
            double p_lzero_norm;
   
            double n_lone_norm;
            double n_lzero_norm;

            double once_time = 0;
    
            int i;

            for (t = 0; t < T; t++)
            {
                auto start = std::chrono::steady_clock::now();
                std::pair<std::vector<int>, std::vector<int>> degree_list;
                degree_collection(degree_list, neighbor_list[t], d_max.first, d_max.second, node_num, epsilon/window_size);
                std::vector<std::pair<std::pair<int, int>, int>> post_degree_list;
                post_processing(post_degree_list, degree_list, node_num);
                auto end = std::chrono::steady_clock::now();
                once_time += (std::chrono::duration<double>(end - start)).count();

                p_lone_norm = 0;
                p_lzero_norm = 0;

                n_lone_norm = 0;
                n_lzero_norm = 0;
                for (i = 0; i < node_num; i++)
                {
                    p_lone_norm += abs(post_degree_list[i].first.first - neighbor_list[t][i].first.size());
                    if (abs(post_degree_list[i].first.first - neighbor_list[t][i].first.size()) > threhold)
                    {
                        p_lzero_norm += 1;
                    }
            
                    n_lone_norm += abs(post_degree_list[i].first.second - neighbor_list[t][i].second.size());
                    if (abs(post_degree_list[i].first.second - neighbor_list[t][i].second.size()) > threhold)
                    {
                        n_lzero_norm += 1;
                    }
                }



                avg_p_lone_norm += p_lone_norm;
                avg_p_lzero_norm += p_lzero_norm;
        
                avg_n_lone_norm += n_lone_norm;
                avg_n_lzero_norm += n_lzero_norm;
            }

            avg_p_lone_norm /= T;
            avg_p_lzero_norm /= T;
    
            avg_n_lone_norm /= T;
            avg_n_lzero_norm /= T;

            report_p_lone_norm += avg_p_lone_norm;
            report_p_lzero_norm += avg_p_lzero_norm;
            report_n_lone_norm += avg_n_lone_norm;
            report_n_lzero_norm += avg_n_lzero_norm;

            report_time += once_time;
        }

        report_p_lone_norm /= repeat_time;
        report_p_lzero_norm /= repeat_time;
        report_n_lone_norm /= repeat_time;
        report_n_lzero_norm /= repeat_time;
        report_time /= repeat_time;

        std::cout << filename << std::endl;
        std::cout << "T:" << T << std::endl;
        std::cout << "V:" << node_num << std::endl;
        std::cout << "window_size:" << window_size << std::endl;
        std::cout << "epsilon:" << epsilon << std::endl;
        std::cout << "d_max_plus:" << d_max.first << std::endl;
        std::cout << "d_max_minus:" << d_max.second << std::endl;


        std::cout << "report_p_lone_norm:" << report_p_lone_norm << std::endl;
        std::cout << "report_p_lzero_norm:" << report_p_lzero_norm << std::endl;
        std::cout << "report_n_lone_norm:" << report_n_lone_norm << std::endl;
        std::cout << "report_n_lzero_norm:" << report_n_lzero_norm << std::endl;
        std::cout << "report_time:" << report_time << std::endl;
    }  

    return 0;
}