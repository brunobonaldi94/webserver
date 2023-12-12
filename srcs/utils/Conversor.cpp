#include "Conversor.hpp"

size_t BytesConversor(std::string byteString)
{
  size_t i;
  for (i = 0; i < byteString.size(); i++)
  {
    if (!std::isdigit(byteString[i]))
      break;
  }
  if (i == byteString.size())
    return std::atoll(byteString.c_str());
  size_t bytes = std::atoll(byteString.substr(0, i).c_str());
  std::string unit = byteString.substr(i, byteString.size() - i);
  if (bytes == 0 && byteString != "0")
    throw std::invalid_argument("Invalid byte string format");
  else if (bytes == 0)
    return 0;
  std::string unitsArr[] = {"B", "K", "M", "G", "T", "P", "E", "Z", "Y"};
  std::vector<std::string> units;
  VectorUtils<std::string>::FillVectorFromArray(units, unitsArr, 9);
  for (size_t i = 0; i < units.size(); i++)
  {
    if (units[i] == StringUtils::UpperCase(unit))
    {
      return bytes * std::pow(1024, i);
    }
  }
  throw std::invalid_argument("Invalid byte string format");
}