#include <vector>
#include <queue>
#include <cmath>

bool cmp_in_degree(std::pair<std::pair<int, int>, int>& u, std::pair<std::pair<int, int>, int>& v)
{
    return u.first.second > v.first.second;
}

bool cmp_id(std::pair<std::pair<int, int>, int>& u, std::pair<std::pair<int, int>, int>& v)
{
    return u.second < v.second;
}

void post_processing(std::vector<std::pair<std::pair<int, int>, int>>& post_degree_list, std::pair<std::vector<int>, std::vector<int>>& degree_list, int node_num)
{
    int i;

    for (i = 0; i < node_num; i++)
    {
        post_degree_list.emplace_back(std::make_pair(std::make_pair(degree_list.first[i], degree_list.second[i]), i));
    }
    
    
    std::sort(post_degree_list.begin(), post_degree_list.end(), cmp_in_degree);   

    
    std::queue<int> Q;
    int k;
    for (k = node_num - 1; k >= 1; k--)
    {
        if (post_degree_list[k - 1].first.second == post_degree_list[k].first.second)
        {
            continue;
        }
        int l = 0;
        int r = 0;
        for (i = 0; i < k; i++)
        {
            l += std::min(post_degree_list[i].first.first, k - 1);
            r += post_degree_list[i].first.second;
        }
        for (i = k; i < node_num; i++)
        {
            l += std::min(post_degree_list[i].first.first, k);
        }
        if (l <= r)
        {
            post_degree_list[k - 1].first.second = post_degree_list[k].first.second;
        }
        else
        {
            Q.push(k);
        }
    }

    int sum_out_degree = 0;
    int sum_in_degree = 0;
    for (i = 0; i < node_num; i++)
    {
        sum_out_degree += post_degree_list[i].first.first;
        sum_in_degree += post_degree_list[i].first.second;
    }
 
    if (sum_out_degree > sum_in_degree)
    {   
        int cur_k;
        int tau;
        int delta;
        bool flag = false;
        while(!Q.empty())
        {
            cur_k = Q.front();
            Q.pop();
            for (i = 0; i < node_num; i++)
            {
                if (i < cur_k)
                {
                    tau = cur_k - 1;
                }
                else
                {
                    tau = cur_k;
                }
                if (post_degree_list[i].first.first > tau)
                {
                    delta = post_degree_list[i].first.first - tau;
                    if (delta >= sum_out_degree - sum_in_degree)
                    {
                        post_degree_list[i].first.first -= (sum_out_degree - sum_in_degree);
                        sum_out_degree = sum_in_degree;
                        flag = true;
                        break;
                    }
                    else
                    {
                        post_degree_list[i].first.first = tau;
                        sum_out_degree -= delta;
                    }
                }
            }
            if (flag)
            {
                break;
            }
        }
        if (sum_out_degree > sum_in_degree)
        {
            for (i = 0; i < node_num; i++)
            {
                if (post_degree_list[i].first.first >= sum_out_degree - sum_in_degree)
                {
                    post_degree_list[i].first.first -= (sum_out_degree - sum_in_degree);
                    sum_out_degree = sum_in_degree;
                    break;
                }
                else
                {
                    sum_out_degree -= post_degree_list[i].first.first;
                    post_degree_list[i].first.first = 0;
                }
            }
        }
        else if (sum_out_degree < sum_in_degree)
        {
            int delta;
            for (i = 0; i < node_num; i++)
            {
                delta = (node_num - 1) - post_degree_list[i].first.first;
                if (delta >= sum_in_degree - sum_out_degree)
                {
                    post_degree_list[i].first.first += (sum_in_degree - sum_out_degree);
                    sum_out_degree = sum_in_degree;
                    break;
                }
                else
                {
                    post_degree_list[i].first.first = node_num - 1;
                    sum_out_degree += delta; 
                }
            }
        }
        
    }
    

    std::sort(post_degree_list.begin(), post_degree_list.end(), cmp_id);  
}