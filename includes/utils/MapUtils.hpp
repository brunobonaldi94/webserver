#pragma once

#include <iostream>
#include <map>

template <typename T, typename K>
class MapUtils
{
private:
  MapUtils();
  ~MapUtils();
public:
  static void FillMapFromArray(std::map<T, K> &map, T* keys, K* values, int size);

  static std::pair<const T, K>* SafeFindMap(std::map<T, K> &map, T key);
};

#include "../srcs/utils/MapUtils.tpp"