#include "VectorUtils.hpp"

template <typename T>
void VectorUtils<T>::FillVectorFromArray(std::vector<T> &vector, T values[], int size)
{
    for (int i = 0; i < size; i++)
        vector.push_back(values[i]);
}

template <typename T>
T* VectorUtils<T>::SafeFindVector(std::vector<T> &vector, T key)
{
    typename std::vector<T>::iterator it = std::find(vector.begin(), vector.end(), key);
    if (it == vector.end())
        return NULL;

    return &(*it);
}

template <typename T>
void VectorUtils<T>::SafeRemoveVector(std::vector<T> &vector, T key)
{
    typename std::vector<T>::iterator it = std::find(vector.begin(), vector.end(), key);
    if (it == vector.end())
        return ;
    vector.erase(it);
}