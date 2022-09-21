#ifndef SEARCH_ENGINE_CONFIGERROREXCEPTIONS_H
#define SEARCH_ENGINE_CONFIGERROREXCEPTIONS_H

#include <exception>

class ConfigException : public std::exception {

public:
    explicit ConfigException(const char *msg) : std::exception(), ex_msg(msg) {}
    virtual const char *what() const noexcept override { return ex_msg; }

private:
    const char *ex_msg;
};


class ConfigFileMissEx : public ConfigException {
public: ConfigFileMissEx() : ConfigException("Config file is missing") {}
};

class ConfigFileEmptyEx : public ConfigException {
public: ConfigFileEmptyEx() : ConfigException("Config file is empty") {}
};

class ConfigInvalidEx : public ConfigException {
public: ConfigInvalidEx() : ConfigException("Config file has syntax error") {}
};


#endif //SEARCH_ENGINE_CONFIGERROREXCEPTIONS_H
