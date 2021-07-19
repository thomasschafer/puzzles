#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using std::map;
using std::vector;
using std::set;


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


set<vector<int>> permutations(vector<int> l) {
    set<vector<int>> perms;
    if (l.size() <= 1) {
        perms.insert(l);
        return perms;
    }
    vector<int> l_subvector(l.begin() + 1, l.end());
    for (auto short_perm : permutations(l_subvector)) {
        for (int split_index = 0; split_index <= short_perm.size(); split_index++) {
            vector<int> full_perm;
            full_perm.insert(
                full_perm.end(), short_perm.begin(), short_perm.begin() + split_index);
            full_perm.push_back(l[0]);
            full_perm.insert(
                full_perm.end(), short_perm.begin() + split_index, short_perm.end());
            perms.insert(full_perm);
        }
    }
    return perms;
}

bool shade(
    vector<vector<int>> arr,
    vector<vector<int>>  helper_arr,
    int cur_row,
    int cur_col,
    vector<int> shade_combination
) {
    // TODO
    return true;
}


vector<vector<int>> rotate_by_90_cw(vector<vector<int>> arr) {
    vector<vector<int>> arr_rotated;
    for (int i = 0; i <= arr[0].size()-1; i++) {
        vector<int> row;
        for (int j = arr.size()-1; j >= 0; j--) {
            row.push_back(arr[j][i]);
        }
        arr_rotated.push_back(row);
    }
    return arr_rotated;
}

bool is_symmetric(vector<vector<int>> arr) {
    // TODO
    return true;
}


void clear_connected_component(vector<vector<int>> arr, int i, int j) {
    // TODO
}

bool is_connected(vector<vector<int>> arr) {
    // TODO
    return true;
}


int area_of_connected_unshaded_area(vector<vector<int>> arr, int i, int j) {
    // TODO
    return 0;
}


int sum_of_squares_of_connected_unshaded_areas(vector<vector<int>> arr) {
    // TODO
    return 0;
}


std::array<int, 2> next_position(int cur_row, int cur_col, int end_col) {
    std::array<int, 2> new_position;
    // TODO
    return new_position;
}


void solve_rec(
    vector<vector<int>> arr,
    vector<vector<int>> helper_arr,
    int cur_row,
    int cur_col,
    int end_row,
    int end_col,
    set<std::string> array_states_checked,
    std::fstream solutions_file,
    int recursion_depth
) {
    // TODO
}


void solve(vector<vector<int>> arr) {
    // TODO
}


int main() {

    return 0;
}

