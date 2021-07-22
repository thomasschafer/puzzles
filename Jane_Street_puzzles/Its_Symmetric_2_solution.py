'''
My solution for the  "It's Symmetric 2" puzzle, from the puzzles section of the Jane Street site.
See https://www.janestreet.com/puzzles/archive/
'''

from datetime import datetime
import math
import numpy as np
import time
from typing import List, Tuple
from io import TextIOWrapper


def factors(n: int, memo: dict) -> List[List[int]]:
    if n in memo:
        return memo[n]
    results = [[n]]
    for i in range(2, math.ceil(n/2)+1):
        if n % i == 0:
            for l in factors(int(n / i), memo):
                if i <= min(l):
                    factor_list = [i] + l
                    results.append(factor_list)
    return results


def padded_factors_of_fixed_length(
        n: int, length: int, padding_nums: List[int],
) -> List[List[int]]:
    memo = dict()
    factor_list = [l for l in factors(n, memo) if len(l) <= length]
    output_list_of_lists = []
    for l in factor_list:
        factors_helper = [l]
        cur_length = len(l)
        while cur_length < length:
            new_factors_helper = []
            for factor_list in factors_helper:
                for padding_num in padding_nums:
                    new_factors_helper.append(factor_list + [padding_num])
            factors_helper = new_factors_helper
            cur_length += 1
        output_list_of_lists += factors_helper
    return output_list_of_lists


def permutations(l: List) -> List[List]:
    if len(l) == 0:
        return []
    if len(l) == 1:
        return [l]
    perms = []
    for short_perm in permutations(l[1:]):
        for split_index in range(len(short_perm)+1):
            full_perm = short_perm[:split_index] + [l[0]] + short_perm[split_index:]
            if not full_perm in perms:
                perms.append(full_perm)
    return perms


def shade(
    arr: List[List[int]],
    helper_arr: List[List[int]],
    cur_row: int,
    cur_col: int,
    shade_combination: List[int],
) -> bool:
    up, right, down, left = shade_combination
    # Shade updwards
    for i in range(1, up + 1):
        if cur_row - i < 0 and up > 1:
            return False
        if cur_row - i >= 0:
            if helper_arr[cur_row - i][cur_col] > 0:
                return False
            arr[cur_row - i][cur_col] = max(arr[cur_row - i][cur_col], 1)
    i = up + 1
    if cur_row - i >= 0:
        if arr[cur_row - i][cur_col] > 0:
            return False
        helper_arr[cur_row - i][cur_col] = 1
    # Shade rightwards
    for i in range(1, right + 1):
        if cur_col + i >= len(arr[0]) and right > 1:
            return False
        if cur_col + i < len(arr[0]):
            if helper_arr[cur_row][cur_col + i] > 0:
                return False
            arr[cur_row][cur_col + i] = max(arr[cur_row][cur_col + i], 1)
    i = right + 1
    if not cur_col + i >= len(arr[0]):
        if arr[cur_row][cur_col + i] > 0:
            return False
        helper_arr[cur_row][cur_col + i] = 1
    # Shade downwards
    for i in range(1, down + 1):
        if (cur_row + i >= len(arr)) and down > 1:
            return False
        if cur_row + i < len(arr):
            if helper_arr[cur_row + i][cur_col] > 0:
                return False
            arr[cur_row + i][cur_col] = max(arr[cur_row + i][cur_col], 1)
    i = down + 1
    if cur_row + i < len(arr):
        if arr[cur_row + i][cur_col] > 0:
            return False
        helper_arr[cur_row + i][cur_col] = 1
    # Shade leftwards
    for i in range(1, left + 1):
        if cur_col - i < 0 and left > 1:
            return False
        if cur_col - i >= 0:
            if helper_arr[cur_row][cur_col - i] > 0:
                return False
            arr[cur_row][cur_col - i] = max(arr[cur_row][cur_col - i], 1)
    i = left + 1
    if not cur_col - i < 0:
        if arr[cur_row][cur_col - i] > 0:
            return False
        helper_arr[cur_row][cur_col - i] = 1
    return True


def rotate_by_90_cw(arr: List[List[int]]) -> List[List[int]]:
    # Rotates a 2d array by 90 degrees
    return [list(row) for row in zip(*arr[::-1])]


def is_symmetric(arr: List[List[int]]) -> bool:
    rotated_arr = [row.copy() for row in arr]
    for i in range(4):
        #  Check if rotationally symmetric
        if i > 0 and rotated_arr == arr:
            return True
        # Check if horizontally symmetric
        if [row[::-1] for row in rotated_arr] == arr:
            return True
        # Check if vertically symmetric
        if rotated_arr[::-1] == arr:
            return True
        # Rotate by 90 degrees
        rotated_arr = rotate_by_90_cw(rotated_arr)
    return False


def clear_connected_component(arr: List[List[int]], i: int, j: int):
    if i < 0 or i >= len(arr) or j < 0 or j >= len(arr[0]) or arr[i][j] == 0:
        return
    arr[i][j] = 0
    clear_connected_component(arr, i+1, j)
    clear_connected_component(arr, i, j+1)
    clear_connected_component(arr, i-1, j)
    clear_connected_component(arr, i, j-1)


