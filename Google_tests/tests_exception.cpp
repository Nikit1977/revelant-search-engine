#include "ConfigErrorExceptions.h"
#include <gtest/gtest.h>

TEST(ConfigExceptionTest, constructor) {
    //Arrange
    ConfigException excClass("test exception");
    //Act

    //Assert
    ASSERT_STREQ("test exception", excClass.what());
}

TEST(DerivedExClassesTest, constructor) {
    // Arrange
    //тестовый производный класс от ConfigException
    class DerivedExClass : public ConfigException {
    public: DerivedExClass() : ConfigException("test exception") {}
    };

    //Act
    DerivedExClass testClass;

    //Assert
    ASSERT_STREQ("test exception", testClass.what());
}