#include "MapUtils.hpp"

template <typename T, typename K>
void MapUtils<T, K>::FillMapFromArray(std::map<T,K> &map, T* keys, K* values, int size)
{
    for (int i = 0; i < size; i++)
        map[keys[i]] = values[i];
}

template <typename T, typename K>
std::pair<const T, K>* MapUtils<T, K>::SafeFindMap(std::map<T, K> &map, T key)
{
    typename std::map<T, K>::iterator it = map.find(key);
    if (it == map.end())
        return NULL;
    return &(*it);
}