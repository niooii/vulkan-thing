#include <stdexcept>
#include <iostream>
#include <chrono>
#include <ctime>
#include <format>

using std::string;

class Logger {
public:
    void debug(const char* str);
    void warn(const char* str);
    void err(const char* str);

    void debug(string& str);
    void warn(string& str);
    void err(string& str);

    void debug(string str);
    void warn(string str);
    void err(string str);

    void activateDebug();
    void activateWarn();
    void activateErr();


private:
    bool debugActive{false};
    bool warnActive{false};
    bool errActive{false};

    static string timestamp();
    string formatMessage(const char* type, const char* str);

};