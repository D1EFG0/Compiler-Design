#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    string filename;
    cout << "Enter the name of the text file to read: ";  //Prompt for FileName.txt
    cin >> filename;

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Could not open file." << endl;  //File error
        return 0;
    }

    string line;
    vector<string> tokens;
    while (getline(file, line)) //Gets lines from file.
    {
        for (int i = 0; i < line.length(); i++) // Check rest of tokens for match
        {
            if (line[i] == '$' && i+1 < line.length()) //Check for matching token(s) that begin with $
            {
                if (line[i+1] == '*')
                    tokens.push_back("MultiplicationTok");        //Assign token/names
                else if (line[i+1] == '/')                                  
                    tokens.push_back("DivisionTok");
                else if (line[i+1] == '%')
                    tokens.push_back("ModuloTok");
                else if (line[i+1] == '-')      
                    tokens.push_back("UnaryNegativeTok");
                else if (line[i+1] == '+')
                    tokens.push_back("UnaryPositiveTok");
                else if (line[i+1] == '!')
                    tokens.push_back("UnaryNOTTok");
                else if (i+2 < line.length() && line[i+1] == '+' && line[i+2] == '+')
                    tokens.push_back("IncrementOPTok");
                else if (i+2 < line.length() && line[i+1] == '-' && line[i+2] == '-')
                    tokens.push_back("DecrementOPTok");
                else if (line[i+1] == '=')
                    tokens.push_back("EqualToTok");
                else if (i+2 < line.length() && line[i+1] == '$' && line[i+2] == '$')
                    tokens.push_back("AssignmentOPTok");
                else if (line.substr(i+1, 8) == "FUNCTION")
                    tokens.push_back("$FUNCTION$");
                else if (line.substr(i+1, 4) == "SEND")
                    tokens.push_back("FuncReturnTok");
                else if (line.substr(i+1, 3) == "IN$")
                    tokens.push_back("IntegerTok");
                else if (line.substr(i+1, 3) == "RE$")
                    tokens.push_back("RealTok");
                else if (line.substr(i+1, 3) == "CH$")
                    tokens.push_back("Chartok");
                else if (line.substr(i+1, 3) == "ST$")
                    tokens.push_back("StringTok");
                else if (line.substr(i+1, 3) == "PT$")
                    tokens.push_back("PointerTok");
                else if (line.substr(i+1, 2) == "VO")
                    tokens.push_back("VoidTok");
                else if (line.substr(i+1, 2) == "BO")
                    tokens.push_back("Booltok");
                else if (line.substr(i+1, 2) == "IF")
                    tokens.push_back("IfTok");
                else if (line.substr(i+1, 4) == "THEN")
                    tokens.push_back("ThenTok");
                else if (line.substr(i+1, 5) == "BREAK")
                    tokens.push_back("BreakTok");
                else if (line.substr(i+1, 6) == "SPEND$")
                    tokens.push_back("ForTok");
                else if (line.substr(i+1, 7) == "PENDING")
                    tokens.push_back("WhileTok");
                else if (line.substr(i+ 1, 9) == "BRIEFCASE")
                    tokens.push_back("CaseTok");
                else if (line.substr(i+1, 2) == "BC")
                    tokens.push_back("IndivCaseTok");
                else if (line.substr(i+1, 4) == "READ")
                    tokens.push_back("InputTok");
                else if (line.substr(i+1, 5) == "WRITE")
                    tokens.push_back("OutputTok");
                    
                    
                }//END OF TOKENS THAT BEGIN WITH '$'    +  More Token Assignments 
            else if (line[i] == '*' && i+1 < line.length() && line[i+1] == '$')
                tokens.push_back("CommentTok");
            else if (line[i] == '*' && i+2 < line.length() && line[i+1] == '*' && line[i+2] == '$')
                tokens.push_back("MultilineCommentTok");
            else if (line[i] == '+')
                tokens.push_back("CodeBlockTok");
            else if (i+1 < line.length() && line[i] == '/' && line[i+1] == '\\')
                tokens.push_back("GreaterThanTok");
            else if (i+1 < line.length() && line[i] == '\\' && line[i+1] == '/')
                tokens.push_back("LessThanTok");
            else if (i+2 < line.length() && line[i] == '\\' && line[i+1] == '=' && line[i+2] == '/')
                tokens.push_back("LessThanEqualTok");
            else if (i+2 < line.length() && line[i] == '/' && line[i+1] == '=' && line[i+2] == '\\')
                tokens.push_back("GreaterThanEqualTok");
            else if (line[i] == '>')
                tokens.push_back("RightArrowTok");
            else if (line[i] == '<')
                tokens.push_back("LeftArrowTok");
            else if (line[i] == ']')
                tokens.push_back("RightBracketTok");
            else if (line[i] == '[')
                tokens.push_back("LeftBracketTok");
            else if (line[i] == '(')
                tokens.push_back("LeftParenTok");
            else if (line[i] == ')')
                tokens.push_back("RightParenTok");
            else if (line[i] == ':')
                tokens.push_back("ColonTok");
            //else if (line[i] == '#')
                //tokens.push_back("HashTok");
            if (isspace(line[i])) continue; // skip white space

            // check for bool literal
            if (line.substr(i, 4) == "true" || line.substr(i, 5) == "false")
            {
                tokens.push_back("BoolLitTok_" + line.substr(i, line[i+4] == 'e' ? 5 : 4));
                i += line[i+4] == 'e' ? 4 : 3;
            } //CHECK FOR HEX INT REAL AND STRING LITERALS
            else if (isdigit(line[i]) || (line[i] == '0' && i+1 < line.length() && tolower(line[i+1]) == 'x') || line[i] == '#') // check for integer, hex, or string literal
            {
                string token = "";
                bool isHex = false;
                bool isReal = false;
                bool isString = false;
                if (line[i] == '#') {
                    isString = true;
                    token += line[i];
                    i++;
                }
            while (i < line.length() && ((isdigit(line[i]) && !isString) || line[i] == '.' || (tolower(line[i]) >= 'a' && tolower(line[i]) <= 'f') || (isString && line[i] != '#'))) // keep collecting (except for the closing hashtag)
            {
                if (isString && line[i] == '#') {
                    token += line[i];
                    break;
                }
            token += line[i];
            if (tolower(line[i]) == 'x') isHex = true;
                if (line[i] == '.' || tolower(line[i]) == 'e') isReal = true;
                    i++;
            }
            i--; // adjust index to account for the last increment in the for loop
            if (isHex) tokens.push_back("HexLitTok_" + token);
                else if (isReal) tokens.push_back("RealLitTok_" + token);
                else if (isString) tokens.push_back("StrLitTok_" + token);
                else tokens.push_back("IntLitTok_" + token);
            }

        else if (line[i] == '\'') // check for character literal
        {
            string character = "";
            if (i+2 < line.length() && line[i+2] == '\'') // a single character inside ''
            {
                character += line[i+1];
                i += 2; // skip over the single character and closing '
            }
            else if (i+4 < line.length() && line[i+4] == '\'') // an escape sequence inside ''
            {
                if (line[i+1] == '\\') // backslash escape sequence
                {
                    if (line[i+2] == 'n') character = "\n";
                    else if (line[i+2] == 't') character = "\t";
                    else if (line[i+2] == '\'') character = "\'";
                    else if (line[i+2] == '\"') character = "\"";
                    else if (line[i+2] == '\\') character = "\\";
                    i += 4; // skip over the escape sequence and closing '
                }
                else // a regular character followed by a character not equal to '
                {
                    character += line[i+1];
                    i += 2; // skip over the regular character
                }
            }
            if (!character.empty()) tokens.push_back("CharLitTok_" + character);
        }
                else if (line[i] == '\"') // check for string literal
                {
                    string str = "";
                    i++; // skip the opening "
                    while (i < line.length() && line[i] != '\"') // keep collecting characters until closing "
                    {
                        if (line[i] == '\\') // escape sequence
                        {
                            if (i+1 < line.length())
                            {
                                if (line[i+1] == 'n') str += "\n";
                                else if (line[i+1] == 't') str += "\t";
                                else if (line[i+1] == '\"') str += "\"";
                                else if (line[i+1] == '\'') str += "\'";
                                else if (line[i+1] == '\\') str += "\\";
                                i++; // skip over the escape sequence character
                            }
                        }
                        else
                        {
                            str += line[i];
                        }
                        i++;
                    }
                if (line[i] == '\"') tokens.push_back("StringLitTok_" + str);
                }
                else    // no token found
                {
                    // do nothing
                }
        }// end of loop that iterates through characters of line
    }// end of loop that iterates through lines of file


file.close();

cout << "\nTokens found:" << tokens.size() << "\n\n";
for (int i = 0; i < tokens.size(); i++)
{
    cout << tokens[i] << endl;
}

return 0;

}