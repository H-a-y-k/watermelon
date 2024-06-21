#include "../include/patternmatching.h"
#include <iostream>

/*
 * Class Pattern
 */

Pattern Pattern::expect(const Token &tok)
{
    using vector_it = std::vector<Token>::iterator;
    expect_stack.push([tok](vector_it it, const vector_it &end) {
        if (*it == tok)
        {
            it++;
        }
        return it;
    });
    return *this;
}

Pattern Pattern::expect(Pattern pat)
{
    using vector_it = std::vector<Token>::iterator;
    expect_stack.push([pat](vector_it it, const vector_it& end) mutable { return pat.match(it, end); });
    return *this;
}

Pattern Pattern::expect(const ExpectPred &expect)
{
    expect_stack.push(expect);
    return *this;
}

Pattern Pattern::or_(Pattern other_ptrn)
{
    using vector_it = std::vector<Token>::iterator;

    Pattern this_ptrn = *this;
    Pattern or_ptrn = this_ptrn | other_ptrn;
    or_ptrn.match_mode = Pattern::MatchUntilSuccess;
    return or_ptrn;
}

Pattern Pattern::operator|(const Token &tok)
{
    auto ptrn = *this;

    using vector_it = std::vector<Token>::iterator;
    ptrn.expect_stack.push([tok](vector_it it, const vector_it &end) {
        if (it == end)
            return it;
        std::cout << "it: " << it->value.value() << "\n";
        // std::cout << "tok: " << tok.value.value() << "\n";
        if (*it == tok)
        {
            it++;
        }
        return it;
    });

    return ptrn;
}

Pattern Pattern::operator|(Pattern pat)
{
    auto ptrn = *this;

    using vector_it = std::vector<Token>::iterator;
    ptrn.expect_stack.push([pat](vector_it it, const vector_it& end) mutable { return pat.match(it, end); });
    return ptrn;
}

Pattern Pattern::operator|(const ExpectPred &expect)
{
    auto ptrn = *this;
    ptrn.expect_stack.push(expect);
    return ptrn;
}

Pattern Pattern::operator||(Pattern other)
{
    using vector_it = std::vector<Token>::iterator;

    Pattern or_ptrn;
    or_ptrn.expect([this, other](vector_it it, const vector_it &end) mutable {
        auto current_it = match(it, end);
        if (current_it == it)
            current_it = other.match(it, end);
        return current_it;
    });
    return or_ptrn;
}

Pattern Pattern::cycle()
{
    Pattern pat = *this;
    pat.cycle_  = true;
    return pat;
}

std::vector<Token>::iterator Pattern::single_match(std::vector<Token>::iterator it, const std::vector<Token>::iterator &end)
{
    if (match_functor.has_value())
        return match_functor.value()(it, end);

    using vector_it = std::vector<Token>::iterator;

    vector_it current_it = it;
    auto new_it = current_it;

    auto expect_stack_cpy = expect_stack;


    while (!expect_stack_cpy.empty())
    {
        auto pred = expect_stack_cpy.front();
        expect_stack_cpy.pop();

        new_it = pred(current_it, end);

        switch(match_mode)
        {
        case MatchUntilFail:
            if (new_it == current_it)
                return current_it;
            break;
        case MatchUntilSuccess:
            if (new_it != current_it)
                return new_it;
            break;
        case StopAtFail:
            if (new_it == current_it)
            {
                // std::cout << new_it->get_value() << "\n";
                return it;
            }
            break;
        default:
            break;
        }

        if (new_it == end && !expect_stack_cpy.empty())
        {
            return it;
        }
        current_it = new_it;
    }
    return current_it;
}

std::vector<Token>::iterator Pattern::cyclic_match(std::vector<Token>::iterator it, const std::vector<Token>::iterator &end)
{
    auto current_it = it;
    auto new_it = single_match(current_it, end);
    while (new_it != current_it)
    {
        current_it = new_it;
        new_it = single_match(current_it, end);
    }
    return current_it;
}

std::vector<Token>::iterator Pattern::match(std::vector<Token>::iterator it, const std::vector<Token>::iterator &end)
{
    if (cycle_)
        return cyclic_match(it, end);
    return single_match(it, end);
}
