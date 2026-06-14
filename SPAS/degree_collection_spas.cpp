#include <vector>
#include <cmath>
#include <algorithm>
#include "utils.h"





double agv_vardis(int compute_num, int window_size, std::vector<std::pair<std::vector<double>, std::vector<double>>>& degree_list_each_user, int sign, int node_id)
{
      double num_var = 0;
      int total = window_size * compute_num;

      double num_exp = 0;

      int count = 0;

      int len = degree_list_each_user.size();

      if (len >= total + 1)
      {
            for (int i = 0; i < total - 1; i++)
            {
                  if (sign == 0)
                  {
                        double var_dis = pow(degree_list_each_user[len - 1 - i].first[node_id] - degree_list_each_user[len - 2 - i].first[node_id], 2);
                        double var_exp = abs(degree_list_each_user[len - 1 - i].first[node_id] - degree_list_each_user[len - 2 - i].first[node_id]);
                        if (var_dis > 0)
                        {
                              num_var += var_dis;
                              num_exp += var_exp;
                              count += 1;     
                        }
                  }
                  else
                  {
                        double var_dis = pow(degree_list_each_user[len - 1 - i].second[node_id] - degree_list_each_user[len - 2 - i].second[node_id], 2);
                        double var_exp = abs(degree_list_each_user[len - 1 - i].second[node_id] - degree_list_each_user[len - 2 - i].second[node_id]);
                        if (var_dis > 0)
                        {
                              num_var += var_dis;
                              num_exp += var_exp;
                              count += 1;
                        }
                  }
            }
            if (count == 0)
            {
                  return 0;
            }
            else
            {
                  return (num_var/count - pow((num_exp / count), 2)); 
            }
      }
      else
      {
            for (int i = 0; i < len - 2; i++)
            {
                  if (sign == 0)
                  {
                        double var_dis = pow(degree_list_each_user[len - 1 - i].first[node_id] - degree_list_each_user[len - 2 - i].first[node_id], 2);
                        double var_exp = abs(degree_list_each_user[len - 1 - i].first[node_id] - degree_list_each_user[len - 2 - i].first[node_id]);
                        if (var_dis > 0)
                        {
                              num_var += var_dis;
                              num_exp += var_exp;
                              count += 1;
                        }
                  }
                  else
                  {
                        double var_dis = pow(degree_list_each_user[len - 1 - i].second[node_id] - degree_list_each_user[len - 2 - i].second[node_id], 2);
                        double var_exp = abs(degree_list_each_user[len - 1 - i].second[node_id] - degree_list_each_user[len - 2 - i].second[node_id]);
                        if (var_dis > 0)
                        {
                              num_var += var_dis;
                              num_exp += var_exp;
                              count += 1;
                        }            
                  }                        
            }
            if (count == 0)
            {
                  return 0;
            }
            else
            {
                  return (num_var/count - pow((num_exp / count), 2));
            }
      }

}


long long update_optimalc(double epsilon_p, int compute_num, int window_size, std::vector<std::pair<std::vector<double>, std::vector<double>>>& degree_list_each_user, int d_max_plus, int d_max_minus, int sign, int node_id)
{
      int d_max;
      if (sign == 0)
      {
            d_max = d_max_plus;
      }
      else
      {
            d_max = d_max_minus;
      }

      double E_dis = agv_vardis(compute_num, window_size, degree_list_each_user, sign, node_id);

      double theta_ = 0.5;
      double term1 = std::pow(1 - 2 * theta_, 2) * std::pow(window_size, 2);
      double term2 = (3 * std::pow(theta_, 2) * E_dis * std::pow(epsilon_p, 2)) / std::pow(d_max, 2);

      
      double numerator = std::sqrt(term1 + term2) - (1 - 2 * theta_) * window_size;
      double denominator = 6 * theta_;

      
      long long quo = static_cast<long long>(numerator / denominator);
      long long optimal_c;

      if (quo < 1)
      {
            optimal_c = 1;
      }
      else
      {
            optimal_c = quo;
      }

      //long long optimal_c = std::max(static_cast<long long>(numerator / denominator), 1);

      return optimal_c;

}


