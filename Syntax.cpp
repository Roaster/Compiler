#include "pch.h"
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
#include <stack>

							//Columns
using namespace std;		//0 1  2   3   4  5  6    7     8   |
						    //l d  _ space . sep op   *  random

vector<vector<int>> table { {2, 5, 16, 15, 16, 10, 11, 11, 16},      //1  Begin
							{2, 2, 3, 4, 2, 4, 4, 4, 4},			//2  Identifier formation
							{3, 3, 3, 4, 3, 4, 4, 4, 4},			//3  Identifier formation
							{1, 1, 1, 1, 1, 1, 1, 1, 1},			//4  Valid identifier
							{16, 5, 16, 6, 7, 6, 6, 6, 6},			//5  Int foration
							{1, 1, 1, 1, 1, 1, 1, 1, 1},			//6  Valid int
							{16, 8, 16, 16, 16, 16, 16, 16, 16},	//7  .
							{9, 8, 9, 9, 9, 9, 9, 9, 9},			//8  Real formation
							{1, 1, 1, 1, 1, 1, 1, 1, 1},			//9  Valid real
							{1, 1, 1, 1, 1, 10, 1, 1, 1},			//10  Seperator
							{12, 12, 12, 12, 12, 12, 12, 13, 12},	//11 Operator
							{1, 1, 1, 1, 1, 1, 1, 1, 1},			//12 Valid Operator
							{13, 13, 13, 13, 13, 13, 13, 14, 13},	//13 Comment
							{13, 13, 13, 13, 13, 13, 1, 13, 13},	//14 End Comment
 							{15, 15, 15, 1, 15, 15, 15, 15, 15},	//15 Space/Nothing
							{16, 16, 16, 16, 16, 16, 16, 16, 16}	   //16 Unknown
							};
//Global Variables
int lineCount = 1;
bool pRules = true;
int ADDRESS = 5000;
int instrAddress = 1;

//Structure for holding all of our tokens and information
struct t_token {

	string token = "";
	string lexeme = "";
	int lineNumber = 0;

	void t_clear() {
		this->token = "";
		this->lexeme = "";
		this->lineNumber = 0;
	}
};

int convertAddress(int address) {
	return address % 5000;
}

struct instruction {
	int address;
	string op;
	int oprnd;
};


//Structure for syntax analyzer
struct sa {

	sa(vector<t_token> tokens, string location);

	bool Rat20();							 //R1
	void optFuncDef(t_token *&Token);		 //R2
	void funcDef(t_token* &Token);			 //R3
	void func(t_token* &Token);				 //R4
	void optParamList(t_token* &Token);		 //R5
	void paramList(t_token* &Token);		 //R6
	void param(t_token* &Token);			 //R7
	void qualifier(t_token* &Token);		 //R8
	void body(t_token* &Token);				 //R9
	void optDecList(t_token* &Token);		 //R10
	void decList(t_token* &Token);			 //R11
	void decl(t_token* &Token);				 //R12
	void ids(t_token* &Token);				 //R13
	void statementList(t_token* &Token);     //R14
	void statement(t_token* &Token);         //R15
	void compound(t_token* &Token);          //R16
	void assigns(t_token* &Token);           //R17
	void ifs(t_token* &Token);               //R18
	void ret(t_token* &Token);               //R19
	void print(t_token* &Token);             //R20
	void scan(t_token* &Token);              //R21
	void whiles(t_token* &Token);            //R22
	void condition(t_token* &Token);         //R23
	void relop(t_token* &Token);             //R24
	void exp(t_token* &Token);               //R25
	void expP(t_token* &Token);              //R26
	void term(t_token* &Token);              //R27
	void termP(t_token* &Token);             //R28
	void factor(t_token* &Token);            //R29
	void primary(t_token* &Token);           //R30
	void emptys(t_token* &Token);            //R31

	//symbol table, gen_address, 2 stacks, 
	//check if in table
	//add to table
	//print out table
	void printSymbols();
	void printInstr();
	int get_address(string symbol);
	void gen_instr(string op, int oprnd);
	void back_patch(int address);
	void insertSymbol(string id, string type);


private:
	vector<t_token> myTokens;

	ofstream myFile;

