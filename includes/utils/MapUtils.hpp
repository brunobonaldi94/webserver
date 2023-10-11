#pragma once

#include <iostream>
#include <map>
template <typename T, typename K>
void FillMapFromArray(std::map<T,K> &map, T* keys, K* values, int size);

template <typename T, typename K>
std::pair<const T, K>* SafeFindInMap(std::map<T, K> &map, T key);

#include "../srcs/utils/MapUtils.tpp"