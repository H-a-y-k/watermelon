#ifndef PATTERNMATCHING_H
#define PATTERNMATCHING_H
#include <functional>
#include <queue>
#include <optional>
#include "tokenization.h"

using ExpectPred = std::function<std::vector<Token>::iterator(std::vector<Token>::iterator it,
                                                              const std::vector<Token>::iterator &end)>;

struct Pattern
{
    enum MatchMode
    {
        MatchUntilFail,
        MatchUntilSuccess,
        MatchAll,
        StopAtFail,
    };


    std::optional<ExpectPred> match_functor;
    std::queue<ExpectPred> expect_stack;

    Pattern expect(const Token &tok);
    Pattern expect(Pattern tok);
    Pattern expect(const ExpectPred &expect);

    Pattern operator|(const Token &tok);
    Pattern operator|(Pattern tok);
    Pattern operator|(const ExpectPred &expect);

    Pattern or_(Pattern other);
    Pattern operator||(Pattern tok);

    Pattern cycle();

    std::vector<Token>::iterator single_match(std::vector<Token>::iterator it, const std::vector<Token>::iterator &end);
    std::vector<Token>::iterator cyclic_match(std::vector<Token>::iterator it, const std::vector<Token>::iterator &end);
    std::vector<Token>::iterator match(std::vector<Token>::iterator it, const std::vector<Token>::iterator &end);
private:
    MatchMode match_mode = StopAtFail;
    bool cycle_=false;
};

#endif // PATTERNMATCHING_H
