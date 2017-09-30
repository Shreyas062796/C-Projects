//
//  main.cpp
//  Parsercs280
//
//
//  Copyright © 2016 Shreyas Ramanujam. All rights reserved.
//

#include "p2parser.h"
#include <string>
#include <istream>
#include <fstream>
#include <map>
using namespace std;

// defines the linenum declared as extern in p2lex.h
int	linenum = 0;
int globalErrorCount;

//map to store count of id's
map<string,int> symbols;

// map to store id values
map<string,evalresult> idValues;

extern string getPrintName(TokenType t);

int
main(int argc, char *argv[])
{
    istream *in = &cin;
    ifstream infile;
    
    char filename[100] = "/Users/lavanya/Desktop/p4testfiles/p4-test6.in";
    
    
    if( argc == 1)
    {
        infile.open(filename);
        
        if( !infile.is_open() )
        {
            cerr << "Could not open file named: " << filename << endl;
            return 1;
        }
        
        in = &infile;
    }
    else
    {
        for( int i = 1; i < argc; i++ )
        {
            if( in != &cin ) {
            cerr << "Cannot specify more than one file" << endl;
            return 1;
            }
        
            infile.open(argv[i]);
            if( infile.is_open() == false ) {
                cerr << "Cannot open file " << argv[i] << endl;
                return 1;
            }
            
            in = &infile;
        }
    }
    
    ParseTree *prog = Program(in);
    
    if( prog == 0 || globalErrorCount != 0 )
    {
        return 0;
    }
    
    // on successful parse, count +, star and []
    
    //cout << "Count of + operators: " << prog->traverseAndCount( &ParseTree::isPlus ) << endl;
    //cout << "Count of * operators: " << prog->traverseAndCount( &ParseTree::isStar ) << endl;
    //cout << "Count of [] operators: " << prog->traverseAndCount( &ParseTree::isBrack ) << endl;
    
    return 0;
}
