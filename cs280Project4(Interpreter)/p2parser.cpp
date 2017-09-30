//
//  p2parser.cpp
//  Parsercs280
//
//  
//  Copyright © 2016 Shreyas Ramanujam. All rights reserved.
//

#include "p2parser.h"


/// Token Wrapper
Token saved;
bool isSaved = false;

Token GetAToken(istream *in) {
    if( isSaved ) {
        isSaved = false;
        return saved;
    }
    
    return getToken(in);
}

void PushbackToken(Token& t) {
    if( isSaved ) {
        cerr << "Can't push back more than one token!!!";
        exit(0);
    }
    
    saved = t;
    isSaved = true;
}

/// error handler
void error(string msg, bool showline=true)
{
    //static int globalErrorCount=0;
    
    if(showline)
        cout << linenum << ": ";
    cout << msg << endl;
    ++globalErrorCount;
}


ParseTree *Program(istream *in)
{
    ParseTree *result = StmtList(in);
    
    //evaluate the tree
    result->traverseAndFindSetPrint();
    
    // make sure there are no more tokens...
    if( GetAToken(in).getTok() != DONE )
        return 0;
    
    return result;
}


ParseTree *StmtList(istream *in)
{
    ParseTree *stmt = Stmt(in);
    
    if( stmt == 0 )
        return 0;
    
    return new Slist(stmt, StmtList(in));
}


ParseTree *Stmt(istream *in)
{
    Token t;
    
    t = GetAToken(in);
    
    if( t.getTok() == ERR ) {
        error("Invalid token");
        return 0;
    }
    
    if( t.getTok() == DONE )
        return 0;
    
    if( t.getTok() == PRINT ) {
        // process PRINT
        ParseTree *ex = Expr(in);
        
        if( ex == 0 ) {
            error("Expecting expression after print");
            return 0;
        }
        
        if( GetAToken(in).getTok() != SC ) {
            error("Missing semicolon");
            return 0;
        }
        
        return new PrintStmt(ex);
    }
    else if( t.getTok() == SET ) {
        // process SET
        Token tid = GetAToken(in);
        
        if( tid.getTok() != ID ) {
            error("Expecting identifier after set");
            return 0;
        }
        
        ParseTree *ex = Expr(in);
        
        if( ex == 0 ) {
            error("Expecting expression after identifier");
            return 0;
        }
        
        if( GetAToken(in).getTok() != SC ) {
            error("Missing semicolon");
            return 0;
        }
        
        //create a new SetStmt object and return
        return new SetStmt(tid.getLexeme(), ex);
    }
    else {
        error("Syntax error, invalid statement");
    }
    
    return 0;
}


ParseTree *Expr(istream *in)
{
    ParseTree *exp = Term( in );
    
    if( exp == 0 ) return 0;
    
    while( true ) {
        
        Token t = GetAToken(in);
        
        if( t.getTok() != PLUS ) {
            PushbackToken(t);
            break;
        }
        
        ParseTree *exp2 = Term( in );
        if( exp2 == 0 ) {
            error("missing operand after +");
            return 0;
        }
        
        exp = new PlusOp(exp, exp2);
    }
    
    return exp;
}


ParseTree *Term(istream *in)
{
    ParseTree *pri = Primary( in );
    
    if( pri == 0 ) return 0;
    
    while( true ) {
        
        Token t = GetAToken(in);
        
        if( t.getTok() != STAR ) {
            PushbackToken(t);
            break;
        }
        
        ParseTree *pri2 = Primary( in );
        if( pri2 == 0 ) {
            error("missing operand after *");
            return 0;
        }
        
        pri = new StarOp(pri, pri2);
    }
    
    return pri;
}


ParseTree *Primary(istream *in)
{
    Token t = GetAToken(in);
    
    if( t.getTok() == ID ) {
        return new Identifier(t);
    }
    else if( t.getTok() == INT ) {
        return new Integer(t);
    }
    else if( t.getTok() == STR ) {
        PushbackToken(t);
        return String(in);
    }
    else if( t.getTok() == LPAREN ) {
        ParseTree *ex = Expr(in);
        if( ex == 0 )
            return 0;
        t = GetAToken(in);
        if( t.getTok() != RPAREN ) {
            error("expected right parens");
            return 0;
        }
        
        return ex;
    }
    
    return 0;
}


ParseTree *String(istream *in)
{
    Token t = GetAToken(in); // I know it's a string!
    ParseTree *lexpr, *rexpr;
    
    Token lb = GetAToken(in);
    if( lb.getTok() != LEFTSQ ) {
        PushbackToken(lb);
        return new StringConst(t);
    }
    
    lexpr = Expr(in);
    if( lexpr == 0 ) {
        error("missing expression after [");
        return 0;
    }
    
    lb = GetAToken(in);
    if( lb.getTok() == RIGHTSQ ) {
        return new BracketOp(t, lexpr);
    }
    else if( lb.getTok() != SC ) {
        error("expected ; after first expression in []");
        return 0;
    }
    
    rexpr = Expr(in);
    if( rexpr == 0 ) {
        error("missing expression after ;");
        return 0;
    }
    
    lb = GetAToken(in);
    if( lb.getTok() == RIGHTSQ ) {
        return new BracketOp(t, lexpr, rexpr);
    }
    
    error("expected ]");
    return 0;
}

