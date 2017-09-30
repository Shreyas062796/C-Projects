//
//  p2parser.cpp
//  Parsercs280
//
//  Created by Lavanya Ramanujam on 11/11/16.
//  Copyright © 2016 Shreyas Ramanujam. All rights reserved.
//

#include "p2parser.h"
//#include "p2lex.h"
#include <string>
#include <istream>
#include <fstream>
using namespace std;

Token GetAToken();
void PushbackToken(Token& t);

extern istream *in;
extern int linenum;
extern int globalErrorCount;

extern int countPlus;
extern int countStar;
extern int countSqr;

Token current_token;
Token saved;
bool isSaved = false;


// HELPER FUNCTIONS
/// Token Wrapper

Token GetAToken()
{
    if( isSaved )
    {
        isSaved = false;
        return saved;
    }
    
    return getToken(in);
}

void PushbackToken(Token& t)
{
    if( isSaved ) {
        cerr << "Can't push back more than one token!!!";
        exit(0);
    }
    
    saved = t;
    isSaved = true;
}


/// error handler
void error(string msg)
{
    //cout << linenum << ": " << msg << endl;
    ++globalErrorCount;
}

int accept(TokenType t)
{
    if (current_token.getTok() == t)
    {
        getnexttoken();
        return 1;
    }
    return 0;
}

int expect(TokenType t)
{
    if (accept(t))
        return 1;
    error("expect: unexpected symbol");
    return 0;
}

void getnexttoken()
{
    current_token = GetAToken();
}

// PARSER FUNCTIONS
ParseTree *Program()
{
    ParseTree stmtlist_pt;
    ParseTree *stmtlistptr= &stmtlist_pt;

    stmtlistptr = StmtList();
    
    //getnexttoken();
    
    // make sure there are no more tokens...
    //if( current_token.getTok() == DONE)
        //return 0;
    
    return stmtlistptr;
}

ParseTree *StmtList()
{
    //cout << "statement list" << endl;
    ParseTree *Stmpt = Stmt();
    
    if(Stmpt)
    {
        // any number statements can occur
        getnexttoken();
        if(current_token.getTok() == SC)
        {
            //cout << "statement list: SEMICOLON" << endl;

            ParseTree *Stlstpt = StmtList();
            
            if(current_token.getTok() == DONE)
            {
                cout << "DONE" << endl;
                return(Stmpt);
            }
            else if (current_token.getTok() == ERR)
            {
                cout << "ERROR" << endl;
            }
            else
            {
                if(Stlstpt != nullptr)
                {
                    return Stlstpt;
                }
                else
                {
                    
                }
            }
        }
        else
        {
            //error("Invalid statement: NO SEMICOLON");
        }
    }
    else
    {
        //error("Invalid statement: NULL RETURN FROM STATEMENT");
    }
    
    return nullptr;
    
}

ParseTree *Stmt()
{
    //cout << "statement" << endl;
    //ParseTree expr_pt = new ParseTree();
    //ParseTree *exprptptr = &expr_pt;
    
    ParseTree print_pt ;
    ParseTree *printptptr = &print_pt;

    ParseTree set_pt ;
    ParseTree *setptptr = &set_pt;
    
    getnexttoken();
    
    if((current_token.getTok() == DONE) || (current_token.getTok() == ERR))
    {
        return(nullptr);
    }
    else if( current_token.getTok() == PRINT )
    {
        
        //cout << "statement:print" << endl;
        ParseTree *pt = Expr();
        
        // process PRINT
        if(pt == nullptr)
        {
            return(0);
        }
        else
        {
            print_pt.setPTChildNode1(pt);
            getnexttoken();
            if(current_token.getTok() == SC)
            {
                ParseTree(print_pt);
                PushbackToken(current_token);
                return(printptptr);
            }
            else
            {
                error("Invalid statement");
                return(0);
            }
            
        }
        
    }
    else if( current_token.getTok() == SET )
    {
        //cout << "statement:set" << endl;

        // process SET
        getnexttoken();
        if(current_token.getTok() == ID )
        {
            //cout << "statement:id" << endl;
            ParseTree *pt = Expr();
            
            if(pt == nullptr)
            {
                error("Invalid statement");
                return(0);
            }
            else
            {
                set_pt.setPTChildNode1(pt);
                getnexttoken();
                if(current_token.getTok() == SC )
                {
                    ParseTree(setptpt);
                    PushbackToken(current_token);
                    return(setptptr);
                }
                else
                {
                    error("Invalid statement");
                    return(0);
                }
            }
        }
        else
            error("Invalid statement");
        
    }
    else
        error("Invalid statement");
    
    return 0;
}