	map<string, pair<int, string>> symbolTable;
	stack<int> jumpStack;
	stack<int> addStack;
	stack<string> myStack;
	vector<instruction> myIns;
};

//Constructor
sa::sa(vector<t_token> tokens, string location) {
    myFile.open(location);	
	myTokens = tokens;
}

int sa::get_address(string symbol) {
	if (symbolTable.find(symbol) != symbolTable.end()) {
		return symbolTable.find(symbol)->second.first;
	}
	else {
		return 2;
	}
}
void sa::insertSymbol(string id, string type) {
	pair<int, string> x;
	x.first = ADDRESS;
	ADDRESS++;
	x.second = type;

	symbolTable.insert(pair<string, pair<int, string>>(id, x) );
}
void sa::gen_instr(string op, int oprnd) {
	instruction x;
	x.address = instrAddress;
	x.op = op;
	x.oprnd = oprnd;
	++instrAddress;
	myIns.push_back(x);
}
void sa::back_patch(int jumpAddress) {
	int address = jumpStack.top();
	jumpStack.pop();
	myIns.at(address -1).oprnd = jumpAddress;
}
void sa::printInstr() {
	for (instruction x : myIns) {
		if (x.oprnd == -1) {
			cout << x.address << setw(7) << x.op << setw(7) << "NIL" << endl;
		}else {
			cout << x.address << setw(7) << x.op << setw(7) << x.oprnd << endl;
		}
	}
}
void sa::printSymbols() {
	map<string, pair<int, string>>::iterator iter = symbolTable.begin();
	while (iter != symbolTable.end()) {

		cout << iter->first << setw(7) << iter->second.first << endl;
		++iter;
	}
	cout << endl;
}

//Rules for RAT20 language
//R1
bool sa::Rat20() {
	t_token *Token = &myTokens[0];

	cout <<"Starting lexical analyzer...\n";
	if(pRules)
    {
        cout << "<Rat20F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$\n";
        myFile << "<Rat20F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$\n";
	

    }
	optFuncDef(Token);
	if (Token->lexeme == "$$") {
		++Token;
		//cout << "New lexeme is " << Token->lexeme << endl;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		optDecList(Token);
		statementList(Token);
	}
	else {
		cout <<"Expecting $$ token.\n";
		myFile <<"Expecting $$ token.\n";
		return false;
	}
	if (Token->lexeme == "$$") {
		return true;
	}
	return false;
}
//R2
void sa::optFuncDef(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R2...\n";
		cout << "<Opt Function Definitions> :: = <Function Definitions> | <Empty>\n";
		myFile << "<Opt Function Definitions> :: = <Function Definitions> | <Empty>\n";
	}
	funcDef(Token);

};

//R3
void sa::funcDef(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R3...\n";
		cout << "<Function Definitions>  ::= <Function> | <Function> <Function Definitions>\n";
        myFile << "<Function Definitions>  ::= <Function> | <Function> <Function Definitions>\n";

	}
	func(Token);
};

//R4
void sa::func(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R4...\n";
		cout << "<Function> ::= function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>\n";
		myFile << "<Function> ::= function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>\n";

	}
	if (Token->lexeme == "function")
    {
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		if (Token->token == "Identifier")
		{
            ++Token;
            cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
            myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
            if (Token->lexeme == "(")
            {
                ++Token;
                cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
                optParamList(Token);
                if (Token->lexeme == ")") {
                    ++Token;
                    cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                    myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
                }else {
                    cout << "Expected ')', but found '"
                    << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
                    myFile << "Expected ')', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
                    exit(1);
                }
            }else {
                cout << "Expected '(', but found '"
                << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
                myFile << "Expected '(', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
                exit(1);
            }
        }else {
            cout << "Expected 'Identifier', but found '"
            << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
            myFile << "Expected 'Identifier', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
            exit(1);
        }
        optDecList(Token);
        body(Token);
	}
};

//R5
void sa::optParamList(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R5...\n";
		cout << "<Opt Parameter List> ::=  <Parameter List>    |     <Empty>\n";
		myFile << "<Opt Parameter List> ::=  <Parameter List>    |     <Empty>\n";
	}
	paramList(Token);

};

