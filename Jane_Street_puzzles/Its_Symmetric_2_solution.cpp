#include <array>
#include <chrono>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using std::vector;


void print_2d_vector(const vector<vector<int>> &arr) {
    for (auto row  : arr) {
        for (auto x : row) {
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


vector<vector<int>> factors(int n, std::map<int, vector<vector<int>>> &memo) {
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
    std::map<int, vector<vector<int>>> memo;
    return factors(n, memo);
}


vector<vector<int>> padded_factors_of_fixed_length(
    int n, int length, const vector<int> &padding_nums
) {
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


std::set<vector<int>> permutations(const vector<int> &l) {
    std::set<vector<int>> perms;
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
    vector<vector<int>> &arr,
    vector<vector<int>> &helper_arr,
    int cur_row,
    int cur_col,
    const vector<int> &shade_combination
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
    for (int i = 0; i <= right; i++) {
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
    for (int i = 0; i <= down; i++) {
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
    for (int i = 0; i <= left; i++) {
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


void clear_connected_component(vector<vector<int>> &arr, int i, int j) {
    if (i < 0 || i >= arr.size() || j < 0 || j >= arr[0].size() || arr[i][j] == 0) {
        return;
    }
    arr[i][j] = 0;
    clear_connected_component(arr, i+1, j);
    clear_connected_component(arr, i, j+1);
    clear_connected_component(arr, i-1, j);
    clear_connected_component(arr, i, j-1);
}


bool is_connected(const vector<vector<int>> &arr) {
    vector<vector<int>> arr_copy = arr;
    int connected_components_found = 0;
    for (int i = 0; i <= arr_copy.size()-1; i++) {
        for (int j = 0; j <= arr_copy[0].size()-1; j++) {
            if (arr_copy[i][j] != 0) {
                if (connected_components_found >= 1) {
                    return false;
                }
                connected_components_found++;
                clear_connected_component(arr_copy, i, j);
            }
        }
    }
    return true;
}


int area_of_connected_unshaded_area(vector<vector<int>> &arr, int i, int j) {
    if (i < 0 || i >= arr.size() || j < 0 || j >= arr[0].size() || arr[i][j] != 0) {
        return 0;
    }
    arr[i][j] = 1;
    return (
        1 +
        area_of_connected_unshaded_area(arr, i+1, j) +
        area_of_connected_unshaded_area(arr, i, j+1) +
        area_of_connected_unshaded_area(arr, i-1, j) +
        area_of_connected_unshaded_area(arr, i, j-1)
    );
}


int sum_of_squares_of_connected_unshaded_areas(const vector<vector<int>> &arr) {
    vector<vector<int>> arr_copy = arr;
    int total_sum = 0;
    for (int i = 0; i <= arr_copy.size()-1; i++) {
        for (int j = 0; j <= arr_copy[0].size()-1; j++) {
            if (arr_copy[i][j] == 0) {
                total_sum += std::pow(area_of_connected_unshaded_area(arr_copy, i, j), 2);
            }
        }
    }
    return total_sum;
}


std::tuple<int, int> next_position(int cur_row, int cur_col, int end_col) {
    if (cur_col == end_col) {
        cur_row += 1;
        cur_col = 0;
    } else {
        cur_col += 1;
    }
    return {cur_row, cur_col};
}


std::string get_logging_filename() {
    std::time_t t = std::time(NULL);
    char cur_datetime[100];
    std::strftime(cur_datetime, 100, "%Y-%m-%d-%T", std::localtime(&t));
    for (int i = 0; i <= sizeof(cur_datetime)/sizeof(cur_datetime[0]); i++) {
        if (cur_datetime[i] == ':') {
            cur_datetime[i] = '_';
        }
    }
    std::string cur_datetime_str = cur_datetime;
    std::string filename = "logs/solutions_file_cpp_" + cur_datetime_str + ".txt";
    return filename;
}

std::string arr_to_string(const vector<vector<int>> &arr) {
    std::string res = "";
    for (auto row : arr) {
        for (auto val : row) {
            res += std::to_string(val) + ", ";
        }
        res += " / ";
    }
    return res;
}


void solve_rec(
    vector<vector<int>> &arr,
    vector<vector<int>> &helper_arr,
    int cur_row,
    int cur_col,
    int end_row,
    int end_col,
    std::set<std::string> &array_states_checked,
    std::ofstream &solutions_file,
    int recursion_depth
) {
    while (cur_row <= end_row) {
        int val = arr[cur_row][cur_col];
        // Assuming that all values are >= 2, which is of course not a good general assumption
        // but works for the test cases given. TODO: fix
        if (val >= 2) {
            std::set<vector<int>> shade_combinations;
            for (auto factors : padded_factors_of_fixed_length(val, 4, {0, 1})) {
                for (auto perm : permutations(factors)) {
                    shade_combinations.insert(perm);
                }
            }
            for (auto shade_combination : shade_combinations) {
                vector<vector<int>> arr_before_shading = arr;
                vector<vector<int>> helper_arr_before_shading = helper_arr;
                bool success = shade(arr, helper_arr, cur_row, cur_col, shade_combination);
                std::string arr_as_string = arr_to_string(arr);
                if (success && (array_states_checked.count(arr_as_string) == 0)) {
                    auto [new_row, new_col] = next_position(cur_row, cur_col, end_col);
                    solve_rec(
                        arr, helper_arr, new_row, new_col, end_row, end_col, array_states_checked,
                        solutions_file, recursion_depth + 1
                    );
                }
                array_states_checked.insert(arr_as_string);
                arr = arr_before_shading;
                helper_arr = helper_arr_before_shading;
            }
            return;
        }
        auto new_positions = next_position(cur_row, cur_col, end_col);
        cur_row = std::get<0>(new_positions);
        cur_col = std::get<1>(new_positions);
        if (recursion_depth <= 20) {
            std::cout << recursion_depth << ": Checked row=" << cur_row << ", col=" << cur_col <<
                std::endl;
        }
    }
    vector<vector<int>> shading_arr;
    for (auto row : arr) {
        vector<int> shading_row;
        for (auto val : row) {
            shading_row.push_back(val != 0);
        }
        shading_arr.push_back(shading_row);
    }
    if (is_symmetric(shading_arr) && is_connected(shading_arr)) {
        std::cout << "Solution found:" << std::endl;
        print_2d_vector(arr);
        int res = sum_of_squares_of_connected_unshaded_areas(arr);
        std::cout << "Sum of squares of areas of connected regions: " << res << std::endl;
        solutions_file << arr_to_string(arr) << std::endl;
        solutions_file << res << std::endl << std::endl;
    }
}


void solve(vector<vector<int>> arr) {
    auto start_time = std::chrono::system_clock::now();
    std::ofstream solutions_file (get_logging_filename());
    int num_rows = arr.size();
    int num_cols = arr[0].size();
    vector<vector<int>> helper_arr;
    for (int i = 0; i <= num_rows; i++) {
        vector<int> row;
        for (int j = 0; j <= num_cols; j++) {
            row.push_back(0);
        }
        helper_arr.push_back(row);
    }
    std::set<std::string> arr_states_checked;
    solve_rec(
        arr, helper_arr, 0, 0, num_rows-1, num_cols-1, arr_states_checked, solutions_file, 1
    );
    auto end_time = std::chrono::system_clock::now();
    double elapsed_seconds = (end_time - start_time).count()/std::pow(10, 6);
    std::cout << "Time taken: " << elapsed_seconds << "s" << std::endl;
    solutions_file << "Time taken: " << elapsed_seconds << "s" << std::endl;
    solutions_file.close();
}


int main() {
    vector<vector<int>> arr = {
        {0, 0, 8, 0, 0},
        {0, 0, 0, 0, 2},
        {0, 0, 4, 0, 0},
        {3, 0, 0, 0, 0},
        {0, 0, 2, 0, 0}
    };
        print_2d_vector(arr);
        solve(arr);
    return 0;
}
