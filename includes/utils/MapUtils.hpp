#pragma once

#include "Logger.hpp"
#include <iostream>
#include <map>

class AContextCreator;
template <typename T, typename K>
class MapUtils
{
private:
  MapUtils();
  ~MapUtils();
public:

  static void FillMapFromArray(std::map<T, K> &map, T* keys, K* values, int size);

  static std::pair<const T, K>* SafeFindMap(std::map<T, K> &map, T key);
  static void SafeRemoveMap(std::map<T, K> &map, T key);

  static void ClearMap(std::map<T, K> &map);
};

#include "../srcs/utils/MapUtils.tpp"