//R6
void sa::paramList(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R6...\n";
		cout << "<Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>\n";
        myFile << "<Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>\n";
	}
	param(Token);
	if (Token->lexeme == ",") {
		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		if(Token->token != "Identifier") {
            cout << "Expected 'Identifier', but found '" << Token->lexeme
            << "'. Error found on line " << Token->lineNumber << endl;
            myFile << "Expected 'Identifier', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
            exit(1);
		}
		paramList(Token);

	}
}
//R7
void sa::param(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R7...\n";
		cout << "<Parameter> ::=  <IDs >  <Qualifier>\n";
		myFile << "<Parameter> ::=  <IDs >  <Qualifier>\n";

	}
	ids(Token);
	qualifier(Token);
}

//R8
void sa::qualifier(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R8...\n";
		cout << "<Qualifier> ::= int     |    boolean    |  real\n";
		myFile << "<Qualifier> ::= int     |    boolean    |  real\n";
	}
	if (Token->lexeme== "int" || Token->lexeme == "boolean" || Token->lexeme == "real") {
		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}
};
//R9
void sa::body(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R9...\n";
		cout << "<Body>  ::=  {  < Statement List>  }\n";
		myFile << "<Body>  ::=  {  < Statement List>  }\n";
	}
	if (Token->lexeme == "{") {
		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		statementList(Token);
		if (Token->lexeme == "}") {
			++Token;
			cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
			myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		}else {
            cout << "Expected '}', but found '"
            << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
            myFile << "Expected '}', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
            exit(1);
        }
	}else {
        cout << "Expected '{', but found '"
        << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected '{', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }

}
//R10
void sa::optDecList(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R10...\n";
		cout << "<Opt Declaration List> ::= <Declaration List>   |    <Empty>\n";
		myFile << "<Opt Declaration List> ::= <Declaration List>   |    <Empty>\n";
	}
	decList(Token);
};
//R11
void sa::decList(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R11...\n";
		cout << "<Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>\n";
		myFile << "<Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>\n";
	}
	decl(Token);
	if (Token->lexeme == ";") {
		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
	}
	if (Token->lexeme == "int" || Token->lexeme == "boolean" || Token->lexeme == "real") {
		decList(Token);
	}
}
//R12
void sa::decl(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R12...\n";
		cout << "<Declaration> ::=   <Qualifier > <IDs>\n";
		myFile << "<Declaration> ::=   <Qualifier > <IDs>\n";
	}
	if (Token->token == "Keyword") {
		qualifier(Token);
		ids(Token);
	}
}
//R13
void sa::ids(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R13...\n";
		cout << "<IDs> ::=     <Identifier>    | <Identifier>, <IDs>\n";
		myFile << "<IDs> ::=     <Identifier>    | <Identifier>, <IDs>\n";
	}
	if (Token->token == "Identifier" ) {
		
		//Add to symboltable and generate instruction
		if (symbolTable.find(Token->lexeme) != symbolTable.end()) {
			gen_instr("PUSHM", get_address(Token->lexeme));
		}
		else {
			insertSymbol(Token->lexeme, Token->token);
		}
		//gen_instr("PUSHM", get_address(Token->lexeme));

		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}
	if (Token->lexeme == ",") {
		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		if(Token->token != "Identifier") {
            cout << "Expected 'Identifier', but found '" << Token->lexeme
            << "'. Error found on line " << Token->lineNumber << endl;
            myFile << "Expected 'Identifier', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
            exit(1);
		}
		ids(Token);
	}
}

//R14
void sa::statementList(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R14...\n";
		cout << "<Statement List> :: = <Statement> | <Statement> <Statement List>\n";
		myFile << "<Statement List> :: = <Statement> | <Statement> <Statement List>\n";
	}

	statement(Token);

	if (Token->lexeme == "}" || Token->lexeme == ";" || Token->lexeme == "$$") {
		return;
		//compound(Token);
	}
	else {
		statementList(Token);
	}//maybe
	//cout << "Statement finished...\n";
}

//R15
void sa::statement(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R15...\n";
		cout << "<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>\n";
		myFile << "<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>\n";
	}
	if(Token->lexeme == "{") compound(Token);
	if(Token->token == "Identifier") assigns(Token);
	if(Token->lexeme =="if") ifs(Token);
	if(Token->lexeme =="return")ret(Token);
	if(Token->lexeme == "put")print(Token);
	if (Token->lexeme == "get")scan(Token);
	if(Token->lexeme == "while")whiles(Token);
}

