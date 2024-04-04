#pragma once
#include <memory>
#include <filesystem>
#include <ostream>
#include <istream>
#include <fstream>
#include <vendor/fmt/fmt.h>

#define BIT(x) 1<<x

class QLayout;

class QWidget;
class QFrame;
class QLabel;
class QLabel;
class QPushButton;
class QPushButton;
class QValidator;
class QLineEdit;
class QCheckBox;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;


template<typename T,typename... Args>
Scope<T> CreateScope(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}


template<typename T,typename...Args>
Ref<T> CreateRef(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

namespace FileSystem = std::filesystem;


static std::string TrimLeft(const std::string& str, char delim = ' ') {
    size_t start = str.find_first_not_of(delim);
    if (start == std::string::npos)
        return "";
    return str.substr(start);
}

static void TrimLeftInplace(std::string& str, char delim = ' ') {
    size_t start = str.find_first_not_of(delim);
    if (start != std::string::npos)
        str.erase(0, start);
}

static std::string TrimRight(const std::string& str, char delim = ' ') {
    size_t end = str.find_last_not_of(delim);
    if (end == std::string::npos)
        return "";
    return str.substr(0, end + 1);
}

static void TrimRightInplace(std::string& str, char delim = ' ') {
    size_t end = str.find_last_not_of(delim);
    if (end != std::string::npos)
        str.erase(end + 1);
}

static std::string Trim(const std::string& str, char delim = ' ') {
    return TrimRight(TrimLeft(str, delim), delim);
}

static void TrimInplace(std::string& str, char delim = ' ') {
    TrimLeftInplace(str, delim);
    TrimRightInplace(str, delim);
}


namespace LinkedOut {


    static bool IsValidUsername(const std::string& username) {
        return username != "";
    }


    static bool IsValidPassword(const std::string& password) {
        if (password.find(' ') != std::string::npos)
            return false;

        if (password == "")
            return false;

        if (!std::all_of(password.begin(), password.end(), [](char c) {return isalpha(c) || isdigit(c); })) {
            return false;
        }
        return true;
    }

    struct UserData {
        std::string Username;
        std::string Password;

        bool IsValid() const{
            return IsValidUsername(Username) && IsValidPassword(Password);
        }

        operator bool()const {
            return IsValid();
        }


    };


    enum class SignupErrorCodes {
        None,
        UserExists
    };

    typedef int LoginErrorCodes;

    enum  LoginErrorCodes_ {
        LoginErrorCodes_None = 0,
        LoginErrorCodes_IncorrectUsername = BIT(0),
        LoginErrorCodes_IncorrectPassword = BIT(1),
        LoginErrorCodes_DBError = BIT(2)
    };

}