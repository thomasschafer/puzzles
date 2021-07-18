#include <cmath>
#include <iostream>
#include <map>
#include <vector>

using std::vector;
using std::map;


vector<vector<int>> factors(int n, map<int, vector<vector<int>>> memo) {
    if (memo.count(n) > 0) {
        return memo[n];
    }
    vector<vector<int>> results = {{n}};
    for (int i = 2; i <= std::pow(n, 1.0/2.0)+1; i++) {
        if (n % i == 0) {
            for (auto l : factors(n / i, memo)) {
                if (i <= *std::min_element(std::begin(l), std::end(l))) {
                    vector<int> factor_list = {i};
                    factor_list.insert(factor_list.end(), l.begin(), l.end());
                    results.push_back(factor_list);
                }
            }
        }
    }
    return results;
}

vector<vector<int>> factors(int n) {
    map<int, vector<vector<int>>> memo;
    return factors(n, memo);
}


vector<vector<int>> padded_factors_of_fixed_length(int n, int length, vector<int> padding_nums) {
    vector<vector<int>> factor_list;
    for (auto l : factors(n)) {
        if (l.size() <= length) {
            factor_list.push_back(l);
        }
    }
    vector<vector<int>> output;
    for (auto l : factor_list) {
        vector<vector<int>> factors_helper = {l};
        int cur_length = l.size();
        while (cur_length < length) {
            vector<vector<int>> new_factors_helper;
            for (auto factor_list : factors_helper) {
                for (auto padding_num : padding_nums) {
                    vector<int> factor_list_copy = factor_list;
                    factor_list_copy.push_back(padding_num);
                    new_factors_helper.push_back(factor_list_copy);
                }
            }
            factors_helper = new_factors_helper;
            cur_length += 1;
        }
        for (auto l : factors_helper) {
            output.push_back(l);
        }
    }
    return output;
}


int main() {
    return 0;
}