void warm_up_stage(std::vector<std::vector<std::pair<std::vector<int>, std::vector<int>>>>& neighbor_list, double epsilon, int d_max_plus, int d_max_minus,  std::vector<std::pair<std::vector<double>, std::vector<double>>>& degree_list_each_user, int window_size, int window_num, std::vector<std::pair<std::vector<double>, std::vector<double>>>& eps_consumed, std::vector<std::pair<std::vector<double>, std::vector<double>>>& svt_consumed, int node_num) 
{
    double c_init = 1;
    int sample_interval = static_cast<int>(window_size / c_init);
    int total_for_warmup = window_size * window_num;
    double epsilon_warmup = epsilon / c_init;

   

    for (int i = 0; i < total_for_warmup; i++) {
        degree_list_each_user.emplace_back();
        if (i % sample_interval == 0) {
            for (int node_id = 0; node_id < node_num; node_id++)
            {
                  degree_list_each_user[i].first.emplace_back(neighbor_list[i][node_id].first.size() + stats::rlaplace(0.0, d_max_plus/epsilon_warmup, engine));
                  eps_consumed.emplace_back();
                  svt_consumed.emplace_back();
                  eps_consumed[node_id].first.emplace_back(epsilon_warmup);
                  svt_consumed[node_id].first.emplace_back(0.0);
                  degree_list_each_user[i].second.emplace_back(neighbor_list[i][node_id].second.size() + stats::rlaplace(0.0, d_max_minus/epsilon_warmup, engine));
                  eps_consumed[node_id].second.emplace_back(epsilon_warmup);
                  svt_consumed[node_id].second.emplace_back(0.0);  
            }

        } else {

            for (int node_id = 0; node_id < node_num; node_id++)
            {
                  degree_list_each_user[i].first.emplace_back(degree_list_each_user[i - 1].first[node_id]);
                  eps_consumed.emplace_back();
                  svt_consumed.emplace_back();
                  eps_consumed[node_id].first.emplace_back(0.0);
                  svt_consumed[node_id].first.emplace_back(0.0);
                  degree_list_each_user[i].second.emplace_back(degree_list_each_user[i - 1].second[node_id]);
                  eps_consumed[node_id].second.emplace_back(0.0);
                  svt_consumed[node_id].second.emplace_back(0.0);
            }

            
        }
    }
}


double compute_epsremain(double epsilon_p, std::vector<std::pair<std::vector<double>, std::vector<double>>>& eps_consumed, int window_size, int sign, int node_id)
{
      double eps_con = 0.0;

      if (sign == 0)
      {
            int len = eps_consumed[node_id].first.size();

            for (int i = 0; i < window_size - 1; i++)
            {
                  eps_con += eps_consumed[node_id].first[len - 1 - i];
            }
      }
      else
      {
            int len = eps_consumed[node_id].second.size();

            for (int i = 0; i < window_size - 1; i++)
            {
                  eps_con += eps_consumed[node_id].second[len - 1 - i];
            }
      }

      
      

      return epsilon_p - eps_con;
}

int find_firstsample(std::vector<std::pair<std::vector<double>, std::vector<double>>>& eps_consumed, int window_size, int sign, int node_id)
{

      if (sign == 0)
      {
            int current = eps_consumed[node_id].first.size();
            for (int i = current - window_size + 1; i < current; i++)
            {
                  if (eps_consumed[node_id].first[i] > 0)
                  {
                        return i;
                  }      
            }
            return current;
      }
      else
      {
            int current = eps_consumed[node_id].second.size();
            for (int i = current - window_size + 1; i < current; i++)
            {
                  if (eps_consumed[node_id].second[i] > 0)
                  {
                        return i;
                  }      
            }
            return current;
      }
}


