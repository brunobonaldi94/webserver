#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class VectorUtils
{
private:
  VectorUtils();
  ~VectorUtils();
public:
  static void FillVectorFromArray(std::vector<T> &vector, T values[], int size);
  static T* SafeFindVector(std::vector<T> &vector, T key);
  static bool hasElement(std::vector<T> &container, T &element);
  static void SafeRemoveVector(std::vector<T> &vector, T key);
};

#include "../srcs/utils/VectorUtils.tpp"
