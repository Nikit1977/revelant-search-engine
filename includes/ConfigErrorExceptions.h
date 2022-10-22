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


class FileMissEx : public ConfigException {
public: FileMissEx() : ConfigException("File is missing") {}
};

class ConfigEmptyEx : public ConfigException {
public: ConfigEmptyEx() : ConfigException("Config file is empty") {}
};

class FormatEx : public ConfigException {
public: FormatEx() : ConfigException("File has syntax error") {}
};

class ConfigVersionEx : public ConfigException {
public: ConfigVersionEx() : ConfigException("Config.json has incorrect file version") {}
};


#endif //SEARCH_ENGINE_CONFIGERROREXCEPTIONS_H
