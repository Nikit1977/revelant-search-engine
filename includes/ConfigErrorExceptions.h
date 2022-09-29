#ifndef SEARCH_ENGINE_CONFIGERROREXCEPTIONS_H
#define SEARCH_ENGINE_CONFIGERROREXCEPTIONS_H

#include <exception>

class ConfigException : public std::exception {

public:
    explicit ConfigException(const char *msg) : std::exception(), ex_msg(msg) {}
    virtual ~ConfigException() noexcept = default;
    const char *what() const noexcept override { return ex_msg; }

private:
    const char *ex_msg;
};


class ConfigMissEx : public ConfigException {
public: ConfigMissEx() : ConfigException("Config file is missing") {}
};

class ConfigEmptyEx : public ConfigException {
public: ConfigEmptyEx() : ConfigException("Config file is empty") {}
};

class ConfigFormatEx : public ConfigException {
public: ConfigFormatEx() : ConfigException("Config file has syntax error") {}
};

class ConfigVersionEx : public ConfigException {
public: ConfigVersionEx() : ConfigException("Config.json has incorrect file version") {}
};


#endif //SEARCH_ENGINE_CONFIGERROREXCEPTIONS_H
