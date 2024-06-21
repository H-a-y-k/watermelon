#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <set>
#include <string>
#include <cctype>
#include <algorithm>
#include <sstream>

inline std::set<std::string> keywords = {"int", "string", "bool", "double", "fn"};
inline std::set<std::string> binaryoperators = {"+", "-", "*", "/", "%", "=", "+=", "-=", "*=", "/=", "%=", "||", "&&", "^"};
inline std::set<std::string> unaryoperators = {"++", "--", "~", "!"};
inline std::set<std::string> symbols = {".", ",", ";", "(", ")", "{", "}", "\'"};

inline bool is_keyword(const std::string &str)
{
    return (keywords.find(str) != keywords.end());
}

inline bool is_binaryop(const std::string &str)
{
    return (binaryoperators.find(str) != binaryoperators.end());
}

inline bool is_unaryop(const std::string &str)
{
    return (unaryoperators.find(str) != unaryoperators.end());
}

inline bool is_symbol(const std::string &str)
{
    return (symbols.find(str) != symbols.end());
}

inline bool is_double(const std::string& str) {
    std::stringstream ss(str);
    double f;
    ss >> std::noskipws >> f;

    return ss.eof() && !ss.fail();
}

inline bool is_int(const std::string& str) {
    std::stringstream ss(str);
    int f;
    ss >> std::noskipws >> f;

    return ss.eof() && !ss.fail();
}

inline bool is_number(const std::string &str)
{
    return is_double(str);
}

inline bool is_name(const std::string &str)
{
    if (!std::isalpha(str[0]) && (str[0] != '_'))
        return false;

    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return (static_cast<bool>(std::isalnum(c)) || (c == '_'));
    });
}
#endif // LANGUAGE_H
