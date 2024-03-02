#include <iostream>
#include <vector>
#include "lexer.h"
#include "parser.h"

/*
Info about the Operators:

    + eller, passar ett eller flera upprepningar av en operand Syntax: OP1+OP2

    * flera, passar ett eller flera upprepningar av en operand. Syntax: OP*

    () infångsgrupp, deluttryck. Uttrycket parsas separat. Syntax: (EXPR)

    . tecken. Matchar valfritt tecken. Syntax: .

    {} räknare. Matchar precis N stycken operander. Syntax: OP{3}

    \I ignorerar versalisering. Syntax: EXPR\I

    \O{} vilken infångsgrupp som ska ges som output. Syntax: EXPR\O{2}.
    Default \O{0} hela matchningen


The program follows the grammar below:




<PROGRAM>    --> ( <REGEXP> | <CAPITAL> ) [ <GROUP> ]
<REG-EXP>   --> <EXPR> [ <REGEXP> ]

<EXPR>      --> <WORD> [ <OR> | <PARENTHESIS> | <EXPR> ]

<CAPITAL>   --> <REG-EXP> "\I"
<PARENTHESIS>-> "( <REG-EXP> )"
<GROUP>     --> "\O{ <CHAR> }"

<COUNTER>   --> <CHAR> "{ <CHAR> }"
<ANY>       --> "."
<MULTIPLE> --> (<CHAR> | <ANY>) "*"
<OR>        --> <WORD> "+" ( <WORD> | <EXPR> )

<WORD>      --> (<MULTIPLE> | <ANY> | <CHAR> | <COUNTER>) [ <WORD> ]
<CHAR>      --> A-Z, a-z, 0-9 and whitespace

This Results in the following codes for the lexer:
    * --> UNLIMITED
    . --> ANY
    + --> OR
    \ --> SLASH
       O --> GROUP
       I --> CAPITAL
    { --> BRACKET_START
    } --> BRACKET_END
    ( --> PARENTHESIS_START
    ) --> PARENTHESIS_END
    Combination of A-Z, a-z, 0-9 --> WORD
    EOF --> END
*/

using it = std::string::iterator;

int main(int argc, char* argv[]) {
    int index = 0;
    std::string result;
    std::string program = "promise to (love+hate) you"; // argv[1];
    std::string input = "Waterloo I was defeated, you won the war Waterloo promise to love you for ever more Waterloo couldn't escape "
                        "if I wanted to Waterloo knowing my fate is to be with you Waterloo finally facing my Waterloo";

    auto first = program.begin();
    auto last = program.end();
    parser parser(first, last);


    auto tree = parser.parse_match();
    auto match = tree->eval(input.begin(), input.end(), index, result);

    std::cout << "Trädstrukturen: " << std::endl;
    tree->print_Tree();

    if (!match){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}