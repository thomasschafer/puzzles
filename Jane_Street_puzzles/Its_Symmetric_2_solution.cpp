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
    vector<vector<int>> helper_arr,
    int cur_row,
    int cur_col,
    vector<int> shade_combination
) {
    int up = shade_combination[0];
    int right = shade_combination[1];
    int down = shade_combination[2];
    int left = shade_combination[3];
    // Shade updwards
    for (int i = 0; i <= up; i++) {
        if (cur_row - i < 0 && up > 1) {
            return false;
        }
        if (cur_row - i >= 0) {
            if (helper_arr[cur_row - i][cur_col] > 0) {
                return false;
            }
            if (arr[cur_row - i][cur_col] < 1) {
                arr[cur_row - i][cur_col] = 1;
            }
        }
    }
    int i = up + 1;
    if (cur_row - i >= 0) {
        if (arr[cur_row - i][cur_col] > 0) {
            return false;
        }
        helper_arr[cur_row - i][cur_col] = 1;
    }
    // Shade rightwards
    for (int i = 0; i <= right + 1; i++) {
        if (cur_col + i >= arr[0].size() and right > 1) {
            return false;
        }
        if (cur_col + i < arr[0].size()) {
            if (helper_arr[cur_row][cur_col + i] > 0) {
                return false;
            }
            if (arr[cur_row][cur_col + i] < 1) {
                arr[cur_row][cur_col + i] = 1;
            }
        }
    }
    i = right + 1;
    if (cur_col + i < arr[0].size()) {
        if (arr[cur_row][cur_col + i] > 0) {
            return false;
        }
        helper_arr[cur_row][cur_col + i] = 1;
    }
    // Shade downwards
    for (int i = 0; i <= down + 1; i++) {
        if ((cur_row + i >= arr.size()) and down > 1) {
            return false;
        }
        if (cur_row + i < arr.size()) {
            if (helper_arr[cur_row + i][cur_col] > 0) {
                return false;
            }
            if (arr[cur_row + i][cur_col] < 1) {
                arr[cur_row + i][cur_col] = 1;
            }
        }
    }
    i = down + 1;
    if (cur_row + i < arr.size()) {
        if (arr[cur_row + i][cur_col] > 0) {
            return false;
        }
        helper_arr[cur_row + i][cur_col] = 1;
    }
    // Shade leftwards
    for (int i = 0; i <= left + 1; i++) {
        if (cur_col - i < 0 && left > 1) {
            return false;
        }
        if (cur_col - i >= 0) {
            if (helper_arr[cur_row][cur_col - i] > 0) {
                return false;
            }
            if (arr[cur_row][cur_col - i] < 1) {
                arr[cur_row][cur_col - i] = 1;
            }
        }
    }
    i = left + 1;
    if (cur_col - i >= 0) {
        if (arr[cur_row][cur_col - i] > 0) {
            return false;
        }
        helper_arr[cur_row][cur_col - i] = 1;
    }
    return true;
}


vector<vector<int>> rotate_by_90_cw(const vector<vector<int>> &arr) {
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


vector<vector<int>> flip_vertically(const vector<vector<int>> &arr) {
    vector<vector<int>> arr_flipped;
    for (auto row : arr) {
        arr_flipped.insert(arr_flipped.begin(), row);
    }
    return arr_flipped;
}


vector<vector<int>> flip_horizontally(const vector<vector<int>> &arr) {
    vector<vector<int>> arr_flipped;
    for (auto row : arr) {
        vector<int> row_flipped;
        for (auto x : row) {
            row_flipped.insert(row_flipped.begin(), x);
        }
        arr_flipped.push_back(row_flipped);
    }
    return arr_flipped;
}


bool is_symmetric(const vector<vector<int>> &arr) {
    vector<vector<int>> rotated_arr = arr;
    for (int i = 0; i <= 3; i++) {
        if (
            (i > 0 && rotated_arr == arr) ||
            (flip_horizontally(rotated_arr) == arr) ||
            (flip_vertically(rotated_arr) == arr)
        ) return true;
        rotated_arr = rotate_by_90_cw(arr);
    }
    return false;
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

void print_2d_vector(vector<vector<int>> const &arr) {
    for (auto row  : arr) {
        for (auto x : row) {
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


int main() {
    return 0;
}