def is_connected(arr: List[List[int]]) -> bool:
    arr_copy = [row.copy() for row in arr]
    connected_components_found = 0
    for i in range(len(arr_copy)):
        for j in range(len(arr_copy[0])):
            if arr_copy[i][j] != 0:
                if connected_components_found >= 1:
                    return False
                connected_components_found += 1
                clear_connected_component(arr_copy, i, j)
    return True


def area_of_connected_unshaded_area(arr: List[List[int]], i: int, j: int) -> int:
    if i < 0 or i >= len(arr) or j < 0 or j >= len(arr[0]) or arr[i][j] != 0:
        return 0
    arr[i][j] = 1
    return (
        1 +
        area_of_connected_unshaded_area(arr, i+1, j) +
        area_of_connected_unshaded_area(arr, i, j+1) +
        area_of_connected_unshaded_area(arr, i-1, j) +
        area_of_connected_unshaded_area(arr, i, j-1)
    )


def sum_of_squares_of_connected_unshaded_areas(arr: List[List[int]]) -> int:
    arr_copy = [row.copy() for row in arr]
    total_sum = 0
    for i in range(len(arr_copy)):
        for j in range(len(arr_copy[0])):
            if arr_copy[i][j] == 0:
                total_sum += area_of_connected_unshaded_area(arr_copy, i, j)**2
    return total_sum


def next_position(cur_row: int, cur_col: int, end_col: int) -> Tuple[int, int]:
    if cur_col == end_col:
        cur_row += 1
        cur_col = 0
    else:
        cur_col += 1
    return cur_row, cur_col


def solve_rec(
    arr: List[List[int]],
    helper_arr: List[List[int]],
    cur_row: int,
    cur_col: int,
    end_row: int,
    end_col: int,
    array_states_checked: set,
    solutions_file: TextIOWrapper,
    recursion_depth: int = -1,
):
    while cur_row <= end_row:
        val = arr[cur_row][cur_col]
        if val >= 2:
            # Assuming that all values are >= 2, which is of course not a good general assumption
            # but works for the test cases given. TODO: fix
            for shade_combination in [
                    perm
                    for factors in padded_factors_of_fixed_length(val, 4, [0, 1])
                    for perm in permutations(factors)
            ]:
                arr_before_shading = [row.copy() for row in arr]
                helper_arr_before_shading = [row.copy() for row in helper_arr]
                success = shade(arr, helper_arr, cur_row, cur_col, shade_combination)
                arr_as_string = ', '.join([', '.join([str(x) for x in row]) for row in arr])
                if success and not arr_as_string in array_states_checked:
                    array_states_checked.add(arr_as_string)
                    new_row, new_col = next_position(cur_row, cur_col, end_col)
                    solve_rec(
                        arr, helper_arr, new_row, new_col, end_row, end_col, array_states_checked,
                        solutions_file, recursion_depth + 1,
                    )
                arr = arr_before_shading
                helper_arr = helper_arr_before_shading
            return
        cur_row, cur_col = next_position(cur_row, cur_col, end_col)
        if recursion_depth in range(1, 20):
            print(f'{recursion_depth}: Checked row={cur_row}, col={cur_col}')
    shading_arr = [[int(x != 0) for x in row] for row in arr]
    if is_symmetric(shading_arr) and is_connected(shading_arr):
        print("Solution found:\n", np.array(arr))
        res = sum_of_squares_of_connected_unshaded_areas(arr)
        print("Sum of squares of areas of connected regions:", res)
        solutions_file.write(str(np.array(arr)) + '\n')
        solutions_file.write(str(res) + '\n\n')


def solve(arr: List[List[int]]):
    start_time = time.time()
    cur_datetime = str(datetime.now())[:-7].replace(" ", "_").replace(":", "_")
    solutions_file = open(f'logs/solutions_file_{cur_datetime}.txt', 'a')
    num_rows, num_cols = len(arr), len(arr[0])
    helper_arr = [[0 for __ in range(num_cols)] for __ in range(num_rows)]
    array_states_checked = set()
    solve_rec(
        arr, helper_arr, 0, 0, num_rows-1, num_cols-1, array_states_checked, solutions_file, 1)
    time_taken_str = str(round(time.time() - start_time, 3)) + ' seconds'
    print('Time taken: ', time_taken_str)
    solutions_file.write(time_taken_str + '\n')
    solutions_file.close()


# arr = [[0, 0, 8, 0, 0],
#        [0, 0, 0, 0, 2],
#        [0, 0, 4, 0, 0],
#        [3, 0, 0, 0, 0],
#        [0, 0, 2, 0, 0]]

# # example_result = 37

arr = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 27, 0],
    [0, 0, 24, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0],
    [3, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28],
    [0, 3, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 8, 0, 0, 0, 5, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 2, 0],
    [24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 2],
    [0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 70, 0, 0],
    [0, 8, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]


def display_arr(arr: List[List[int]]):
    for row in [['__' if x == 0 else str(x) if x >= 10 else ' ' + str(x) for x in row] for row in arr]:
        print(', '.join(row))


display_arr(arr)

solve(arr)
