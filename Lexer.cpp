#include "pch.h"
#include <iostream>
#include <string>
#include <cctype>
#include <vector>		
#include <fstream>
#include <iomanip>

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


	



struct t_token {

	string token = "";
	string lexeme = "";

	void t_clear() {
		this->token = "";
		this->lexeme = "";
	}
};

//Structure for syntax analyzer 
struct sa {
	sa(vector<t_token> tokens);
	
	bool Rat20(); //R1
	void optFuncDef(t_token *&Token); //R2
	void funcDef(t_token* &Token); //R3
	void func(t_token* &Token); //R4
	void optParamList(t_token* &Token); //R5
	void paramList(t_token* &Token); //R6
	void param(t_token* &Token); //R7
	void qualifier(t_token* &Token); //R8
	void body(t_token* &Token);	//R9
	void optDecList(t_token* &Token); //R10
	void decList(t_token* &Token); //R11
	void decl(t_token* &Token); //R12
	void ids(t_token* &Token); //R13
	void statementList(t_token* &Token); //R14
	



private:
	vector<t_token> myTokens;
	
};

//creator?
sa::sa(vector<t_token> tokens) {
	myTokens = tokens;
}

//R1
bool sa::Rat20() {
	t_token *Token = &myTokens[0];

	printf("Starting lexical analzyer...\n");
	optFuncDef(Token);
	if (Token->lexeme == "$$") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
		optDecList(Token);
		statementList(Token);
	}
	else {
		printf("Expecting $$ token.\n");
		return false;
	}
	if (Token->token == "$$") {
		return true;
	}
	return false;
}
//R2
void sa::optFuncDef(t_token* &Token) {
	printf("Now entering R2...\n");
	funcDef(Token);

};

//R3
void sa::funcDef(t_token* &Token) {
	printf("Now entering R3...\n");
	func(Token);
};

//R4 
void sa::func(t_token* &Token) {
	cout <<"Now entering R4...\n";
	if (Token->lexeme == "function") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
	}
	if (Token->token == "Identifier") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
	}
	if (Token->lexeme == "(") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
	}
	optParamList(Token);
	if (Token->lexeme == ")") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
	}
	optDecList(Token);
};

//R5
void sa::optParamList(t_token* &Token) {
	cout << "Now entering R5...\n";
	paramList(Token);
	
};

//R6
void sa::paramList(t_token* &Token) {
	cout << "Now entering R6...\n";
	param(Token);
	if (Token->token == ",") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
		paramList(Token);
	}

}
//R7
void sa::param(t_token* &Token) {
	cout<<"Now entering R7...\n";
	ids(Token);
	qualifier(Token);
}

//R8
void sa::qualifier(t_token* &Token) {
	printf("Now entering R8...\n");
	if (Token->lexeme== "int" || Token->lexeme == "boolean" || Token->lexeme == "real") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
	}
};
//R9
void sa::body(t_token* &Token) {
	cout << "Now entering R9...\n";
	if (Token->lexeme == "{") {
		++Token;
		statementList(Token);
		if (Token->lexeme == "}") {
			++Token;
		}
	}
}
//R10
void sa::optDecList(t_token* &Token) {
	cout << "Now entering R10...\n";
	decList(Token);
};
//R11
void sa::decList(t_token* &Token) {
	cout << "Now entering R11...\n";
	decl(Token);
	if (Token->lexeme == ";") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
		decList(Token);
	}
	
}
//R12
void sa::decl(t_token* &Token) {
	cout << "Now entering R12...\n";
	qualifier(Token);
	ids(Token);
}
//R13
void sa::ids(t_token* &Token) {
	cout << "Now entering R13...\n";
	if (Token->token == "Identifier" || Token->token == "Keyword") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
	}
	if (Token->lexeme == ",") {
		++Token;
		cout << "New token is " << Token->lexeme << endl;
		ids(Token);
	}
}
//R14
void sa::statementList(t_token* &Token) {
	cout << "Now entering R14...\n";
	statement(Token);
}

//R15
void sa::statement(t_token* &Token) {
	if()
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
					myTokens.push_back(tempToken);

					cout << tempString << endl;
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
				myTokens.push_back(tempToken);

				tempToken.t_clear();

				tempString = "";
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
				myTokens.push_back(tempToken);

				tempToken.t_clear();

				tempString ="";
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
					myTokens.push_back(tempToken);

					tempToken.t_clear();

					tempString = "";
					currentState = 1;
				}
				else {
					cout << "Operator: \t" << tempString << endl;

					tempToken.token = "Operator";
					tempToken.lexeme = tempString;
					myTokens.push_back(tempToken);

					tempToken.t_clear();


					tempString = "";
					currentState = 1;
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
	}*/

	vector<t_token> myTokens;
	

	//Open text file with code
	string location = "c:/Users/Brandon/Desktop/me2.txt";//= argv[1]; 
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
	cout << "\nFinished.  \nThe output has been saved to: " + endLocation << endl;


	//Write lexemes and keywords to output file
	endFile << "Lexeme" << setw(9) << "" << "Token\n\n\n";
	for (t_token x : myTokens) {
		endFile << x.lexeme << setw(15 - x.lexeme.length()) << "" <<  x.token << endl;
	}


	//Always cleanup after yourself
	file.close();
	endFile.close();

	//Begin Syntax Analysis
	sa myAnaylzer(myTokens);
	myAnaylzer.Rat20();

	return 0;
}