#include <map>
#include <iostream>
#include <ADirectiveCreator.hpp>
#include <AContextCreator.hpp>
#include "MapUtils.hpp"

class ADirectiveCreator;
class AContextCreator;

typedef  std::map<std::string, ADirectiveCreator *> MapDirCreator;
typedef  std::pair<const std::string, ADirectiveCreator *> PairDirCreator;
typedef  std::map<std::string, AContextCreator *> MapContextCreator;
typedef  std::pair<const std::string, AContextCreator *> PairContextCreator;
