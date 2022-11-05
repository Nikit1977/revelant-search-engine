#include "gtest/gtest.h"
#include "ConverterJSON.h"
#include "ConfigErrorExceptions.h"

#include <fstream>
#include <sstream>


TEST(TestConverterClass, Constructor) {
    ConverterJSON convertor;

    ASSERT_STREQ("..\\config.json", convertor.getPathToConfigFile());
    ASSERT_STREQ("..\\requests.json", convertor.getPathToRequestFile());
    ASSERT_STREQ("..\\answers.json", convertor.getPathToAnswersFile());
}

struct ConvertorMethods : public ::testing::Test {
    ConverterJSON convertor;
    const char* temp_file = "temp_json";
    boost::json::object example = {{"config", {{"name", "SkillboxSearchEngine"}, {"version", "0.1.2"}}},
                                   {"files",  boost::json::array{"../resources/file001.txt",
                                                                  "../resources/file002.txt"}} };
    void SetUp() {
        std::ofstream file(temp_file, std::ios_base::trunc);
        if (file.is_open()) {
            file << example;
            file.close();
        }
        convertor.setPathToConfigFile(temp_file);
    }
};

//файл существует и у него правильный формат
TEST_F(ConvertorMethods, ReadJsonFile_Exists) {

    EXPECT_NO_THROW({
            convertor.readFileJSON(temp_file);
    });
}

//file не существует
TEST_F(ConvertorMethods, ReadJsonFile_file_no_exists) {

    EXPECT_ANY_THROW({
                        convertor.readFileJSON("wrong_name");
                    });
}

//файл существует...
TEST_F(ConvertorMethods, ReadJsonFile_file_is_no_json) {
    //Arrange
    std::ofstream file(temp_file, std::ios_base::app);
    if (file.is_open()) {
        file << "text"; // ...но файл не json
        file.close();
    }
    //Act
    //Assert
    EXPECT_THROW({
                 convertor.readFileJSON(temp_file);
                 },
                 FormatEx);
}

TEST_F(ConvertorMethods, CreateInfo_test_init_configInfo) {
    //Act
    convertor.createConfigInfo(); //создание из тестового файла
    //Assert
    ASSERT_TRUE(example == convertor.getConfigData());
}

TEST_F(ConvertorMethods, GetTextDocuments_test_count_and_paths) {
    //Arrange
    convertor.createConfigInfo();
    std::vector<std::string> checkList {"..\\resources\\file001.txt", "..\\resources\\file002.txt"};

    //Act
    auto testList = convertor.GetTextDocuments();

    //Assert
    ASSERT_TRUE(testList.size() == 2);

    for (int i = 0; i < testList.size(); i++) {
        ASSERT_STREQ(checkList[i].c_str(), testList[i].c_str());
    }

}

TEST_F(ConvertorMethods, GetRequests_count_test) {
    //Arrange
    std::vector<std::string> checkList {"mama", "papa"};
    boost::json::object test_request = { {"requests", boost::json::array{"mama", "papa"}}};
    std::ofstream file(temp_file, std::ios_base::trunc);
    if (file.is_open()) {
        file << test_request;
        file.close();
    }
    convertor.setPathToRequestFile(temp_file);

    //Act
    auto testList = convertor.GetRequests();

    //Assert
    ASSERT_TRUE(testList.size() == 2);

    for (int i = 0; i < testList.size(); i++) {
        ASSERT_STREQ(checkList[i].c_str(), testList[i].c_str());
    }
}

TEST_F(ConvertorMethods, CheckConfigValid_json_has_config_data) {
    //Act
    convertor.createConfigInfo();
    //Assert
    EXPECT_NO_THROW({convertor.checkConfigValid();});
}

TEST_F(ConvertorMethods, CheckConfigValid_if_json_has_no_config_data) {
    //Arrange
    boost::json::object invalid_json = {{"no_config", {{"name", "SkillboxSearchEngine"}, {"version", "0.1.2"}}}};
    std::ofstream file(temp_file, std::ios_base::trunc);
    if(file.is_open()) {
        file << invalid_json;
        file.close();
    }
    //Act
    convertor.createConfigInfo();
    //Assert
    EXPECT_THROW({convertor.checkConfigValid();}, ConfigEmptyEx);
}

TEST_F(ConvertorMethods, CheckConfigValid_if_json_has_empty_config_data) {
    //Arrange
    boost::json::object invalid_json = {{"config", {}}};
    std::ofstream file(temp_file, std::ios_base::trunc);
    if(file.is_open()) {
        file << invalid_json;
        file.close();
    }
    //Act
    convertor.createConfigInfo();
    //Assert
    EXPECT_THROW({convertor.checkConfigValid();}, ConfigEmptyEx);
}

TEST(helperFunc_valuesFrom, value_is_array) {
    //Arrange
    ConverterJSON convertor;
    std::vector<std::string> checkList {"../resources/file001.txt", "../resources/file002.txt"};
    boost::json::object example = { {"files",  boost::json::array{"../resources/file001.txt", "../resources/file002.txt"}} };
    //Act
    auto testList = convertor.values_from("files", example);
    //Assert
    ASSERT_TRUE(testList.size() == 2);

    for (int i = 0; i < testList.size(); i++) {
        ASSERT_STREQ(checkList[i].c_str(), testList[i].c_str());
    }
}

TEST(helperFunc_valuesFrom, value_is_string) {
    //Arrange
    ConverterJSON convertor;
    std::vector<std::string> checkList {"../resources/file001.txt"};
    boost::json::object example = { {"files",  "../resources/file001.txt"} };
    //Act
    auto testList = convertor.values_from("files", example);
    //Assert
    ASSERT_TRUE(testList.size() == 1);

    for (int i = 0; i < testList.size(); i++) {
        ASSERT_STREQ(checkList[i].c_str(), testList[i].c_str());
    }
}

struct GetResponsesLimit_count : public ::testing::Test {
    ConverterJSON convertor;
    const char* temp_file = "temp_json";
    void SetUp() {
        std::ofstream file(temp_file, std::ios_base::trunc);
        if (file.is_open()) {
            file << R"({"config":{"max_responses":)";
            file.close();
        }
        convertor.setPathToConfigFile(temp_file);
    }
};
TEST_F(GetResponsesLimit_count, negative) {
    //Arrange
    std::ofstream file(temp_file, std::ios_base::app);
    if (file.is_open()) {
        file << R"(-1}})";
        file.close();
    }
    //Act
    convertor.createConfigInfo();
    auto test_response = convertor.GetResponsesLimit();
    //Assert
    ASSERT_EQ(test_response, 5);
}

TEST_F(GetResponsesLimit_count, positive) {
    //Arrange
    std::ofstream file(temp_file, std::ios_base::app);
    if (file.is_open()) {
        file << R"(3}})";
        file.close();
    }
    //Act
    convertor.createConfigInfo();
    auto test_response = convertor.GetResponsesLimit();

    //Assert
    ASSERT_EQ(test_response, 3);
}

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