//R16
void sa::compound(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R16...\n";
		cout << "<Compound> ::=   {  <Statement List>  }\n";
		myFile << "<Compound> ::=   {  <Statement List>  }\n";
	}
	if (Token->lexeme == "{") {
		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		statementList(Token);
		if (Token->lexeme == "}") {
			++Token;
			cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
			myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		}else {
            cout << "Expected '}', but found '"
            << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
            myFile << "Expected '}', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";

            exit(1);
        }
	}else {
        cout << "Expected '{', but found '"
        << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected '{', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }



}
//R17
void sa::assigns(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R17...\n";
		cout << "<Assign> ::=     <Identifier> = <Expression> ;\n";
		myFile << "<Assign> ::=     <Identifier> = <Expression> ;\n";
	}
	if (Token->token == "Identifier" || Token->token == "Keyword") {
		string savex = Token->lexeme;

		++Token;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		if (Token->lexeme == "=") {
            ++Token;
            cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
            myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
            exp(Token);
			//gen_instr("PUSHI", myStack.size());
			gen_instr("POPM", get_address(savex));
			//myStack.push(Token->lexeme);

            if (Token->lexeme == ";") {
                ++Token;
                cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
            }else {
                cout << "Expected ';', but found '" << Token->lexeme
                << "'. Error found on line " << Token->lineNumber << endl;
                myFile << "Expected ';', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
                exit(1);
            }
        }else {
            cout << "Expected '=', but found '" << Token->lexeme
            << "'. Error found on line " << Token->lineNumber << endl;
            myFile << "Expected '=', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
            exit(1);
        }
	}else {
        cout << "Expected 'Identifier', 'Keyword', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected 'Identifier', 'Keyword', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
}

//R18
void sa::ifs(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R18...\n";
		cout << "<If> ::=     if  ( <Condition>  ) <Statement>   fi   |   if (<Condition>) < Statement > else  <Statement>  fi\n";
		myFile << "<If> ::=     if  ( <Condition>  ) <Statement>   fi   |   if (<Condition>) < Statement > else  <Statement>  fi\n";
	}
	if (Token->lexeme == "if") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		if (Token->lexeme == "(") {
            Token++;
            cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
            myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
            condition(Token);
            if (Token->lexeme == ")") {
                Token++;
                cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
                statement(Token);
                if (Token->lexeme == "fi") {
                    Token++;
                    cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                    myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
                } else if (Token->lexeme == "else") {
                    Token++;
                    cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                    myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
                    statement(Token);
                    if (Token->lexeme == "fi") {
                        Token++;
                        cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                        myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
                    }else {
                        cout << "Expected 'fi', but found '" << Token->lexeme
                        << "'. Error found on line " << Token->lineNumber << endl;
                        myFile << "Expected 'fi', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
                        exit(1);
                    }
                }else {
                    cout << "Expected 'else', but found '" << Token->lexeme
                    << "'. Error found on line " << Token->lineNumber << endl;
                    myFile << "Expected 'else', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
                    exit(1);
                }
            }else {
                cout << "Expected ')', but found '" << Token->lexeme
                << "'. Error found on line " << Token->lineNumber << endl;
                myFile << "Expected ')', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
                exit(1);
            }
        }else {
            cout << "Expected '(', but found '" << Token->lexeme
            << "'. Error found on line " << Token->lineNumber << endl;
            myFile << "Expected '(', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
            exit(1);
        }
	}else {
        cout << "Expected 'if', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected 'if', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
}

//R19
void sa::ret(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R19...\n";
		cout << "<Return> ::=  return ; |  return <Expression> ;\n";
		myFile << "<Return> ::=  return ; |  return <Expression> ;\n";
	}
	if (Token->lexeme == "return") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		if (Token->lexeme == ";") {
            Token++;
            cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
            myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
        }
        else {
            exp(Token);
            if (Token->lexeme == ";") {
                Token++;
                cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
            }
            else {
                cout << "Expected ';', but found '" << Token->lexeme
                << "'. Error found on line " << Token->lineNumber << endl;
                myFile << "Expected ';', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
                exit(1);
            }
        }
    }else {
        cout << "Expected 'return', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected 'return', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }

}

