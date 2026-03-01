#include <vector>
#include <cmath>
#include <algorithm>
#include "utils.h"


void degree_collection(std::pair<std::vector<int>, std::vector<int>>& degree_list, std::vector<std::pair<std::vector<int>, std::vector<int>>>& neighbor_list, int d_max_plus, int d_max_minus, int node_num, double epsilon)
{
    int i;
    int d_i_plus;
    int d_i_minus;

    int j;

    int hat_d_i_plus = 0;
    int hat_d_i_minus = 0;

 

   
    for (i = 0; i < node_num; i++)
    {   
        d_i_plus = neighbor_list[i].first.size();
        d_i_minus = neighbor_list[i].second.size();
        
        std::vector<double> p_plus;
        std::vector<double> p_minus;
        double p_plus_sum = 0;
        double p_minus_sum = 0; 

    
        for (j = 0; j <= d_max_plus; j++)
        {
            p_plus.emplace_back(exp((-epsilon * abs(std::min(d_i_plus, d_max_plus) - j))/(2 * d_max_plus)));
            p_plus_sum += p_plus[j];
        }
        
        
        double q_plus = genrand_real2();
        double left_cdf_plus = 0;
        for (j = 0; j <= d_max_plus; j++)
        {
            if (j > 0)
            {
                left_cdf_plus += p_plus[j - 1]/p_plus_sum;
            }
            if (left_cdf_plus <= q_plus)
            {
                hat_d_i_plus = j;
            }
            else
            {
                break;
            }
        }

        for (j = 0; j <= d_max_minus; j++)
        {
            p_minus.emplace_back(exp((-epsilon * abs(std::min(d_i_minus, d_max_minus) - j))/(2 * d_max_minus)));
            p_minus_sum += p_minus[j];
        }

        double q_minus = genrand_real2();
        double left_cdf_minus = 0;
        for (j = 0; j <= d_max_minus; j++)
        {
            if (j > 0)
            {
                left_cdf_minus += p_minus[j - 1]/p_minus_sum;
            }
            if (left_cdf_minus <= q_minus)
            {
                hat_d_i_minus = j;
            }
            else
            {
                break;
            }
        }

        degree_list.first.emplace_back(hat_d_i_plus);
        degree_list.second.emplace_back(hat_d_i_minus);
    }
}