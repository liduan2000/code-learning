def binary_search(arr, x):
    def helper(low, high):
        while low <= high:
            mid = (low + high) // 2
            if x > arr[mid]:
                low = mid + 1
            elif x < arr[mid]:
                high = mid - 1
            else:
                return mid
        return -1

    return helper(0, len(arr) - 1)


# l = [1, 2, 3, 4, 5, 6, 7]
# print(binary_search(l, 5))


def selection_sort(arr):
    length = len(arr)
    for i in range(length):
        tmp = i
        for j in range(i + 1, length):
            if arr[j] < arr[tmp]:
                tmp = j
        arr[i], arr[tmp] = arr[tmp], arr[i]
    return arr


def bubble_sort(arr):
    length = len(arr)
    for i in range(1, length):
        for j in range(length - i):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr


def insertion_sort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        while j >= 0 and key < arr[j]:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key
    return arr


def merge_sort(arr):
    if len(arr) < 2:
        return arr
    mid = len(arr) // 2
    l = merge_sort(arr[:mid])
    r = merge_sort(arr[mid:])

    def merge(left, right):
        res = []
        i = j = 0
        while i < len(left) and j < len(right):
            if left[i] < right[j]:
                res.append(left[i])
                i += 1
            else:
                res.append(right[j])
                j += 1
        res.extend(left[i:])
        res.extend(right[j:])
        return res

    return merge(l, r)


import random


def quick_sort(arr, left, right):
    if left < right:
        pivot_index = partition_rand(arr, left, right)
        quick_sort(arr, left, pivot_index)
        quick_sort(arr, pivot_index + 1, right)


def partition_rand(arr, left, right):
    rand_pivot = random.randrange(left, right)
    arr[left], arr[rand_pivot] = arr[rand_pivot], arr[left]
    return partition(arr, left, right)


def partition(arr, left, right):
    pivot = left  # pivot
    i = left - 1
    j = right + 1
    while True:
        while True:
            i = i + 1
            if arr[i] >= arr[pivot]:
                break
        while True:
            j = j - 1
            if arr[j] <= arr[pivot]:
                break
        if i >= j:
            return j
        arr[i], arr[j] = arr[j], arr[i]


array = [10, 7, 8, 9, 1, 5]
array = merge_sort(array)
print(array)