//R20
void sa::print(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R20...\n";
		cout << "<Print> ::=    put ( <Expression>);\n";
		myFile << "<Print> ::=    put ( <Expression>);\n";
	}
	if (Token->lexeme == "put") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
        cout << "Expected 'put', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected 'put', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
	if (Token->lexeme == "(") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
        cout << "Expected '(', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected '(', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
	exp(Token);
	if (Token->lexeme == ")") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
        cout << "Expected ')', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected ')', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
	if (Token->lexeme == ";") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
        cout << "Expected ';', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected ';', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }

}

//R21
void sa::scan(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R21...\n";
		cout << "<Scan> ::=    get ( <IDs>);\n";
		myFile << "<Scan> ::=    get ( <IDs>);\n";
	}
	if (Token->lexeme == "get") {
		Token++;

		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
        cout << "Expected 'get', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected 'get', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
	if (Token->lexeme == "(") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
        cout << "Expected '(', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected '(', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
	//WORK FOR PART 3~~~~~~~~~~~~~~~
	gen_instr("STDIN", -1);
	ids(Token);
	if (Token->lexeme == ")") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
        cout << "Expected ')', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected ')', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
	if (Token->lexeme == ";") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
        cout << "Expected ';', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected ';', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
}

//R22
void sa::whiles(t_token* &Token) {
	int tempAddress;
	if (pRules) {
		//cout << "Now entering R22...\n";
		cout << "<While> ::=  while ( <Condition>  )  <Statement> \n";
		myFile << "<While> ::=  while ( <Condition>  )  <Statement> \n";
	}
	if (Token->lexeme == "while") {
		//part 3
		tempAddress = instrAddress;
		gen_instr("LABEL", -1);

		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}
	else {
        cout << "Expected 'while', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected 'while', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
	if (Token->lexeme == "(") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}else {
		cout << "Expected '(', but found '" << Token->lexeme
		<< "'. Error found on line " << Token->lineNumber << endl;
		myFile << "Expected '(', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
		exit(1);
	}
	condition(Token);
	if (Token->lexeme == ")") {
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}
	else {
        cout << "Expected ')', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected ')', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
	statement(Token);

	//part 3
	gen_instr("JUMP", tempAddress);
	back_patch(instrAddress);
}

//R23
void sa::condition(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R23...\n";
		cout << "<Condition> ::=     <Expression>  <Relop>   <Expression>\n";
		myFile << "<Condition> ::=     <Expression>  <Relop>   <Expression>\n";
	}
	exp(Token);
	string myOp = Token->lexeme;
	relop(Token);
	exp(Token);
	if (myOp == "<") {
		gen_instr("LES", -1);
		jumpStack.push(instrAddress);
		gen_instr("JUMPZ", -1);
	}
	myStack.pop();
}

//R24
void sa::relop(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R24...\n";
		cout << "<Relop> ::=   ==   |   !=    |   >     |   <    |  <=   |    =>\n";
		myFile << "<Relop> ::=   ==   |   !=    |   >     |   <    |  <=   |    =>\n";
	}
	if (Token->lexeme == "==" || Token->lexeme == "!=" ||
		Token->lexeme == ">" || Token->lexeme == "<" ||
		Token->lexeme == "<=" || Token->lexeme == ">=")
		{
            Token++;
            cout << "Token: " << Token->token <<"\tLexeme: " << Token->lexeme << endl;
            myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
    }else {
        cout << "Expected '==', '!=', '>', '<', '<=', '>=', but found '" << Token->lexeme
        << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected '==', '!=', '>', '<', '<=', '>=', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
}

//R25
void sa::exp(t_token* &Token) {
	//cout << "Now entering R25...\n";
	cout << "<Expression> ::=      <Term> <Expression Prime>\n";
	myFile << "<Expression> ::=      <Term> <Expression Prime>\n";
	term(Token);
	expP(Token);
}

