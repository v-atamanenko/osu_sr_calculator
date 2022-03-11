#ifndef OSU_SR_CALCULATOR_LOGGER_H
#define OSU_SR_CALCULATOR_LOGGER_H

#include "string"

class Logger {
public:
    bool IsVerbose = false;
    std::string File;

    Logger(bool isVerbose, const std::string& _file) {
        IsVerbose = isVerbose;
        File = _file;
    }

    void log(const std::string& message, const std::string& log_type="msg") const {
        // Available log_types:
        // "msg", "warn", "error"

        // will log warnings and errors despite verbosity
        if (log_type == "warn") {
            fprintf(stderr, "[WARN] [%s] %s\n", File.c_str(), message.c_str());
        }

        if (log_type == "error") {
            fprintf(stderr, "[ERROR] [%s] %s\n", File.c_str(), message.c_str());
        }

        if (log_type == "msg" && IsVerbose) {
            fprintf(stderr, "[MSG] [%s] %s\n", File.c_str(), message.c_str());
        }
    }
};

#endif //OSU_SR_CALCULATOR_LOGGER_H
