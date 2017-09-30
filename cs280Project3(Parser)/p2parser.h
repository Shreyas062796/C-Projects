//
//  p2parser.hpp
//  Parsercs280
//
//  Created by Lavanya Ramanujam on 11/11/16.
//  Copyright © 2016 Shreyas Ramanujam. All rights reserved.
//

#ifndef p2parser_h
#define p2parser_h

#include <stdio.h>
#include "p2lex.h"
#include <string>
#include <istream>
#include <fstream>
using namespace std;


/*enum ReturnStatus {
    CONTINUE,
    DONE,		// finished!
    ERR,		// an unrecoverable error
};*/


/////////
//// this class can be used to represent the parse result
//// the various different items that the parser might recognize can be
//// subclasses of ParseTree

class ParseTree {
private:
    ParseTree *Child1;
    ParseTree *Child2;
    ParseTree *Child3;
    
    int	whichLine;
    
public:
    ParseTree(ParseTree *x = 0, ParseTree *y = 0, ParseTree *z = 0) : Child1(x),Child2(y),Child3(z)
    {
        whichLine = linenum;
    }
    
    void setPTChildNode1(ParseTree *x) {Child1 = x;}
    void setPTChildNode2(ParseTree *x) {Child2 = x;}
    void setPTChildNode3(ParseTree *x) {Child3 = x;}
    
    ParseTree *getPTChildNode1(void) { return Child1;}
    ParseTree *getPTChildNode2(void) { return Child2;}
    ParseTree *getPTChildNode3(void) { return Child3;}
    
    int TraverseTreeGetCount(TokenType t);
    
    int onWhichLine() { return whichLine; }
};

//// integer...
class IntegerParseTree : public ParseTree {
private:
    Token	iTok;
    
public:
    IntegerParseTree(const Token& iTok) : ParseTree(), iTok(iTok) {}
    
    int	getInteger() { return stoi( iTok.getLexeme() ); }
};

class IDParseTree : public ParseTree {
private:
    Token	iTok;
    
public:
    IDParseTree(const Token& iTok) : ParseTree(), iTok(iTok) {}
    
    string	getID() { return iTok.getLexeme() ; }
};

class STRParseTree : public ParseTree {
private:
    Token	iTok;
    
public:
    STRParseTree(const Token& iTok) : ParseTree(), iTok(iTok) {}
    
    string	getID() { return iTok.getLexeme() ; }
};


class PlusOpParseTree : public ParseTree {
private:
    Token	iTok;
    
public:
    PlusOpParseTree(Token& iTok) : ParseTree(), iTok(iTok) {}
    
    string	getString() { return iTok.getLexeme(); }
};

class StarOpParseTree : public ParseTree {
private:
    Token	iTok;
    
public:
    StarOpParseTree(Token& iTok) : ParseTree(), iTok(iTok) {}
    
    string	getString() { return iTok.getLexeme(); }
};

class PrintParseTree : public ParseTree {
private:
    Token	iTok;
    
public:
    PrintParseTree(Token& iTok) : ParseTree(), iTok(iTok) {}
    
    string	getString() { return iTok.getLexeme(); }
};



//Function declarations
ParseTree *Program();
ParseTree *StmtList();
ParseTree *Stmt();
ParseTree *Expr();
ParseTree *Term();
ParseTree *Primary();
ParseTree *String();
void getnexttoken();
int expect(TokenType t);
int accept(TokenType t);
void error(string msg);



#endif /* p2parser_hpp */
