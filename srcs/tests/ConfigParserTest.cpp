#include "doctest.h"
#include "ConfigParser.hpp"

TEST_CASE("ParseConfigFile_EmptyFile_ReturnsFalse")
{
    //arrange
    std::string fileName = "empty.conf";
    ConfigParser configParser(fileName);

    //act
    bool result = configParser.ParseConfigFile();
    //assert
    FAST_CHECK_UNARY_FALSE(result);
}

TEST_CASE("ParseConfigFile_InvalidFile_ReturnsFalse")
{
    //arrange
    std::string fileName = "invalid.conf";
    ConfigParser configParser(fileName);

    //act
    bool result = configParser.ParseConfigFile();
  
    //assert
    FAST_CHECK_UNARY_FALSE(result);
}

TEST_CASE("ParseConfigFile_NoSemiCollon_ReturnsFalse")
{
    //arrange
    std::string fileName = "directive_no_semi-collon.conf";
    ConfigParser configParser(fileName);

    //act
    bool result = configParser.ParseConfigFile();
  
    //assert
    FAST_CHECK_UNARY_FALSE(result);
}