void degree_collection_spas(std::vector<std::vector<std::pair<std::vector<int>, std::vector<int>>>>& neighbor_list, double epsilon, int d_max_plus, int d_max_minus,  std::vector<std::pair<std::vector<double>, std::vector<double>>>& degree_list_each_user, int window_size, int window_warm, int compute_num, int node_num, int Tt)
{
      double epsilon_s = epsilon / 4.0;
      double epsilon_p = epsilon - epsilon_s;
      double eps_1 = epsilon_s / 2.0;
      double eps_2 = epsilon_s - eps_1;

      std::vector<std::pair<std::vector<double>, std::vector<double>>> eps_consumed;
      std::vector<std::pair<std::vector<double>, std::vector<double>>> svt_consumed;
      
      
      warm_up_stage(neighbor_list, epsilon, d_max_plus, d_max_minus, degree_list_each_user, window_size, window_warm, eps_consumed, svt_consumed, node_num);

      std::vector<std::pair<long long, long long>> optimal_c; 
      for (int node_id = 0; node_id < node_num; node_id++)
      {
            long long pos_optimal_c = update_optimalc(epsilon_p, compute_num, window_size, degree_list_each_user, d_max_plus, d_max_minus, 0, node_id);
            long long neg_optimal_c = update_optimalc(epsilon_p, compute_num, window_size, degree_list_each_user, d_max_plus, d_max_minus, 1, node_id);
            optimal_c.emplace_back(std::make_pair(pos_optimal_c, neg_optimal_c));
      }

      int start_ = window_size * window_warm;
      int end_ = Tt;
      
      std::vector<std::pair<double, double>> rho_;

      for (int node_id = 0; node_id < node_num; node_id++)
      {
            double pos_rho_ = stats::rlaplace(0.0, d_max_plus/eps_1, engine);
            double neg_rho_ = stats::rlaplace(0.0, d_max_minus/eps_1, engine);

            rho_.emplace_back(std::make_pair(pos_rho_, neg_rho_));
      }
      
      
      

      
    for (int i = start_; i < end_; i++) 
    {
      degree_list_each_user.emplace_back();
      
      for (int node_id = 0; node_id < node_num; node_id++)
      {
            double pos_eps_remain = compute_epsremain(epsilon_p, eps_consumed, window_size, 0, node_id);
            double neg_eps_remain = compute_epsremain(epsilon_p, eps_consumed, window_size, 1, node_id);
            
            double pos_T_ =  optimal_c[node_id].first * d_max_plus / epsilon_p;
            double neg_T_ =  optimal_c[node_id].second * d_max_minus / epsilon_p;
            
            double pos_diff = std::abs((double)(neighbor_list[i][node_id].first.size()) - (double)(neighbor_list[i - 1][node_id].first.size()));
            double neg_diff = std::abs((double)(neighbor_list[i][node_id].second.size()) - (double)(neighbor_list[i - 1][node_id].second.size()));
            
            double pos_v_ = stats::rlaplace(0.0, (d_max_plus * (2.0 * optimal_c[node_id].first)) / (eps_2), engine);
            double neg_v_ = stats::rlaplace(0.0, (d_max_minus * (2.0 * optimal_c[node_id].second)) / (eps_2), engine);

            int pos_first_sample_inwindow = find_firstsample(eps_consumed, window_size, 0, node_id);
            int neg_first_sample_inwindow = find_firstsample(eps_consumed, window_size, 1, node_id);

            int pos_remaining_steps_in_window = window_size - (i - pos_first_sample_inwindow);
            int neg_remaining_steps_in_window = window_size - (i - neg_first_sample_inwindow);

            if (pos_remaining_steps_in_window <= static_cast<int>(pos_eps_remain / (epsilon_p / optimal_c[node_id].first)))
            {
                  double pos_eps_svt_remain = compute_epsremain(eps_2, svt_consumed, (i - pos_first_sample_inwindow + 1), 0, node_id); 

                  double pos_allocated_eps = pos_eps_remain / pos_remaining_steps_in_window + pos_eps_svt_remain / optimal_c[node_id].first;

                  double tmp = neighbor_list[i][node_id].first.size() + stats::rlaplace(0.0, d_max_plus / pos_allocated_eps, engine);

                  svt_consumed[node_id].first.emplace_back(pos_eps_svt_remain / optimal_c[node_id].first);
                  eps_consumed[node_id].first.emplace_back(pos_eps_remain / pos_remaining_steps_in_window);
                  degree_list_each_user[i].first.emplace_back(tmp);
            }
            else if ((pos_diff + pos_v_ > pos_T_ + rho_[node_id].first) && (pos_eps_remain >= (epsilon_p / optimal_c[node_id].first)))
            {
                  svt_consumed[node_id].first.emplace_back(eps_2 / optimal_c[node_id].first);
                  double pos_eps_svt_remain = compute_epsremain(eps_2, svt_consumed, window_size, 0, node_id);
                  double tmp;
                  if (pos_eps_svt_remain > 0)
                  {
                        tmp = neighbor_list[i][node_id].first.size() + stats::rlaplace(0.0, d_max_plus / (epsilon_p / optimal_c[node_id].first + pos_eps_svt_remain), engine); 
                  }
                  else
                  {
                        tmp = neighbor_list[i][node_id].first.size() + stats::rlaplace(0.0, d_max_plus / (epsilon_p / optimal_c[node_id].first), engine);
                  }

                  degree_list_each_user[i].first.emplace_back(tmp);
                  eps_consumed[node_id].first.emplace_back(epsilon_p / optimal_c[node_id].first);

            }
            else
            {
                  degree_list_each_user[i].first.emplace_back(degree_list_each_user[i - 1].first[node_id]);
                  eps_consumed[node_id].first.emplace_back(0.0);
                  svt_consumed[node_id].first.emplace_back(0.0);
            }
            optimal_c[node_id].first = update_optimalc(epsilon_p, compute_num, window_size, degree_list_each_user, d_max_plus, d_max_minus, 0, node_id);

            if (neg_remaining_steps_in_window <= static_cast<int>(neg_eps_remain / (epsilon_p / optimal_c[node_id].second)))
            {
                  double neg_eps_svt_remain = compute_epsremain(eps_2, svt_consumed, (i - neg_first_sample_inwindow + 1), 1, node_id); 

                  double neg_allocated_eps = neg_eps_remain / neg_remaining_steps_in_window + neg_eps_svt_remain / optimal_c[node_id].second;

                  double tmp = neighbor_list[i][node_id].second.size() + stats::rlaplace(0.0, d_max_minus / neg_allocated_eps, engine);

                  svt_consumed[node_id].second.emplace_back(neg_eps_svt_remain / optimal_c[node_id].second);
                  eps_consumed[node_id].second.emplace_back(neg_eps_remain / neg_remaining_steps_in_window);
                  degree_list_each_user[i].second.emplace_back(tmp);
            }
            else if ((neg_diff + neg_v_ > neg_T_ + rho_[node_id].second) && (neg_eps_remain >= (epsilon_p / optimal_c[node_id].second)))
            {
                  svt_consumed[node_id].second.emplace_back(eps_2 / optimal_c[node_id].second);
                  double neg_eps_svt_remain = compute_epsremain(eps_2, svt_consumed, window_size, 1, node_id);
                  double tmp;
                  if (neg_eps_svt_remain > 0)
                  {
                        tmp = neighbor_list[i][node_id].second.size() + stats::rlaplace(0.0, d_max_minus / (epsilon_p / optimal_c[node_id].second + neg_eps_svt_remain), engine); 
                  }
                  else
                  {
                        tmp = neighbor_list[i][node_id].second.size() + stats::rlaplace(0.0, d_max_minus / (epsilon_p / optimal_c[node_id].second), engine);
                  }

                  degree_list_each_user[i].second.emplace_back(tmp);
                  eps_consumed[node_id].second.emplace_back(epsilon_p / optimal_c[node_id].second);

            }
            else
            {
                  degree_list_each_user[i].second.emplace_back(degree_list_each_user[i - 1].second[node_id]);
                  eps_consumed[node_id].second.emplace_back(0.0);
                  svt_consumed[node_id].second.emplace_back(0.0);
            }
            optimal_c[node_id].second = update_optimalc(epsilon_p, compute_num, window_size, degree_list_each_user, d_max_plus, d_max_minus, 1, node_id);
            

      }
        
    }
      
}
