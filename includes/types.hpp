#include <map>
#include <iostream>
#include <ADirectiveCreator.hpp>
#include <AContextCreator.hpp>
#include <ADirective.hpp>
#include "MapUtils.hpp"

class ADirectiveCreator;
class AContextCreator;
class ADirective;

typedef  std::map<std::string, ADirectiveCreator *> MapDirCreator;
typedef  std::pair<const std::string, ADirectiveCreator *> PairDirCreator;
typedef  std::map<std::string, AContextCreator *> MapContextCreator;
typedef  std::pair<const std::string, AContextCreator *> PairContextCreator;
typedef  std::map<std::string, ADirective*> MapDirectives;
typedef  std::pair<const std::string, ADirective *> PairDirectives; 
typedef  std::map<std::string, std::vector<AContext *> > MapContexts;