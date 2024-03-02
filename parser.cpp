//
// Created by willi on 14/02/2024.
//

#include "parser.h"

parser::parser(it &first, it last):first(first), last(last){}

ch_op *parser::parse_char(){
    auto ch = lexer.check(first, last);
    if(lexer.getType() == lex::END)
        return nullptr;
    return new ch_op(ch);
}

word_op *parser::parse_word(){
    auto ch_node = parse_char();

    auto result = new word_op;
    if(!ch_node || lexer.getType() == lexer.ANY){
        auto any_node = parse_any();
        if (any_node){
            first++;
            auto mult_node = parse_mult();
            auto count_node = parse_count();
            if(mult_node){
                mult_node ->add(any_node);
                result ->add(mult_node);
            }else if(count_node){
                count_node->add(any_node);
                result->add(count_node);
            }else{
                result->add(any_node);
            }
            result->add(parse_word());
        }else
        return nullptr;
    }else{
        if (lexer.getType() == lexer.OR || lexer.getType() == lexer.LCURL || lexer.getType() == lexer.LPAREN || lexer.getType() == lexer.MULT || lexer.getType() == lexer.RPAREN ||lexer.getType() == lexer.RCURL){
            if (lexer.getType() == lexer.MULT){
                first++;
            }
            return nullptr;
        }
            first++;

        auto count_node = parse_count();
        auto mult_node = parse_mult();
        if(mult_node){
            mult_node ->add(ch_node);
            result ->add(mult_node);
        }else if(count_node){
            count_node->add(ch_node);
            result->add(count_node);
        }else{
            result->add(ch_node);
        }
        result->add(parse_word());
    }

    return result;
}

group_op *parser::parse_group(){
    auto group_node = new group_op;
    lexer.check(first, last);
    if(lexer.type == lexer.LPAREN) {
        first++;
        auto expr_node = parse_expr();
        if (expr_node) {
            group_node->add(expr_node);
            group_node->add(parse_expr());
            return group_node;
        }
    }else if(lexer.type == lexer.RPAREN) {
        first++;
        return nullptr;
    }
    return nullptr;
}

expr_op *parser::parse_expr() {
    auto expr_node = new expr_op;

    auto group_op = parse_group();
    if(group_op){
        expr_node->add(group_op);
        first++;
        if (lexer.getType() == lexer.END){
            return expr_node;
        }
    }

    auto word_node = parse_word(); //KLAR
    if(word_node){
        auto or_node = parse_or(); //KLAR
        if (or_node){
            or_node->add(word_node);
            first++;

            word_node = parse_word();
            if(word_node){
                or_node->add(word_node);
            }
            expr_node->add(or_node);
        }else{
            expr_node->add(word_node);
            }

        expr_node->add(parse_expr());
        return expr_node;
    }

    auto count_node = parse_count();
    if (count_node){
        expr_node->add(parse_expr());
        return expr_node;
    }
    return nullptr;
}

match_op *parser:: parse_match(){
    auto expr_node = parse_expr();
    if(!expr_node)
        return nullptr;
    auto result = new match_op;
    result->add(expr_node);
    return result;
}

or_op* parser::parse_or() {
    lexer.check(first, last);
    if (lexer.getType() == lexer.OR)
        return new or_op();
    return nullptr;
}

any_op* parser::parse_any() {
    lexer.check(first, last);
    if (lexer.getType() == lexer.ANY){
        return new any_op();
    }
    return nullptr;
}

count_op* parser::parse_count() {
    lexer.check(first, last);
    if(lexer.getType() == lexer.LCURL){
        first++;
        lexer.check(first, last);
        if(lexer.getType() == lexer.DIGIT){
            int number = *first - '0';
            auto count_node = new count_op(number);
            first++;
            lexer.check(first, last);
            if(lexer.getType() == lexer.RCURL) {
                first ++;
                return count_node;
            }
        }
    }
    return nullptr;
}

mult_op* parser::parse_mult(){
    lexer.check(first, last);
    if(lexer.getType() == lexer.MULT)
        return new mult_op();
    return nullptr;
}

/*slash_op* parser::parse_slash() { //GÖRA SIST
    lexer.check(first, last);
    if(lexer.getType() == lexer.SLASH){
        auto pos = first++;
        lexer.check(pos, last);
        if(lexer.getType() == lexer.OUT){
            pos++;
            lexer.check(pos, last);
            if(lexer.getType() == lexer.LCURL){
                auto text_node = parse_word();
                if(!text_node)
                    return nullptr;
                lexer.check(first, last);
                if(lexer.getType() != lexer.RCURL) {
                    return nullptr;
                }
                auto slash_node = new slash_op;
                slash_node->add(text_node);
                return slash_node;
            }
        }
        else if(lexer.getType() == lexer.CAPITAL){
            auto text_node = parse_word();
            if(!text_node)
                return nullptr;
            lexer.check(first, last);
            if(lexer.getType() != lexer.CAPITAL) {
                return nullptr;
            }
            auto slash_node = new slash_op;
            slash_node->add(text_node);
            return slash_node;
        }
    }

    return nullptr;
}
*/

