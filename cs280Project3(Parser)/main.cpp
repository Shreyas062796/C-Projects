//#include "p2lex.h"
#include "p2parser.h"
#include <string>
#include <istream>
#include <fstream>
#include <map>
#define FILENAME "/Users/lavanya/Desktop/p3-testfiles/p3-test4in"
using namespace std;
bool VERBOSE_MODE = false;

istream *in;
int linenum = 0;
int globalErrorCount = 0;
map <TokenType,int> tokenCount;
int countPlus = 0;
int countStar = 0;
int countSqr = 0;

extern ParseTree *Program();
extern void error(string msg);

int main(int argc, const char * argv[])
{
    fstream myFile;
    char filename[100] = " " ;
    Token token ;
    bool inputfile = false;
    ParseTree *progptr;
    
    
    if(argc==1)
    {
        inputfile = true;
        strcpy(filename,FILENAME);
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
        //cout << "Filename:" << filename << endl;
        myFile.open(filename, ios::in );
        
        if(myFile.is_open())
        {
            //cout << "opened file Filename:" << filename << endl;
            
            //initializeTokenCount();
            linenum = 0;
            
            //cout << "analyze File:" << filename << endl;
            
            // make sure file is not empty...
            in = &myFile;
            
            if(!in->eof())
            {
                progptr = Program();
                
                cout << "Count of + operators:" << countPlus << endl;
                cout << "Count of * operators:" << countStar << endl;
                cout << "Count of [] operators:" << countSqr << endl;
                cout << "Success. Congrats!" << endl;

                if (globalErrorCount != 0)
                {
                    //print the error count
                    
                }
                    
                 
            }
            else
                error("empty file");
            
            myFile.close();
            //cout << "closed File:" << filename << endl;
        }
        else
        {
            cout << "Unable to open file\n";
        }
    }
    else
        cout << "Error: No input file provided\n";
    
    return 0;
}