ParseTree *Expr()
{
    //cout << "expression" << endl;

    ParseTree *ter = Term();
    
    if(ter)
    {
        getnexttoken();
        
        if( current_token.getTok() == PLUS )
        {
            //cout << "expression:plus" << endl;

            ParseTree *expr = Expr();
            countPlus++;
            if(expr)
            {
                return expr;
            }
            else
                return 0;
        }
        else
        {
            PushbackToken(current_token);
            return(ter);
            
        }
        
    }
    
    return 0;
}

ParseTree *Term()
{
    //cout << "Terminal" << endl;
    ParseTree *pri = Primary();
    
    if(pri)
    {
        getnexttoken();
        
        if( current_token.getTok() == STAR )
        {
            ParseTree *ter = Term();
            countStar++;
            
            if(ter)
            {
                return ter;
            }
            else
                return 0;
        }
        else
        {
            PushbackToken(current_token);
            return(pri);
        }
        
    }
    
    return 0;
}

ParseTree *Primary()
{
    //cout << "primary" << endl;
    getnexttoken();
    
    if( current_token.getTok() == ID )
    {
        //cout << "primary:id" << endl;
        return new IDParseTree(current_token);
        
    }
    else if( current_token.getTok() == INT )
    {
        //cout << "primary:int" << endl;
        return new IntegerParseTree(current_token);
    }
    else if( current_token.getTok() == LPAREN )
    {
         //cout << "primary:lparen" << endl;
        ParseTree *expr = Expr();
        
        if(expr)
        {
            if(!expect(RPAREN))
            {
                
                error("expected right parens");
                return 0;
            }
            else
            {
                cout << "primary:rparen" << endl;
                return(expr);
            }
        }
        else
        {
            return(0);
            
        }
    }
    else if( current_token.getTok() == STR)
    {
        ParseTree *str = String();
        //cout << "PRIMARY:STRING" << endl;

        if(str)
        {
            return(str);
        }
        else
        {
            return(0);
        }
        
    }
    
    return 0;
}

ParseTree *String()
{
    //getnexttoken();
    int leftsquarefound = false;
    //cout << "STRING:STR" << endl;
    ParseTree stringpt;
    ParseTree *strptptr = &stringpt;
        
    STRParseTree Strtoken(current_token);
    stringpt.setPTChildNode1(&Strtoken);
    
    getnexttoken();
        
    if(current_token.getTok() == LEFTSQ)
    {
        //cout << "STRING:LEFTSQ" << endl;
        leftsquarefound = true;
            
        ParseTree *exp = Expr();
            
        if(exp == nullptr)
        {
            return 0;
        }
        else
        {
            getnexttoken();
                
            if(current_token.getTok() == RIGHTSQ)
            {
                //cout << "STRING:RIGHTSQ" << endl;
                stringpt.setPTChildNode2(exp);
                leftsquarefound = false;
                return(strptptr);
            }
            else if(current_token.getTok() == SC)
            {
                //cout << "STRING:SC" << endl;
                ParseTree *exp1 = Expr();
                    
                if(exp1 == nullptr)
                {
                    return 0;
                }
                else
                {
                    getnexttoken();
                        
                    if(current_token.getTok() == RIGHTSQ)
                    {
                        //cout << "STRING:RIGHTSQ" << endl;
                        countSqr++;
                        stringpt.setPTChildNode3(exp1);
                        leftsquarefound = false;
                        return(strptptr);
                    }
                    else
                    {
                        error("expected right parens");
                        return 0;
                    }
                }
            }
            else
            {
                error("expected right parens");
                return 0;
            }
        }
    }
    else
    {
        PushbackToken(current_token);
        return(strptptr);
    }
}

