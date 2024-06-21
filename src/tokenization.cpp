#include "../include/tokenization.h"

Token::Token(Type type)
    : type(type)
    , value(std::nullopt)
{
}

Token::Token(const char *value)
    : type(Token::Any)
    , value(value)
{
}

Token::Token(const std::string &value)
    : type(Token::Any)
    , value(value)
{
}

Token::Token(Type type, const std::string &value)
    : type(type)
    , value(value)
{
}

Token extract_numeric(std::string::iterator &str_it, std::string::iterator end)
{
    auto str = std::string(1, *str_it);
    auto init_str_it = str_it;

    if (str == ".")
    {
        str_it++;
        if (str_it != end)
        {
            str.push_back(*str_it);
        }
    }

    if (!is_number(str))
    {
        str_it = init_str_it;
        return Token::invalid();
    }

    while (is_number(str) && str_it != end)
    {
        str_it++;
        str.push_back(*str_it);
    }
    str.pop_back();
    return Token(Token::NumericLiteral, str);
}

Token extract_string(std::string::iterator &str_it, std::string::iterator end)
{
    auto str = std::string(1, *str_it);
    auto init_str_it = str_it;

    if (str != "'")
        return Token::invalid();
    str_it++;

    while (*str_it != '\'' && *str_it != '\n' && str_it != end)
    {
        str.push_back(*str_it);
        str_it++;
    }
    if (*str_it == '\'')
        str.push_back(*str_it);
    else
    {
        str_it = init_str_it;
        return Token::invalid();
    }
    return Token(Token::StringLiteral, str);
}

Token extract_name(std::string::iterator &str_it, std::string::iterator end)
{
    auto str = std::string(1, *str_it);

    if (!is_name(str))
        return Token::invalid();

    while ((str_it != end) && is_name(str))
    {
        str_it++;
        str.push_back(*str_it);
    }
    // str_it--;
    str.pop_back();
    return Token(Token::Name, str);
}

Token extract_symbol(std::string::iterator &str_it, std::string::iterator end)
{
    auto str = std::string(1, *str_it);

    if (is_symbol(std::string(str)))
    {
        str_it++;
        return Token(Token::Symbol, str);
    }
    return Token::invalid();
}

Token extract_operator(std::string::iterator &str_it, std::string::iterator end)
{
    auto str = std::string(1, *str_it);

    if (!(is_binaryop(str) || is_unaryop(str)))
        return Token::invalid();

    while ((str_it != end) && (is_binaryop(str) || is_unaryop(str)))
    {
        str_it++;
        str.push_back(*str_it);
    }
    // str_it--;
    str.pop_back();

    return Token(Token::Operator, str);
}

Token extract_token(std::string::iterator &str_it, const std::string::iterator &end)
{
    auto tok = Token::invalid();
    tok = extract_numeric(str_it, end);
    if (tok.isvalid()) return tok;
    tok = extract_string(str_it, end);
    if (tok.isvalid()) return tok;
    tok = extract_name(str_it, end);
    if (tok.isvalid()) return tok;
    tok = extract_symbol(str_it, end);
    if (tok.isvalid()) return tok;
    tok = extract_operator(str_it, end);
    return tok;
}

std::vector<Token> tokenize(std::string code)
{
    std::vector<Token> tokvec;

    auto it = code.begin();
    while (it != code.end())
    {
        while (std::isspace(*it))
            it++;

        auto tok = extract_token(it, code.end());
        if (tok.isvalid())
        {
            tokvec.push_back(tok);
        }
        else
            break;
    }

    return tokvec;
}
