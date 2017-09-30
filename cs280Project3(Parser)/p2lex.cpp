#include <string>
#include <fstream>
#include <map>
using namespace std;
#include "p2lex.h"

extern map <TokenType,int> tokenCount;
extern bool VERBOSE_MODE;

Token getToken(istream* instream);

ostream& operator<<(ostream& out, const Token& t)
{
    TokenType ttype = t.tok;
    
    switch(ttype)
    {
        case INT:
            out << "int (" << t.lexeme << ")" ;
            break;
        case STR:
            out << "str (" << t.lexeme << ")";
            break;
        case PLUS:
            out << "plus";
            break;
        case STAR:
            out << "star";
            break;
        case LEFTSQ:
            out << "leftSquare";
            break;
        case RIGHTSQ:
            out << "rightSquare";
            break;
        case PRINT:
            out << "print";
            break;
        case SET:
            out << "set";
            break;
        case SC:
            out << "semicolon";
            break;
        case LPAREN:
            out << "leftParen";
            break;
        case RPAREN:
            out << "rightParen";
            break;
        case ID:
            out << "id (" << t.lexeme << ")";
            break;
        case DONE:
            out << "done";
            break;
        case ERR:
            out << "err";
            break;
        default:
            out << "undefined";
            break;
    }
    
    return out;
}

Token getToken(istream* instream)
{
    char ch;
    string word_str = "" ;
    string int_str = "" ;
    string string_str = "";
    Token status;
    
    int token_found = false;
    
    if( instream->eof() )
    {
        Token done(DONE,"DONE");
        status = done;
        if(VERBOSE_MODE == true)
            cout << status << endl;
        return(status);
    }
    
    while(instream->get(ch)) //loop until a token is returned
    {
        if (ch == '\n')
        {
            linenum++;
            if(VERBOSE_MODE == true)
                cout << "End of Line" << endl;
            continue;
        }
        else if(ch == ' ')
        {
            continue;
        }
        else if (ch == '/')
        {
            // Ignore comment starting with // to the end of line
            
            instream->get(ch);
            
            if (ch == '/')
            {
                while (instream->get(ch))
                {
                    if(ch == '\n')
                    {
                        linenum++;
                        break;
                    }
                }
            }
            else
                instream->unget();
            
            continue;
        }
        else if(ch == ';')
        {
            //Found a semi colon
            tokenCount[SC]++;
            token_found = true;
            Token semicolon(SC,";");
            status = semicolon;
            
            if(VERBOSE_MODE == true)
                cout << status << endl;
            
            break;
            
        }
        else if (ch == '[')
        {
            // FOund a open bracket
            tokenCount[LEFTSQ]++;
            token_found = true;
            //cout << "LEFTSQUARE" << endl;
            Token leftsq(LEFTSQ,"[");
            status = leftsq;
            
            if(VERBOSE_MODE == true)
                cout << status << endl;
            break;
        }
        else if (ch == ']')
        {
            // FOund a close bracket
            tokenCount[RIGHTSQ]++;
            token_found = true;
            //cout << "RIGHTSQUARE" << endl;
            Token rightsq(RIGHTSQ, "]");
            status = rightsq;
            
            if(VERBOSE_MODE == true)
                cout << status << endl;
            break;
        }
        else if (ch == '+')
        {
            // Found a plus
            tokenCount[PLUS]++;
            token_found = true;
            //cout << "PLUS" << endl;
            Token opplus(PLUS, "+");
            status = opplus ;
            
            if(VERBOSE_MODE == true)
                cout << status << endl;
            
            break;
            
            
        }
        else if(isdigit(ch))
        {
            // increment int counter
            int_str += ch;
            
            while (instream->get(ch))
            {
                if(isdigit(ch))
                {
                    int_str += ch;
                }
                else
                {
                    instream->unget();
                    break;
                }
            }
            
            tokenCount[INT]++;
            token_found = true;
            Token intger(INT, int_str);
            status = intger ;
            if(VERBOSE_MODE == true)
                cout << status << endl;
            int_str = "" ;
            
            break;
            
            
            
        }
        else if (ch == '*')
        {
            tokenCount[STAR]++;
            token_found = true;
            Token star(STAR,"*");
            status = star;
            
            if(VERBOSE_MODE == true)
                cout << status << endl;
            
            break;
        }
        else if (ch == ')')
        {
            tokenCount[RPAREN]++;
            token_found = true;
            Token rightparen(RPAREN,"(");
            status = rightparen;
            
            if(VERBOSE_MODE == true)
                cout << status << endl;
            
            break;
            
        }
        else if (ch == '(')
        {
            tokenCount[LPAREN]++;
            token_found = true;
            Token leftparen(LPAREN,")");
            status = leftparen;
            if(VERBOSE_MODE == true)
                cout << status << endl;
            break;
            
        }
        else if(ch == '"')
        {
            string_str += ch;
            
            if(instream->eof())
            {
                Token err(ERR,string_str);
                status = err;
                
                cout << status << "(" << string_str << ")" << endl;
                
                string_str = "" ;
                
            }
            else
            {
                while (instream->get(ch))
                {
                    if(ch == '"')
                    {
                        string_str += ch;
                        
                        // increment string count
                        tokenCount[STR]++;
                        token_found = true;
                        
                        //print string_str
                        Token str(STR,string_str);
                        status = str;
                        
                        if(VERBOSE_MODE == true)
                            cout << status << endl;
                        
                        string_str = "" ;
                        break;
                    }
                    else if( ch == '\n')
                    {
                        Token err(ERR,string_str);
                        status = err;
                        
                        
                        cout << status << "(" << string_str << ")" << endl;
                        
                        string_str = "" ;
                        break;
                    }
                    else
                    {
                        string_str += ch;
                        
                        if(instream->eof())
                        {
                            Token err(ERR,string_str);
                            status = err;
                            
                            cout << status << "(" << string_str << ")" << endl;
                            
                            string_str = "" ;
                            break;
                        }
                        
                    }
                    
                }
                
            }
        }
        else if(isalpha(ch))
        {
            word_str += ch;
            
            while (instream->get(ch))
            {
                if(isalpha(ch))
                {
                    word_str += ch; // Found a ID or
                }
                else
                {
                    instream->unget();
                    break;
                }
            }
            
            if (strcmp(word_str.c_str(), "set") == 0)
            {
                tokenCount[SET]++;
                token_found = true;
                Token set(SET,"set");
                status = set;
                //cout << "SET" << endl;
                
                if(VERBOSE_MODE == true)
                    cout << status << endl;
                word_str = "" ;
                break;
                
                
            }
            else if (strcmp(word_str.c_str(), "print") == 0)
            {
                tokenCount[PRINT]++;
                token_found = true;
                Token print(PRINT,"print");
                status = print;
                //cout << "PRINT" << endl;
                
                if(VERBOSE_MODE == true)
                    cout << status << endl;
                word_str = "" ;
                break;
            }
            else
            {
                //if ID increment ID token type
                tokenCount[ID]++;
                token_found = true;
                Token identify(ID,word_str);
                status = identify;
                //cout << "ID("<< word_str << ")" << endl;
                word_str = "" ;
                
                // print the word
                if(VERBOSE_MODE == true)
                    cout << status << endl;
                break;
                
            }
        }
        
        if(token_found == true)
            break;
        
        if(status.getTok() == ERR)
            break;
        
        if(instream->eof())
        {
            Token done(DONE,"DONE");
            status = done;
            if(VERBOSE_MODE == true)
                cout << status << endl;
            break;
        }
        
    }// end of while
    
    
    return status;
    
}
