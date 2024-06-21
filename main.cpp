#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include "include/language.h"
#include "include/tokenization.h"
#include "include/patternmatching.h"
// #include "include/parser.h"

int main()
{
    std::string lol = "(4+8*2/3+7)*2";
    auto it = lol.begin();
    auto tok = tokenize(lol);

    Pattern pattern;

    // Pattern paramptrn = (pattern | Token::Name | ",").cycle().or_(pattern | Token::Name);
    // Pattern numptrn = pattern | Token::NumericLiteral | "+" | Token::NumericLiteral | ";";
    // Pattern ifptrn =
    //     pattern | "if" | "(" | paramptrn | ")" | "{" | numptrn | "}";

    Pattern exprterm;
    exprterm.expect((pattern | Token::Name).or_(pattern | Token::NumericLiteral).or_(pattern | Token::StringLiteral));

    Pattern binop;
    binop.expect((pattern | "+").or_(pattern | "-").or_(pattern | "/").or_(pattern | "*"));

    Pattern primitive_noparen_expr;
    primitive_noparen_expr.expect(exprterm | (binop | exprterm).cycle());
    primitive_noparen_expr = primitive_noparen_expr.or_(exprterm);
    Pattern primitive_paren_expr;
    primitive_paren_expr.expect(pattern | "(" | primitive_noparen_expr | ")");

    Pattern primitive_expr;
    primitive_expr.expect(primitive_noparen_expr || primitive_paren_expr);

    Pattern expr;
    expr.expect(primitive_expr | (binop | primitive_expr).cycle());
    expr = expr.or_(pattern | "(" | expr | ")");
    expr = expr.or_(primitive_expr);

    auto final_it = expr.match(tok.begin(), tok.end());
    // std::cout << final_it->get_value();
    std::cout << std::boolalpha;
    std::cout << (final_it == tok.begin()) << "\n";
    std::cout << (final_it == tok.end());

    return 0;
}
