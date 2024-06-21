#ifndef PARSER_H
#define PARSER_H

#include "tokenization.h"
#include <stack>
#include <functional>

#include <map>
#include "patternmatching.h"

// using ExpectPred = std::function<bool(const Token&)>;

using PatternExtractor = std::function<std::vector<Token>(std::vector<Token>::iterator &it,
                                                          const std::vector<Token>::iterator &end,
                                                          const Pattern &pattern)>;

class Parser
{
    std::vector<Token> tokens;
    std::vector<Token>::iterator tok_it;

    std::map<const std::string, Pattern> patterns;

public:
    Parser(const std::vector<Token> &tokens);

    void register_pattern(std::string name, Pattern pattern)
    {
        patterns.insert({name, pattern});
    }

    // std::vector<Token> extract_pattern(const std::string &name)
    // {
    //     return extract_pattern(patterns[name]);
    // }

    // std::vector<Token> extract_pattern(const PatternExtractor &extract)
    // {
    //     return extract(tok_it, tokens.end(), &expect_stack);
    // }
};

#endif // PARSER_H
