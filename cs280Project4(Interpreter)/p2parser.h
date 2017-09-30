//
//  p2parser.h
//  Parsercs280
//
//
//  Copyright © 2016 Shreyas Ramanujam. All rights reserved.
//

#include "p2lex.h"
#include <string>
#include <istream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

extern int globalErrorCount;

enum Type {
    STRING,
    INTEGER,
    UNIDENTIFIED
};

struct evalresult {
    Type  t;
    string  str;
};

/// error handler
extern void error(string msg, bool showline);
extern map<string,evalresult> idValues;
extern map<string,int> symbols;

/////////
//// this class can be used to represent the parse result
//// the various different items that the parser might recognize can be
//// subclasses of ParseTree

class ParseTree {
private:
    ParseTree *leftChild;
    ParseTree *rightChild;
    
    int	whichLine;
    
public:
    
    ParseTree(ParseTree *left = 0, ParseTree *right = 0) : leftChild(left),rightChild(right) {
        whichLine = linenum;
    }
    
    int onWhichLine() { return whichLine; }
    
    int traverseAndCount(int (ParseTree::*f)()) {
        int cnt = 0;
        if( leftChild ) cnt += leftChild->traverseAndCount(f);
        if( rightChild ) cnt += rightChild->traverseAndCount(f);
        return cnt + (this->*f)();
    }
    
    ParseTree * traverseAndFind(int (ParseTree::*f)())
    {
        if ((this->*f)())
            return this;
        else
        {
            if( leftChild )
                return(leftChild->traverseAndFind(f));
            
            if( rightChild )
                return(rightChild->traverseAndFind(f));
            
            return 0;
        }
    }
    
    int traverseAndFindSetPrint()
    {
        if (this->isSet())
        {
            evalresult value = Evaluate(this->leftChild);
            
            // populate the ID map table with the value
            this->setValueforID(value);
        }
        else if (this->isPrint())
        {
            evalresult value = Evaluate(this->leftChild);
            
            // Remove all double-quote characters
            value.str.erase(
                    remove( value.str.begin(), value.str.end(), '\"' ),
                    value.str.end()
                    );
            
            cout << value.str << endl;
            
        }
        else
        {
            if( leftChild )
                leftChild->traverseAndFindSetPrint();
            
            if( rightChild )
                rightChild->traverseAndFindSetPrint();
        }
        
        return 0;
    }

    
    int isOperator(int (ParseTree::*f)())
    {
        return ((this->*f)());
    }
    
    int countUseBeforeSet( map<string,int>& symbols ) {
        int cnt = 0;
        if( leftChild ) cnt += leftChild->countUseBeforeSet(symbols);
        if( rightChild ) cnt += rightChild->countUseBeforeSet(symbols);
        return cnt + this->checkUseBeforeSet(symbols);
    }
    
    int countUseBeforePrint( map<string,int>& symbols ) {
        int cnt = 0;
        if( leftChild ) cnt += leftChild->countUseBeforeSet(symbols);
        if( rightChild ) cnt += rightChild->countUseBeforeSet(symbols);
        return cnt + this->checkUseBeforeSet(symbols);
    }

    evalresult Evaluate(ParseTree * expr)
    {
        evalresult result;
        result.t = UNIDENTIFIED;
        result.str = " ";
        
        if (expr->isInt())
        {
            result.t = INTEGER;
            result.str = to_string(expr->getInteger());
            
            // Remove all double-quote characters
            result.str.erase(
                            remove( result.str.begin(), result.str.end(), '\"' ),
                            result.str.end()
                            );
            
            return(result);
        }
        else if (expr->isString() )
        {
            result.t = STRING;
            result.str = expr->getString();
            
            // Remove all double-quote characters
            result.str.erase(
                            remove( result.str.begin(), result.str.end(), '\"' ),
                            result.str.end()
                            );
            
            return(result);
        }
        else if (expr->isId())
        {
            return(expr->getValueforID());
        }
        else if(expr->isPlus())
        {
            evalresult op1 = Evaluate(expr->leftChild);
            
            if(op1.t == INTEGER)
            {
                evalresult op2 = Evaluate(expr->rightChild);
                
                if(op2.t == INTEGER)
                {
                    result.t = INTEGER;
                    result.str = to_string(stoi(op1.str) + stoi(op2.str));
                    return(result);
                }
                else if(op2.t == STRING)
                {
                    cout << "Invalid operands type" << endl;
                }
            }
            else if(op1.t == STRING)
            {
                evalresult op2 = Evaluate(expr->rightChild);
            
                if(op2.t == STRING)
                {
                    result.t = STRING;
                    
                    
                    string outputstring;
                    outputstring.append(op1.str);
                    outputstring.append(op2.str);
                    result.str = outputstring;
                    return(result);
                }
                else if(op2.t == INTEGER)
                {
                    cout << "Inalid operands type" << endl;
                }
            }
        }
        else if(expr->isStar())
        {
            
            evalresult op1 = Evaluate(expr->leftChild);
            
            if(op1.t == INTEGER)
            {
                evalresult op2 = Evaluate(expr->rightChild);
                
                if(op2.t == INTEGER)
                {
                    result.t = INTEGER;
                    result.str = to_string(stoi(op1.str) * stoi(op2.str));
                    return(result);
                }
                else if(op2.t == STRING)
                {
                    string outputstring;
                    
                    for (int i = 0 ; i < stoi(op1.str);i++)
                    {
                        outputstring += op2.str;
                    }
                    
                    result.t = STRING;
                    result.str = outputstring;
                    return(result);
                }
            }
            else if(op1.t == STRING)
            {
                
                evalresult op2 = Evaluate(expr->rightChild);
                
                if(op2.t == STRING)
                {
                    cout << "Inalid operands type" << endl;
                }
                else if(op2.t == INTEGER)
                {
                    string outputstring;
                    
                    for (int i = 0 ; i < stoi(op2.str);i++)
                    {
                        outputstring += (op1.str);
                    }
                    
                    result.t = STRING;
                    result.str = outputstring;
                    return(result);

                }
            }
        }
        
        
        return(result);
        
    }


