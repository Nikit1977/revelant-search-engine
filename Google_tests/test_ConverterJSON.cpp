#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include "ConverterJSON.h"

#include <fstream>
#include <sstream>


struct TestConverter: public ::testing::Test {
protected:

    ConverterJSON convertor;
};

TEST_F(TestConverter, ConstructConverter) {
    ASSERT_STREQ("..\\config.json", convertor.getPathToConfigFile());
    ASSERT_STREQ("..\\requests.json", convertor.getPathToRequestFile());
    ASSERT_STREQ("..\\answers.json", convertor.getPathToAnswersFile());
}

/*TEST_F(TestConverter, ReadJSONFile) {
    std::string data {R"("config" : "data")"};
    std::istream virtualFile(data);
}
*/
/*TEST_F(TestConverter, EngineVersion) {
    auto ver1 = convertor.getEngineVersionJSON();
    auto ver2 = convertor.getEngineVersion();
    EXPECT_FATAL_FAILURE(ASSERT_STRNE(ver1, ver2), "Config file is missing");
}

void ConverterJSON::checkEngineVersion() {
    if (std::strcmp(getEngineVersion(), getEngineVersionJSON()) != 0) throw ConfigVersionEx();
}*/

//Arrange

//Act

//ASSERT