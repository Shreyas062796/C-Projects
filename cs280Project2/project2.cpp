
//
//Implementation of the Lexical Analyzer
#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <fstream>
using namespace std;
#include "p2lex.h"
#define FILE "p2-test6in"

bool VERBOSE_MODE = false;

map <TokenType,int> tokenCount;
int	linenum;

void initializeTokenCount();

void initializeTokenCount()
{
    tokenCount[ID] = 0 ;
    tokenCount[STR] = 0;
    tokenCount[INT] = 0 ;
    tokenCount[PLUS] = 0 ;
    tokenCount[STAR] = 0;
    tokenCount[LPAREN] = 0;
    tokenCount[RPAREN] = 0;
    tokenCount[LEFTSQ] = 0;
    tokenCount[RIGHTSQ] = 0;
    tokenCount[SC] = 0;
    tokenCount[SET] = 0;
    tokenCount[PRINT] = 0;
}


int main(int argc, const char * argv[])
{
    ifstream myFile;
    char filename[100] = " " ;
    Token token ;
    bool inputfile = false;
    
    if(argc==1)
    {
        inputfile = false;
    }
    if(argc == 2)
    {
        if (strcmp(argv[1], "-v") == 0)
        {
            VERBOSE_MODE = true;
            inputfile = false;
        }
        else
        {
            inputfile = true;
            strcpy(filename,argv[1]);
        }
    }
    
    if( argc == 3)
    {
        if (strcmp(argv[1], "-v") == 0)
        {
            VERBOSE_MODE = true;
        }
        
        inputfile = true;
        strcpy(filename,argv[2]);
    }
    
    if(inputfile == true)
    {
        cout << "Filename:" << filename << endl;
        myFile.open(filename, ios::in );
        
        if(myFile.is_open())
        {
            cout << "opened file Filename:" << filename << endl;
            
            initializeTokenCount();
            linenum = 0;
            
            cout << "analyze File:" << filename << endl;
            for(;;)
            {
                token = getToken(&myFile);
                
                if((token.getTok() == DONE ) || (token.getTok() == ERR))
                {
                    break;
                }
            }
            
            if(token.getTok() == DONE )
            {
                // print token map
                cout << "==========================================================="  << endl;
                cout << "The number of ocurences of SET is " << tokenCount.find(SET)->second << endl;
                cout << "The number of ocurences of INT is " << tokenCount.find(INT)->second <<     endl;
                cout << "The number of ocurences of SC is " <<  tokenCount.find(SC)->second << endl;
                cout << "The number of ocurences of STR is " << tokenCount.find(STR)->second << endl;
                cout << "The number of ocurences of LPAREN is " << tokenCount.find(LPAREN)->second << endl;
                cout << "The number of ocurences of RPAREN is " << tokenCount.find(RPAREN)->second << endl;
                cout << "The number of ocurences of RIGHTSQ is " << tokenCount.find(RIGHTSQ)->second << endl;
                cout << "The number of ocurences of LEFTSQ is " << tokenCount.find(LEFTSQ)->second << endl;
                cout << "The number of ocurences of PRINT is " << tokenCount.find(PRINT)->second << endl;
                cout << "The number of ocurences of SET is " << tokenCount.find(SET)->second << endl;
                cout << "The number of ocurences of PLUS is " << tokenCount.find(PLUS)->second << endl;
                cout << "The number of ocurences of STAR is " << tokenCount.find(STAR)->second << endl;
                cout << "The number of ocurences of ID is " << tokenCount.find(ID)->second << endl;
                cout << "==========================================================="  << endl;
                
            }
            else if(token.getTok() == ERR )
            {
                cout << "==========================================================="  << endl;
                cout << "ERROR OCURRED: " << token.getLexeme()  << endl;
                cout << "==========================================================="  << endl;
            }
            
            myFile.close();
            cout << "closed File:" << filename << endl;
        }
        else cout << "Unable to open file\n";
    }
    else cout << "Error: No input file provided\n";
    
}

