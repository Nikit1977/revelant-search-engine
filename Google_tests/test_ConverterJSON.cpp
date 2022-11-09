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
//построение тестового файла и назначение его как целевого config.json
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
//тест записи в поле ConfigInfo корректных данных из тестового файла
TEST_F(ConvertorMethods, CreateInfo_test_init_configInfo) {
    //Act
    convertor.createConfigInfo(); //создание из тестового файла
    //Assert
    ASSERT_TRUE(example == convertor.getConfigData());
}
//тест: config.json имеет корректное значение config
TEST_F(ConvertorMethods, CheckConfigValid_json_has_config_data) {
    //Act
    convertor.createConfigInfo();
    //Assert
    EXPECT_NO_THROW({convertor.checkConfigValid();});
}

//тест: поле config отсутствует в файле
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

//тест: ключ config не имеет значений
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
//получение значений (values) по ключу, если значения являются списком (массивом)
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

//получение значений (values) по ключу, если значение не является списком (массивом)
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
//тест: извлечение запросов из json файла из ключа - requests
TEST(GetRequests_test, count_and_eql) {
    //Arrange
    //построение тестового файла
    std::vector<std::string> checkList {"mama", "papa"};
    boost::json::object test_request = { {"requests", boost::json::array{"mama", "papa"}}};
    const char* json_file = "temp_json";
    std::ofstream file(json_file, std::ios_base::trunc);
    if (file.is_open()) {
        file << test_request;
        file.close();
    }
    //установка тестового файла в качестве источника файла с запросами
    ConverterJSON convertor;
    convertor.setPathToRequestFile(json_file);

    //Act
    auto testList = convertor.GetRequests();

    //Assert
    ASSERT_TRUE(testList.size() == 2);

    for (int i = 0; i < testList.size(); i++) {
        ASSERT_STREQ(checkList[i].c_str(), testList[i].c_str());
    }
}

//тест на извлечение требуемого количества ответов

//построение тестового файла, имеющего ключ max_responses
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
    //здесь и в последующих тестах - добавление к ключу max_responses значения
    std::ofstream file(temp_file, std::ios_base::app);
    if (file.is_open()) {
        file << R"(-1}})"; // отрицательное
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
        file << R"(3}})"; //положительное
        file.close();
    }
    //Act
    convertor.createConfigInfo();
    auto test_responses = convertor.GetResponsesLimit();

    //Assert
    ASSERT_EQ(test_responses, 3);
}

TEST_F(GetResponsesLimit_count, not_num) {
    //Arrange
    std::ofstream file(temp_file, std::ios_base::app);
    if (file.is_open()) {
        file << R"("nan"}})"; //не число
        file.close();
    }
    //Act
    convertor.createConfigInfo();
    auto test_responses = convertor.GetResponsesLimit();

    //Assert
    ASSERT_EQ(test_responses, 5);
}
//построение тестового config файла с путями к текстовым файлам и создание тестовых текстовых файлов
struct GetTextDocuments_test : public ::testing::Test {
    ConverterJSON convertor;
    const char* temp_file = "temp_json";

    void SetUp() {
        std::ofstream file(temp_file, std::ios_base::trunc);
        if (file.is_open()) {
            //заготовка config.json
            file << R"({"config":{"name":"SkillboxSearchEngine"}, "files": [)";
            file.close();
        }
        std::ofstream text_file01("file001.txt", std::ios_base::trunc);
        if (text_file01.is_open()) {
            text_file01 << R"(Text from file001)";
            text_file01.close();
        }
        std::ofstream text_file02("file002.txt", std::ios_base::trunc);
        if (text_file02.is_open()) {
            text_file02 << R"(Text from file002)";
            text_file02.close();
        }
        convertor.setPathToConfigFile(temp_file);
    }
    std::vector<std::string> checkList {"Text from file001", "Text from file002"};
};

TEST_F(GetTextDocuments_test, all_files_is_txt) {
    //Arrange
    std::ofstream file(temp_file, std::ios_base::app);
    if (file.is_open()) {
        //дозаполнение config.json
        file << R"("file001.txt", "file002.txt"]})";
        file.close();
    }
    std::cout << "All files are .txt" << std::endl;

    convertor.createConfigInfo();

    //Act
    auto testList = convertor.GetTextDocuments();

    //Assert
    ASSERT_TRUE(testList.size() == 2);
    std::cout << "count files - OK" << std::endl;

    for (int i = 0; i < testList.size(); i++) {
        ASSERT_STREQ(checkList[i].c_str(), testList[i].c_str());
    }
    std::cout << "text equal - OK" << std::endl;
}
//тест на указание в config.json лишних/случайных файлов, не являющимися .txt, которые должны  быть  проигнорированы
TEST_F(GetTextDocuments_test, not_all_files_is_txt) {
    //Arrange
    std::ofstream file(temp_file, std::ios_base::app);
    if (file.is_open()) {
        file << R"("file001.txt", "file003", "file002.txt", "file004.doc"]})";
        file.close();
    }
    std::cout << "Included files are not .txt" << std::endl;

    convertor.createConfigInfo();
    checkList.emplace_back("Text from other files");

    //Act
    auto testList = convertor.GetTextDocuments();

    //Assert
    ASSERT_TRUE(testList.size() == 2);
    std::cout << "count files - OK" << std::endl;

    for (int i = 0; i < testList.size(); i++) {
        ASSERT_STREQ(checkList[i].c_str(), testList[i].c_str());
    }
    std::cout << "text equal - OK" << std::endl;
}