    evalresult getValueforID() {
        return(idValues[this->getID()]);
    }
    
    virtual int checkUseBeforeSet( map<string,int>& symbols ) {
        return 0;
    }
    
    virtual evalresult checkUseBeforeSet( map<string,evalresult>& symbols ) {
        evalresult result;
        result.t = UNIDENTIFIED;
        result.str = nullptr;
        return(result);
    }
    
    virtual void setValueforID(evalresult value){};
    virtual int Eval(){return 0;};
    virtual int isPlus() { return 0; }
    virtual int isStar() { return 0; }
    virtual int isBrack() { return 0; }
    virtual int isInt() { return 0; }
    virtual int isId() { return 0; }
    virtual int isSet() { return 0; }
    virtual int isPrint() { return 0; }
    virtual int isString() { return 0; }
    virtual string getID() { return 0; }
    virtual string getString() { return 0; }
    virtual int getInteger() { return 0; }
    virtual int isEmptyString() { return 0; }
};

class Slist : public ParseTree {
public:
    Slist(ParseTree *left, ParseTree *right) : ParseTree(left,right) {}
};

class PrintStmt : public ParseTree {
    ParseTree *expr;
    
public:
    PrintStmt(ParseTree *expr) : ParseTree(expr) {}
    
    int isPrint() { return 1; }
};

class SetStmt : public ParseTree {
private:
    string	ident;
    ParseTree *expr;
    
public:
    SetStmt(string id, ParseTree *expr) : ParseTree(expr), ident(id) {}
    
    int isSet() { return 1; }
    
    int checkUseBeforeSet( map<string,int>& symbols ) {
        symbols[ident]++;
        return 0;
    }
    
    evalresult checkUseBeforeSet( map<string,evalresult>& symbols ) {
        return(symbols[ident]);
    }
    
    void setValueforID(evalresult result) {
        idValues[ident] = result;
    }
    
    string	getID() { return ident; }
    
    evalresult getValueforID( string id ) {
        return(idValues[id]);
    }

};

class PlusOp : public ParseTree {
public:
    PlusOp(ParseTree *left, ParseTree *right) : ParseTree(left,right) {}
    
    int isPlus() { return 1; }
    
    
};

class StarOp : public ParseTree {
public:
    StarOp(ParseTree *left, ParseTree *right) : ParseTree(left,right) {}
    int isStar() { return 1; }
};

class BracketOp : public ParseTree {
private:
    Token sTok;
    
public:
    BracketOp(const Token& sTok, ParseTree *left, ParseTree *right = 0) : ParseTree(left,right), sTok(sTok) {}
    int isBrack() { return 1; }
};

class StringConst : public ParseTree {
private:
    Token sTok;
    
public:
    StringConst(const Token& sTok) : ParseTree(), sTok(sTok) {}
    
    string	getString() { return sTok.getLexeme(); }
    
    int isString() { return 1; }

    int isEmptyString() {
        if( sTok.getLexeme().length() == 2 ) {
            error("Empty string not permitted on line " + to_string(onWhichLine()), false );
            return 1;
        }
        return 0;
    }
};

//// for example, an integer...
class Integer : public ParseTree {
private:
    Token	iTok;
    
public:
    Integer(const Token& iTok) : ParseTree(), iTok(iTok) {}
    
    int isInt() { return 1; }
    
    int	getInteger() { return stoi( iTok.getLexeme() ); }
};

class Identifier : public ParseTree {
private:
    Token	iTok;
    
public:
    Identifier(const Token& iTok) : ParseTree(), iTok(iTok) {}
    
    int isId() { return 1; }
    
    string	getID() { return iTok.getLexeme(); }
    
    int checkUseBeforeSet( map<string,int>& symbols ) {
        if( symbols.find( iTok.getLexeme() ) == symbols.end() ) {
            error("Symbol " + iTok.getLexeme() + " used without being set at line " + to_string(onWhichLine()), false);
            return 1;
        }
        return 0;
    }
};

ParseTree *Program(istream *in);
ParseTree *StmtList(istream *in);
ParseTree *Stmt(istream *in);
ParseTree *Expr(istream *in);
ParseTree *Term(istream *in);
ParseTree *Primary(istream *in);
ParseTree *String(istream *in);
