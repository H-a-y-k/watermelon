#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include <string>
#include <vector>
#include <optional>
#include "language.h"

struct Token
{
    enum Type
    {
        Invalid,
        StringLiteral,
        NumericLiteral,
        Operator,
        Symbol,
        Name,
        Any
    };

    Type type;
    std::optional<std::string> value;

    Token(Type type);
    Token(const char *value);
    Token(const std::string &value);
    Token(Type type, const std::string &value);

    std::string get_value() const
    {
        return value.value();
    }

    bool isvalid() const
    {
        return type != Invalid;
    }

    static Token invalid()
    {
        return Token(Invalid);
    }

    bool operator==(const Token &other)
    {
        return (type == other.type || type == Any || other.type == Any) &&
               (value == other.value || !value.has_value() || !other.value.has_value());
    }

    bool operator!=(const Token &other)
    {
        return (type != other.type && type != Any && other.type != Any) ||
               (value != other.value && value.has_value() && other.value.has_value());
    }
};

Token extract_numeric(std::string::iterator &str_it, std::string::iterator end);
Token extract_string(std::string::iterator &str_it, std::string::iterator end);
Token extract_name(std::string::iterator &str_it, std::string::iterator end);
Token extract_symbol(std::string::iterator &str_it, std::string::iterator end);
Token extract_operator(std::string::iterator &str_it, std::string::iterator end);
Token extract_token(std::string::iterator &str_it, const std::string::iterator &end);

std::vector<Token> tokenize(std::string code);
#endif // TOKENIZATION_H
