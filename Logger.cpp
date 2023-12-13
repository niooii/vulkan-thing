#include "Logger.h"

using std::cout;
using std::cerr;

void Logger::debug(const char* str) {
    if(!debugActive) {
        return;
    }

    cout << formatMessage("DEBUG", str);
}

void Logger::warn(const char* str) {
    if(!warnActive) {
        return;
    }

    cerr << formatMessage("WARN", str);
}

void Logger::err(const char* str) {
    if(!errActive) {
        return;
    }

    cerr << formatMessage("ERROR", str);
}

void Logger::debug(string& str) {
    if(!debugActive) {
        return;
    }

    cout << formatMessage("DEBUG", str.c_str());
}

void Logger::warn(string& str) {
    if(!warnActive) {
        return;
    }

    cerr << formatMessage("WARN", str.c_str());
}

void Logger::err(string& str) {
    if(!errActive) {
        return;
    }

    cerr << formatMessage("ERROR", str.c_str());
}

void Logger::debug(string str) {
    if(!debugActive) {
        return;
    }

    cout << formatMessage("DEBUG", str.c_str());
}

void Logger::warn(string str) {
    if(!warnActive) {
        return;
    }

    cerr << formatMessage("WARN", str.c_str());
}

void Logger::err(string str) {
    if(!errActive) {
        return;
    }

    cerr << formatMessage("ERROR", str.c_str());
}

string Logger::timestamp() {
    using std::time_t, std::tm, std::to_string, std::format;

    time_t now = time(nullptr);

    tm *date = std::localtime(&now);

    string timestamp = format("{}:{}:{}", date->tm_hour, date->tm_min, date->tm_sec);

    return timestamp;
}

void Logger::activateDebug() {
    debugActive = true;
}

void Logger::activateWarn() {
    warnActive = true;
}

void Logger::activateErr() {
    errActive = true;
}

string Logger::formatMessage(const char *type, const char *message) {
    return std::format("[{}: {}] {}\n", type, timestamp(), message);
}

// activation functions (ik this isnt what that means)
