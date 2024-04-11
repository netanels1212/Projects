/*******************************************************************************
Project: Sorting Algorithms
Name: Haytham
Reviewer: ***
Date: 22.11.23
Version: 1.0
*******************************************************************************/

#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h> /*size_t*/


void BubbleSort(int *array, size_t len);

void SelectionSort(int *array, size_t len);

void InsertionSort(int *array, size_t len);

void BubbleSortOptimized(int *array, size_t len);

/* time complexity - O(log n) */
int BinarySearch(int arr_[], size_t len_, int key_);

int *BinarySearchRec(int *begin_, int *end_, int key_);

int MergeSort(int *arr_to_sort_, size_t num_elements_);

void QSort(void *base_, size_t nmemb_, size_t size_, int(*compar)(const void *data1, const void *data2));

#endif /*__SORT_H__*/