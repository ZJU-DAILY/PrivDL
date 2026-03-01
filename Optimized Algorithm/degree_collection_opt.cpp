#include <vector>
#include <cmath>
#include <algorithm>
#include "utils.h"



void degree_collection_opt(std::pair<std::vector<int>, std::vector<int>>& degree_list, std::vector<std::pair<std::vector<int>, std::vector<int>>>& neighbor_list, int d_max_plus, int d_max_minus, int node_num, int window_size, double epsilon_one, double epsilon_two, double epsilon_three, double epsilon_four, int theta, std::vector<std::pair<std::vector<int>, std::vector<int>>>& degree_list_each_user, int cur_timestamp)
{
    int i;
    //int eta_u_plus = sqrt(node_num);
    //int eta_l_plus = log(node_num);
    //int eta_u_minus = sqrt(node_num);
    //int eta_l_minus = log(node_num);
    int eta_u_plus = 0;
    int eta_l_plus = 0;
    int eta_u_minus = 0;
    int eta_l_minus = 0;

    degree_list_each_user.emplace_back();
    

    for (i = 0; i < node_num; i++)
    {
        for (int sign = 0; sign <= 1; sign++)
        {
            double hat_eta_u, hat_eta_l;
            if (sign == 0)
            {
                hat_eta_u = eta_u_plus + stats::rlaplace(0.0, window_size * d_max_plus/epsilon_one, engine);
                hat_eta_l = eta_l_plus + stats::rlaplace(0.0, window_size * d_max_plus/epsilon_two, engine);
            }
            else
            {
                hat_eta_u = eta_u_minus + stats::rlaplace(0.0, window_size * d_max_minus/epsilon_one, engine);
                hat_eta_l = eta_l_minus + stats::rlaplace(0.0, window_size * d_max_minus/epsilon_two, engine);
            }

            double chi;
            int d_i_plus = neighbor_list[i].first.size();
            int d_i_minus = neighbor_list[i].second.size();
            if(sign == 0)
            {
                chi = std::min(d_i_plus, d_max_plus) + stats::rlaplace(0.0, 2 * window_size * d_max_plus/epsilon_three, engine);
            }
            else
            {
                chi = std::min(d_i_minus, d_max_minus) + stats::rlaplace(0.0, 2 * window_size * d_max_minus/epsilon_three, engine);
            }

            if ((chi <= hat_eta_l) || (chi >= hat_eta_u))
            {
                if (sign == 0)
                {
                    int j;
                    int hat_d_i_plus = 0;

                    //int step_plus = d_max_plus / de_flatten;

                    std::vector<double> p_plus;
                    double p_plus_sum = 0; 

                    for (j = 0; j * theta <= d_max_plus; j++)
                    {
                        p_plus.emplace_back(exp((-epsilon_four * abs(std::min(d_i_plus, d_max_plus) - j * theta))/(2 * window_size * d_max_plus)));
                        p_plus_sum += p_plus[j];
                    }

                    /*
                    for (j = 0; j <= d_max_plus; j++)
                    {
                        p_plus.emplace_back(exp((-epsilon_four * abs(std::min(d_i_plus, d_max_plus) - j * step_plus))/(2 * window_size * d_max_plus)));
                        p_plus_sum += p_plus[j];
                    }

                    */
                        
                    double q_plus = genrand_real2();
                    double left_cdf_plus = 0;
                    for (j = 0; j * theta <= d_max_plus; j++)
                    {
                        if (j > 0)
                        {
                            left_cdf_plus += p_plus[j - 1]/p_plus_sum;
                        }
                        if (left_cdf_plus <= q_plus)
                        {
                            hat_d_i_plus = j * theta;
                        }
                        else
                        {
                            break;
                        }
                    }

                    degree_list.first.emplace_back(hat_d_i_plus);
                    degree_list_each_user[cur_timestamp].first.emplace_back(hat_d_i_plus);
                }
                else
                {
                    int j;
                    int hat_d_i_minus = 0;

                    //int step_minus = d_max_minus / de_flatten;

                    std::vector<double> p_minus;
                    double p_minus_sum = 0; 

                    for (j = 0; j * theta <= d_max_minus; j++)
                    {
                        p_minus.emplace_back(exp((-epsilon_four * abs(std::min(d_i_minus, d_max_minus) - j * theta))/(2 * window_size * d_max_minus)));
                        p_minus_sum += p_minus[j];
                    }
                    
                    /*
                    for (j = 0; j <= d_max_minus; j++)
                    {
                        p_minus.emplace_back(exp((-epsilon_four * abs(std::min(d_i_minus, d_max_minus) - j))/(2 * window_size * d_max_minus)));
                        p_minus_sum += p_minus[j];
                    }
                    */
                        
                    double q_minus = genrand_real2();
                    double left_cdf_minus = 0;
                    for (j = 0; j * theta <= d_max_minus; j++)
                    {
                        if (j > 0)
                        {
                            left_cdf_minus += p_minus[j - 1]/p_minus_sum;
                        }
                        if (left_cdf_minus <= q_minus)
                        {
                            hat_d_i_minus = j * theta;
                        }
                        else
                        {
                            break;
                        }
                    }

                    degree_list.second.emplace_back(hat_d_i_minus);
                    degree_list_each_user[cur_timestamp].second.emplace_back(hat_d_i_minus);
                }
            }
            else
            {
                if (sign == 0)
                {
                    if (cur_timestamp == 0)
                    {
                        degree_list.first.emplace_back(0);
                        degree_list_each_user[cur_timestamp].first.emplace_back(0);
                    }
                    else
                    {
                        degree_list.first.emplace_back(degree_list_each_user[cur_timestamp - 1].first[i]);
                        degree_list_each_user[cur_timestamp].first.emplace_back(degree_list_each_user[cur_timestamp - 1].first[i]);
                    }
                }
                else
                {
                    if (cur_timestamp == 0)
                    {
                        degree_list.second.emplace_back(0);
                        degree_list_each_user[cur_timestamp].second.emplace_back(0);
                    }
                    else
                    {
                        degree_list.second.emplace_back(degree_list_each_user[cur_timestamp - 1].second[i]);
                        degree_list_each_user[cur_timestamp].second.emplace_back(degree_list_each_user[cur_timestamp - 1].second[i]);
                    }
                    
                }
            }
        }
    }
    
}