//R26
void sa::expP(t_token* &Token) {
	//cout << "Now entering R26...\n";
	cout << "<Expression Prime> ::=      +  <Term> <Expression Prime>   |    -  <Term> <Expression Prime>   |   E\n";
	myFile << "<Expression Prime> ::=      +  <Term> <Expression Prime>   |    -  <Term> <Expression Prime>   |   E\n";
	if (Token->lexeme == "+" || Token->lexeme == "-") {
		//If + generate ADD instruction else generate SUB instruciton
		
		string tempToken = Token->lexeme;
		Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		term(Token);
		tempToken == "+" ? gen_instr("ADD", -1) : gen_instr("SUB", -1);
		expP(Token);
	}
}

//R27
void sa::term(t_token* &Token) {
	//cout << "Now entering R27...\n";
	cout << "<Term> ::=      <Factor> <Term Prime>\n";
	myFile << "<Term> ::=      <Factor> <Term Prime>\n";
	factor(Token);
	termP(Token);
}

//R28
void sa::termP(t_token* &Token) {
	//cout << "Now entering R28...\n";
	cout << "<Term Prime> ::=      *  <Factor> <Term Prime>   |    /  <Factor> <Term Prime>   |   E\n";
	myFile << "<Term Prime> ::=      *  <Factor> <Term Prime>   |    /  <Factor> <Term Prime>   |   E\n";
	if (Token->lexeme == "*" || Token->lexeme == "/") {

		//If * generate MUL instruction else generate DIV instruciton
		
		Token++;
		//cout << "New lexeme is " << Token->lexeme << endl;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
		factor(Token);
		Token->lexeme == "*" ? gen_instr("MUL", -1) : gen_instr("DIV", -1);
		termP(Token);
	}
}

//R29
void sa::factor(t_token* &Token) {
	if (pRules) {
		//cout << "Now entering R29...\n";
		cout << "<Factor> ::=      -  <Primary>    |    <Primary>\n";
		myFile << "<Factor> ::=      -  <Primary>    |    <Primary>\n";
	}
	if (Token->lexeme == "-") {
		Token++;
		//cout << "New lexeme is " << Token->lexeme << endl;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
	}
	primary(Token);
}

//R30
void sa::primary(t_token* &Token) {
    if (pRules) {
        //cout << "Now entering R30...\n";
        cout << "<Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |  <Real> | true | false\n";
        myFile << "<Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |  <Real> | true | false\n";
    }
    if (Token->token == "Identifier") {

		gen_instr("PUSHM", get_address(Token->lexeme));

        Token++;
        cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
        myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
        if (Token->lexeme == "(") {
            ++Token;
            cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
            myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
            ids(Token);
            if (Token->lexeme == ")") {
                ++Token;
                cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
                myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
            }
            else {
                cout << "Expected ')', but found '" << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
                myFile << "Expected ')', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
                exit(1);
            }
        }
    } else if (Token->token == "Integer" ||
        Token->token == "Real" ||
        Token->lexeme == "true" ||
        Token->lexeme =="false") {
		if (Token->token == "Real" || Token->token == "Integer") {
			gen_instr("PUSHI", myStack.size());
			myStack.push(Token->lexeme);
		}
		else {
			gen_instr("PUSHM", get_address(Token->lexeme));
		}
		
        Token++;
        cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
        myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
    }
    else if (Token->lexeme == "(") {
        Token++;
		cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
		myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
        exp(Token);
        /*cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
        myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";*/
        if (Token->lexeme == ")") {
            Token++;
            cout << "Token: " << Token->token << "\tLexeme: " << Token->lexeme << endl;
            myFile << "Token: " + Token->token + "\tLexeme: " + Token->lexeme + "\n";
        }
    }
    else {
        cout << "Expected 'Integer', '(', 'Real', 'true', 'false', but found '"
        << Token->lexeme << "'. Error found on line " << Token->lineNumber << endl;
        myFile << "Expected 'Integer', '(', 'Real', 'true', 'false', but found '" + Token->lexeme + "'. Error found on line " << Token->lineNumber << "\n";
        exit(1);
    }
}

//R31
void sa::emptys(t_token* &Token) {
	//cout << "Now entering R31...\n";
	cout << "<Empty> ::= E\n";
	myFile << "<Empty> ::= E\n";
}

