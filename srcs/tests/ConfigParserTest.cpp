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

TEST_CASE("ParseConfigFile_ServerMissingCurlyBraces_ReturnsFalse")
{
    //arrange
    std::string fileName = "server_missing_curly_braces.conf";
    ConfigParser configParser(fileName);

    //act
    bool result = configParser.ParseConfigFile();
  
    //assert
    FAST_CHECK_UNARY_FALSE(result);
}

TEST_CASE("ParseConfigFile_ConfFileWithWrongExtension_RaiseWrongFileException")
{
    //arrange
    std::string fileName = "webserver.conf1";
    
    //assert
    CHECK_THROWS_MESSAGE(ConfigParser configParser(fileName), "Wrong file extension");
}