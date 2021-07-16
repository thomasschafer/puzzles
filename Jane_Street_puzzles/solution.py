from typing import List, Tuple
import numpy as np


def factors(n: int, memo: dict) -> List[List[int]]:
    if n in memo:
        return memo[n]
    results = [[n]]
    for i in range(2, int(n**(1/2)+1)):
        if n % i == 0:
            for l in factors(int(n / i), memo):
                if i <= min(l):
                    factor_list = [i] + l
                    results.append(factor_list)
    return results


def padded_factors_of_fixed_length(n: int, length: int, padding_nums: List[int]) -> List[List[int]]:
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


def shade(arr: List[List[int]], helper_arr: List[List[int]], cur_row: int, cur_col: int, shade_combination: List[int]):
    # Folllow shade_combination in each direction: if val <= 0 then val -= 1 (<0 means shaded),
    #   if helper_arr at position of val is > 0 then another shade combination ends just before
    #   that cell, so return False as otherwise we would be extending the shaded region that
    #   another cell can 'see'
    # If shading was completed successfully then return True
    up, right, down, left = shade_combination
    # Shade updwards
    for i in range(1, up + 1):
        if cur_row - i < 0 and up > 1:
            return False
        if cur_row - i >= 0:
            if helper_arr[cur_row - i][cur_col] > 0:
                return False
            arr[cur_row - i][cur_col] = -1
    i = up + 1
    if cur_row - i >= 0:
        if arr[cur_row - i][cur_col] < 0:
            return False
        helper_arr[cur_row - i][cur_col] = 1
    # Shade rightwards
    for i in range(1, right + 1):
        if cur_col + i >= len(arr[0]) and right > 1:
            return False
        if cur_col + i < len(arr[0]):
            if helper_arr[cur_row][cur_col + i] > 0:
                return False
            arr[cur_row][cur_col + i] = -1
    i = right + 1
    if not cur_col + i >= len(arr[0]):
        if arr[cur_row][cur_col + i] < 0:
            return False
        helper_arr[cur_row][cur_col + i] = 1
    # Shade downwards
    for i in range(1, down + 1):
        if (cur_row + i >= len(arr)) and down > 1:
            return False
        if cur_row + i < len(arr):
            if arr[cur_row + i][cur_col] > 0:
                return False
            arr[cur_row + i][cur_col] = -1
    i = down + 1
    if cur_row + i < len(arr):
        if arr[cur_row + i][cur_col] < 0:
            return False
        helper_arr[cur_row + i][cur_col] = 1
    # Shade leftwards
    for i in range(1, left + 1):
        if cur_col - i < 0 and left > 1:
            return False
        if cur_col - i >= 0:
            if arr[cur_row][cur_col - i] > 0:
                return False
            arr[cur_row][cur_col - i] = -1
    i = left + 1
    if not cur_col - i < 0:
        if arr[cur_row][cur_col - i] < 0:
            return False
        helper_arr[cur_row][cur_col - i] = 1
    return True


def is_symmetric(arr: List[List[int]]) -> bool:
    return True

def is_connected(arr: List[List[int]]) -> bool:
    return True

def sum_of_squares_of_connected_areas(arr: List[List[int]]) -> int:
    return 0

def next_position(cur_row: int, cur_col: int, end_col: int) -> Tuple[int, int]:
    if cur_col == end_col:
        cur_row += 1
        cur_col = 0
    else:
        cur_col += 1
    return cur_row, cur_col

def solve_rec(arr: List[List[int]], helper_arr: List[List[int]], cur_row: int, cur_col: int, end_row: int, end_col: int):
    while cur_row <= end_row:
        val = arr[cur_row][cur_col]
        if val > 0:
            for shade_combination in [
                    perm
                    for factors in padded_factors_of_fixed_length(val, 4, [0, 1])
                    for perm in permutations(factors)
            ]:
                arr_before_shading = [row.copy() for row in arr]
                helper_arr_before_shading = [row.copy() for row in helper_arr]
                success = shade(arr, helper_arr, cur_row, cur_col, shade_combination)
                if success:
                    new_row, new_col = next_position(cur_row, cur_col, end_col)
                    solve_rec(arr, helper_arr, new_row, new_col, end_row, end_col)
                arr = arr_before_shading
                helper_arr = helper_arr_before_shading
            return
        cur_row, cur_col = next_position(cur_row, cur_col, end_col)
    print('Possible solution, checking')
    if is_symmetric(arr) and is_connected(arr):
        print("Solution found:\n", np.array(arr))
        print("Sum of squares of areas of connected regions:",
            sum_of_squares_of_connected_areas(arr))

    # Shade from numbers based on possible permutations, with 'end' (e.g. -1) placeholder at end
    # Shade remaining, using backtracking with:
    # Check if symmetric (rotation or reflection) and connected
    # If all of the above hold, print the solution and then backtrack
    return

def solve(arr: List[List[int]]):
    num_rows, num_cols = len(arr), len(arr[0])
    helper_arr = [[0 for __ in range(num_cols)] for __ in range(num_rows)]
    solve_rec(arr, helper_arr, 0, 0, num_rows-1, num_cols-1)

# Note: value of 0 means no number and no shade, values of <0 means shaded




arr = [[0, 0, 8, 0, 0],
       [0, 0, 0, 0, 2],
       [0, 0, 4, 0, 0],
       [3, 0, 0, 0, 0],
       [0, 0, 2, 0, 0]]

expected_result = 37

arr = [[0, 0, 8, 0, 0],
       [0, 0, 0, 0, 2],
       [0, 0, 0, 0, 0],
       [3, 0, 0, 0, 0],
       [0, 0, 2, 0, 0]]

# arr = [[0, 0, 8, 0, 0],
#        [0, 0, 0, 0, 0],
#        [0, 0, 0, 0, 0],
#        [0, 0, 0, 0, 0],
#        [0, 0, 2, 0, 0]]

solve(arr)