bool isKeyword(string input) {
	return (input == "while" || input == "if" || input == "fi" || input == "function" ||
		input == "int" || input == "return" || input == "get" ||input=="put"||
		input=="boolean" ||input=="real"||input=="else"||input=="true"||input=="false");
}
bool isOp(char input) {
	return (input == '+' || input == '-' || input == '*' || input == '/' ||
		input == '!'|| input=='<' ||input=='>' ||input=='=');
}
bool isSep(char input) {
	return (input == ';' || input == '(' || input == ')' || input=='{' ||
		input == '}' || input=='$' || input==',');
}
bool isSpecialOp(char first, char second) {
	return ((first == '!' && second == '=') || (first == '<' && second == '=') ||
		(first == '=' && second == '>') ||
		(first == '=' && second == '='));
}


vector<t_token> lexer(string input) {
	//Set our transition table variables
	int currentState = 0;
	int column = 0;
	int row = 0;

	//Create a place to store our tokens
	vector<t_token> myTokens;
	t_token tempToken;


	string tempString = "";

	for (int i = 0; i < input.length(); ++i) {

		if (isalpha(input.at(i))) {
			column = 0;
		}
		else if (isdigit(input.at(i))) {
			column = 1;
		}
		else if (input.at(i) == '_') {
			column = 2;
		}
		else if (isspace(input.at(i)) ) { // || input.at(i) == ','
			if (input.at(i) == '\n') ++lineCount;
			column = 3;
		}
		else if (input.at(i) == '.' ) {
			column = 4;
		}
		else if (isSep(input.at(i))) {
			column = 5;
		}
		else if (input.at(i) == '*') {
			column = 7;
		}
		else if (isOp(input.at(i))) {
			column = 6;
		}
		else {
			column = 8;
		}

		currentState = table.at(row).at(column);

		switch (currentState) {
			case(1):
			case(2):
			case(3):
				//if (input.at(i) == ',') break;
				tempString.push_back(input.at(i));
				break;
			case(4):
				if (tempString != "") {
					isKeyword(tempString) ? cout << "Keyword: \t" : cout << "Identifier: \t";

					isKeyword(tempString) ? tempToken.token = "Keyword" : tempToken.token = "Identifier";
					tempToken.lexeme = tempString;
					tempToken.lineNumber = lineCount; //
					myTokens.push_back(tempToken);

					cout << tempString << endl;
					if (input.at(i) == '\n') --lineCount;
					--i;


					tempString = "";
					tempToken.t_clear();

					currentState = 1;
					break;
				}

				tempToken.t_clear();
				tempString = "";
				currentState = 1;
				break;
			case(5):
				tempString.push_back(input.at(i));
				break;
			case(6):

				cout << "Integer: \t" << tempString << endl;

				tempToken.token = "Integer";
				tempToken.lexeme = tempString;
				tempToken.lineNumber = lineCount; //
				myTokens.push_back(tempToken);

				tempToken.t_clear();

				tempString = "";

				if (input.at(i) == '\n') --lineCount;
				--i;


				currentState = 1;
				break;
			case(7):
				tempString.push_back(input.at(i));
				break;
			case(8):
				tempString.push_back(input.at(i));
				break;
			case(9):

				cout << "Real: \t\t" << tempString << endl;

				tempToken.token = "Real";
				tempToken.lexeme = tempString;
				tempToken.lineNumber = lineCount; //
				myTokens.push_back(tempToken);

				tempToken.t_clear();

				tempString ="";
				if (input.at(i) == '\n') --lineCount;
				--i;

				currentState = 1;
				break;
			case(10):

				if (tempString == "" && input.at(i) == '$') {
					tempString.push_back(input.at(i));
					break;
				}
				else if (tempString == "$" && input.at(i) == '$') {
					tempString.push_back(input.at(i));
					cout << "Separator: \t" << tempString << endl;

					tempToken.token = "Separator";
					tempToken.lexeme = tempString;
					tempToken.lineNumber = lineCount;

					myTokens.push_back(tempToken);

					tempToken.t_clear();


					tempString = "";
					currentState = 1;
					break;
				}
				else {
					cout << "Separator: \t" << input.at(i) << endl;

					tempToken.token = "Separator";
					tempToken.lexeme = input.at(i);
					tempToken.lineNumber = lineCount;
					myTokens.push_back(tempToken);

					tempToken.t_clear();


					tempString = "";
					currentState = 1;
					break;
				}
				break;
			case(11):

				tempString.push_back(input.at(i));
				break;
			case(12):

				if (isSpecialOp(*tempString.begin(), input.at(i))) {
					tempString.push_back(input.at(i));
					cout << "Operator: \t" << tempString << endl;

					tempToken.token = "Operator";
					tempToken.lexeme = tempString;
					tempToken.lineNumber = lineCount;
					myTokens.push_back(tempToken);

					tempToken.t_clear();

					tempString = "";
					currentState = 1;
				}
				else {
					cout << "Operator: \t" << tempString << endl;

					tempToken.token = "Operator";
					tempToken.lexeme = tempString;
					tempToken.lineNumber = lineCount;

					myTokens.push_back(tempToken);

					tempToken.t_clear();


					tempString = "";
					currentState = 1;
					if (input.at(i) == '\n') --lineCount;
					--i;

				}
				break;
			case(13):
				break;
			case(14):
				tempString = "";
				break;
			case(15):
				tempString = "";
				currentState = 1;
				break;
			case(16):
				if (isspace(input.at(i)) && tempString != "") {
					cout << "Unknown: \t" << tempString << endl;

					tempToken.token = "Unknown";
					tempToken.lexeme = tempString;
					tempToken.lineNumber = lineCount;
					myTokens.push_back(tempToken);

					tempToken.t_clear();

					tempString = "";
					currentState = 1;
					break;

				}
				else {
					tempString.push_back(input.at(i));
					break;
				}
		}
		row = currentState - 1;
	}
	return myTokens;
}
int main(int argc, char* argv[])
{
	//Check to make sure 3 parameters have been given
	/*if (argc != 3) {
		cout << "Please pass an input file and output file. Closing now...\n";
		return 1;
	}
	string location = argv[1];
	string endLocation = argv[2];
	*/
	vector<t_token> myTokens;


	//Open text file with code
	//Open text file with codeADD
	//string location = "c:/Users/Edward/Downloads/me2 (1).txt";//= argv[1];
	//string location = "c:/Users/Edward/Downloads/me2 (5).txt";//= argv[1];
	//string location = "c:/Users/Edward/Downloads/me1 (1).txt";//= argv[1];
	//string location = "c:/Users/Edward/Downloads/test1 (1).txt";//= argv[1];
    //string location = "c:/Users/Edward/Downloads/test1.txt";//= argv[1];
    //string location = "c:/Users/Edward/Downloads/test4.txt";//= argv[1];
    string location = "c:/Users/Brandon/Desktop/test1.txt";//= argv[1];
	string endLocation = "c:/Users/Brandon/Desktop/output.txt";//= argv[2];

	fstream file;
	ofstream endFile;

	//Open files to read and write
	endFile.open(endLocation);
	file.open(location);
	if (file.is_open() && endFile.is_open()) {
		cout << "Success\n\n";
	}

	//Gather all the contents into a string. Append \n to replicate actual code format
	string input = "";
	string fileContents = "";
	while (getline(file, input)) {
		fileContents += input;
		fileContents += '\n';
	};

	//Print fileContents to make sure it looks good
	cout << fileContents << endl;

	//This is where the magic happens
	myTokens = lexer(fileContents);
	//cout << "\nFinished.  \nThe output has been saved to: " + endLocation << endl;


	//Write lexemes and keywords to output file
	/*
	endFile << "Lexeme" << setw(9) << "" << "Token\n\n\n";
	for (t_token x : myTokens) {
		endFile << x.lexeme << setw(15 - x.lexeme.length()) << "" <<  x.token << endl;
	}
	*/

	//Always cleanup after yourself
	file.close();
	endFile.close();

	//Begin Syntax Analysis
	sa myAnaylzer(myTokens, endLocation);
	if (myAnaylzer.Rat20()) {
		cout << "\n The provided code is syntactically correct! \n";
	}
	else
        cout << "Provided code is not syntactically correct...\n";

	myAnaylzer.printSymbols();
	myAnaylzer.printInstr();
	return 0;
}
