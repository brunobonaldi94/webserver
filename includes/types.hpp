#include <map>
#include <iostream>
#include <ADirectiveCreator.hpp>
#include <AContextCreator.hpp>
#include "MapUtils.hpp"

class ADirectiveCreator;
class AContextCreator;

typedef  std::map<std::string, ADirectiveCreator *> MapDirCreator;
typedef  std::map<std::string, AContextCreator *> MapContextCreator;