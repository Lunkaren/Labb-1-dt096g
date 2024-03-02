//
// Created by willi on 14/02/2024.
//
#include <iostream>
#include <vector>
#include <iomanip>
#include "lexer.h"

#ifndef LABB1_DT096G_PARSER_H
#define LABB1_DT096G_PARSER_H

using it = std::string::iterator;

struct op{ //operator base
    virtual bool eval(it first, it last, int& index, std::string& toPrint) = 0;
    virtual std::string ident()=0;

    void print_Tree(int indentering = 0){           // printar trädstrukturen
        std::cout << std::setw(indentering) << "" << ident() << "\n";
        for(auto child: children){
            child->print_Tree(indentering +4);
        }
    }

    void add(op* child){
        if (child)
            children.push_back(child);
    }
    std::vector<op*> children;
    void printResult(std::string result){
        std::cout << "Matchande text som hittades: " << result << "\n";
    }
};

struct ch_op:op{
    char c;
    ch_op(char c):c(c){}

    std::string ident() override{
        return "ch_op: "+std::string(1, c);
    }

    bool eval(it first, it last, int& index, std::string& toPrint) override{
        if (*first == c){
            toPrint += c;
            return true;
        }
        toPrint = "";
        return false;
    }
};

struct word_op: op{
    std::string ident() override{
        return "word_op: ";
    }

    bool eval(it first, it last, int& index, std::string& toPrint) override{
        auto result = children[0]->eval(first, last, index, toPrint);
        index++;
        if(result && children.size() > 1){
            return result && children[1]->eval(first + 1, last, index, toPrint);
        }
        return result;
    }
};

struct or_op:op{
    std::string ident() override{
        return "or_op: ";
    }

    bool eval(it first, it last, int& index, std::string& toPrint) override{
        auto result = children[0]->eval(first, last, index, toPrint);
        if (!result && children.size() > 1){
            result = children[1]->eval(first, last, index, toPrint);
        }
        return result;
    }
};

struct mult_op:op{
    std::string ident() override{
        return "mult_op: ";
    }

    bool eval(it first, it last, int& index, std::string& toPrint) override{
        auto result = children[0]->eval(first, last, index, toPrint);
        auto toReturn = result;
        std::string fixString;
        while (result){ //Ökar pos för att kolla nästa position sen tilldelas INDEX den senast matchade positionen
            first++;
            index++;
            toReturn = result;
            fixString = toPrint;
            result = children[0]->eval(first, last, index, toPrint);
            if (!result){
                index--;
            }
        }
        toPrint = fixString;
        return toReturn;
    }
};

struct any_op:op{
    std::string c;
    std::string ident() override{
        return "any_op: *";
    }

    bool eval(it first, it last, int& index, std::string& toPrint) override{
        std::string abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyx, '";
        std::size_t found = abc.find(*first);
        if (found!=std::string::npos){
            c = *first;
            toPrint += c;
            return true;
        }
        toPrint = "";
        return false;
    }
};

struct count_op:op{
    std::string ident() override{
        return "count_op: ";
    }

    int laps = 0;
    count_op(int laps):laps(laps){}

    bool eval(it first, it last, int& index, std::string& toPrint) override{
        auto result = children[0]->eval(first, last, index, toPrint);
        auto toReturn = result;

        int currentLap = 1;
        while (result && laps > currentLap){ //Ökar pos för att kolla nästa position sen tilldelas INDEX den senast matchade positionen
            currentLap++;
            first++;
            index++;
            toReturn = result;
            result = children[0]->eval(first+1, last, index, toPrint);
        }
        return toReturn;
    }
};


struct group_op:op{
    std::string ident() override{
        return "group_op: ";
    }

    bool eval(it first, it last, int& index, std::string& toPrint) override{
        auto result = children[0]->eval(first, last, index, toPrint);
        return result;
    }
};

struct expr_op:op{ //Texten == first och last då hela inputen redan lästs igenom och evaluerats och ligger som children.
    std::string ident() override{
        return "expr_op: ";
    }

    bool eval(it first, it last, int& index, std::string& toPrint) override{
        if(!children.empty()){
            index = 0;
            auto result = children[0] ->eval(first, last, index, toPrint); //kollar om det som ligger på plats 0 = det som ligger på plats index i texten
            int pos = 0;
            if(result && children.size() > 1){
                result = children[1] ->eval(first + index, last, pos, toPrint); //Om det finns en plats 2 och våran bokstav matchade så går vi vidare och kollar på andra grenen.
            }
            return result;
            }
        index = 0;
        return true;
    }
};

struct match_op:op{
    std::string ident() override{
        return "match_op: ";
    }
    bool eval(it first, it last, int& index, std::string& toPrint) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last, index, toPrint);
        if(!result){
            first++;
            return eval(first, last, index, toPrint);
        }
        printResult(toPrint);
        return true;
    }
};

class parser {
public:
    parser(it& first, it last);
    
    ch_op* parse_char();
    word_op* parse_word();
    group_op* parse_group();
    expr_op* parse_expr();
    match_op* parse_match();
    or_op* parse_or();
    any_op* parse_any();
    count_op* parse_count();
    mult_op* parse_mult();
    it first;
    it last;
private:
    lex lexer;
};


#endif //LABB1_DT096G_PARSER_H
