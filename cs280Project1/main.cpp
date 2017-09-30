//"/Users/Shreyas/Documents/ProgrammingConcepts/TextFormatter2/Test.txt"
//
//  main.cpp
//  textformatter
//
//  Created by Shreyas Ramanujam on 9/28/16.
//  Shreyas Ramanujam
//  Textformatter
//  CS280
//
//

#include<iostream>
#include<fstream>
#include<string>
using namespace std;

#define CHAR_COUNT 60
#define FILE_NAME "/Users/Shreyas/Documents/ProgrammingConcepts/Textformatter/Textformatter/p1-test1"
#define PATTERN ".ll "

void Formatter(ifstream& File);
void Spacesformatting(string &output_str);

void Formatter(ifstream& File)
{
    string Text1;
    string output_str = "";
    string new_character_count_str = "";
    
    size_t is_previous_line_empty = false;
    size_t character_count = CHAR_COUNT, total_char_count=0,new_char_count =0;
    
    
    while(getline(File,Text1, '\n'))
    {
        string input_str = "";
        total_char_count = Text1.length();
        
        if(Text1.empty())
        {
            // print out the string
            if( output_str.length() != 0)
            {
                Spacesformatting(output_str); // even out the spaces
                cout << output_str << endl ;
                output_str = "";
                
            }
            
            if(is_previous_line_empty == true )
            {
                cout << endl;
                is_previous_line_empty = false;
            }
            else
            {
                is_previous_line_empty = true;
                continue;
            }
            
        }
        
        while( !Text1.empty())
        {
            if(is_previous_line_empty == true )
            {
                is_previous_line_empty = false;
            }
            
            size_t patternposition = 0;
            size_t spaceposition = 0;
            size_t nullposition = 0;
            
            
            // reinitialize character count
            patternposition = Text1.rfind(PATTERN);
            
            if (patternposition != std::string::npos)
            {
                Text1.erase(0, 4);
                spaceposition = Text1.find_first_of(" ");
                
                if (spaceposition != std::string::npos)
                {
                    new_character_count_str = Text1.substr(0,spaceposition);
                    
                }
                else
                {
                    nullposition = Text1.length();
                    
                    if (nullposition != 0)
                    {
                        new_character_count_str = Text1.substr(0,nullposition);
                    }
                }
                
                if(new_character_count_str.find_first_not_of("0123456789") == std::string::npos)
                {
                    new_char_count = stoi(new_character_count_str,nullptr,0);
                    
                    if( new_char_count >= 10 && new_char_count <= 120 )
                    {
                        character_count = new_char_count;
                        
                    }
                }
                
                Text1.erase(0, new_character_count_str.length());
                continue;
            }
            
            // read the input sub string for the number of characters
            if(output_str.length() > 0)
            {
                input_str = Text1.substr(0,character_count-output_str.length());
            }
            else
            {
                input_str = Text1.substr(0,character_count);
            }
            
            
            Text1.erase(0, input_str.length());
            
            
            // Append the input string to outout string, this should work in both above cases
            output_str.append(input_str);
            
            // print out the string
            if( output_str.length() == character_count)
            {
                Spacesformatting(output_str); // even out the spaces
                cout << output_str << endl ;
                output_str = "";
                
            }
            else
            {
                if( output_str.length() == total_char_count)
                {
                    Spacesformatting(output_str); // even out the spaces
                    cout << output_str << endl ;
                    output_str = "";
                    
                }
            }
            
        }
    }
    
}

void Spacesformatting(string &output_str)
{
    int spaceCounter = 0;
    char * cstr = new char [output_str.length()+1];
    std::strcpy (cstr, output_str.c_str());
    
    for(int i = 0; i < output_str.length(); i++)
    {
        if(cstr[i] == ' ')
        {
            spaceCounter++;
        }
    }
    
    int numwords = 0; // Holds number of words
    bool isprevcharspace = false;
    
    for(int i = 0; cstr[i] != '\0'; i++)
    {
        if (cstr[i] == ' ') //Checking for spaces
        {
            if (isprevcharspace != true)
            {
                numwords++;
                isprevcharspace = true;
            }
        }
        else
        {
            if (isprevcharspace == true)
            {
                isprevcharspace = false;
            }
        }
    }
    
    int numofspaces = 0;
    
    if(spaceCounter == 0 || numwords == 0)
    {
        return;
    }
    else
    {
        numofspaces = spaceCounter/numwords;
        
        char *temp_str = new char [output_str.length()+1];
        
        int count = 0;
        temp_str[count] = '\0';
        isprevcharspace = false;
        
        for(int i = 0; cstr[i] != '\0'; i++)
        {
            if (cstr[i] == ' ') //Checking for spaces
            {
                if (isprevcharspace != true)
                {
                    isprevcharspace = true;
                    
                    for (int j = 0; j < numofspaces; j++)
                    {
                        temp_str[count] = ' ' ;
                        count++;
                    }
                }
            }
            else
            {
                if (isprevcharspace == true)
                {
                    isprevcharspace = false;
                }
                
                temp_str[count] = cstr[i] ;
                count++;
            }
        }
        
        temp_str[count] = '\0';
        
        output_str.clear();
        output_str.append(temp_str);
    }
    
}

int main(int argc, const char *argv[]) {
    
    ifstream myFile;
    for ( int i=1 ; i<argc ; i++)
    {
        cout << "Filename:" << argv[i] << endl;
        myFile.open( argv[i], ios::in );
        if(myFile.is_open())
        {
            cout << "opened file Filename:" << argv[i] << endl;
            Formatter(myFile);
            cout << "formatted File:" << argv[i] << endl;
            myFile.close();
            cout << "closed File:" << argv[i] << endl;
        }
        else cout << "Unable to open file\n";
        
    }
    
    
    return 0;
}
