#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
void FillVectorFromArray(std::vector<T> &vector, T values[], int size);

template <typename T>
T* SafeFindInVector(std::vector<T> &vector, T key);

#include "../srcs/utils/VectorUtils.tpp"