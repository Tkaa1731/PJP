#include "iostream"
#include "string"
#include <list>
#define STR(a) (a-'0')
using namespace std;

class Lexicon {
public:
	string Token;
	string Value;

	friend ostream& operator<<(ostream& os, const Lexicon& obj) {
		os << obj.Token << obj.Value;
		return os;
	}
};
class Identifier : Lexicon {
public:
	Identifier(string v) {
		this->Token = "ID:	";
		this->Value = v;
	};
};
class Number : Lexicon {
private:
	int value;
public:
	Number(int n) {
		this->Token = "NUM:	";
		this->Value = to_string(n);
		this->value = n;
	}
	void AddDigit(int d) {
		this->value = this->value * 10 + d;
		this->Value = to_string(this->value);
	}
};
class Operator : Lexicon {
public:
	Operator(char op) {
		string s(1, op);
		this->Token = "OP:	";
		this->Value = s;
	}
};
class Delimiter : Lexicon {
public:
	Delimiter(char dlm) {
		switch (dlm)
		{
		case '(':
			this->Token = "LPAR";
			break;
		case ')':			
			this->Token = "RPAR";
			break;
		case ';':
			this->Token = "SEMICOLON";
			break;
		default:
			break;
		}
	}
};
class Keyword : Lexicon {
public:
	Keyword(string kw) {
		if (kw == "div")
			this->Token = "DIV";
		else
			this->Token = "MOD";
	}
};
void Execute(string text) {
	string operators = "+-*/";
	string delimiters = "();";
	list<Lexicon*> list{};

	for (int i = 0; i < text.length(); i++)
	{
		if (operators.find(text[i]) != string::npos) {
			if (i != text.length() - 1 && text[i + 1] == '/') {//COMMENT
				while (text[i] != '\n' && i != text.length() - 1)// ignore until end of line
					i++;
				continue;
			}
			else {//OPPERATOR
				Operator* o = new Operator(text[i]);
				list.push_back((Lexicon*)o);
			}
		}
		else if (delimiters.find(text[i]) != string::npos) {// DELIMITER
			Delimiter* d = new Delimiter(text[i]);
			list.push_back((Lexicon*)d);
		}
		else if (isdigit(text[i])) {// NUMBER
			Number* n = new Number(STR(text[i]));
			while (i != text.length() - 1 && isdigit(text[i+1]))
				n->AddDigit(STR(text[++i]));
			list.push_back((Lexicon*)n);
		}
		else if(isalpha(text[i])) {
			string s;
			s += text[i];
			while (i != text.length() - 1 && isalpha(text[i+1]))
				s += text[++i];
			if (s == "div" || s == "mod") { // KEYWORD
				Keyword* kw = new Keyword(s);
				list.push_back((Lexicon*)kw);
			}
			else{// IDENTIFIER
				Identifier* id = new Identifier(s);
				list.push_back((Lexicon*)id);
			}
		}
	}
	cout << "------- LEXICON -------" << endl;
	for (Lexicon* l : list) {
		cout << *l << endl;
		delete l;
	}
}
int main() {
	/*To run that code ENTER input formated like an example :
	  2+3;
	  3 mod hello; 
	  END
	*/
	string input;
	string line;
	cout << "Enter your code ended by \\n END:"<< endl;
	while (getline(cin, line) && line.rfind("END") == string::npos)
		input.append(line+'\n');
	Execute(input);
}
/*
	INPUT:   -2 + (245 div 3);  // note \n 2 mod 3 * hello
	The input may be containing the following symbols:

	identifiers - consisting of a sequence of letters and numbers starting with a letter
	numbers - formed by a sequence of decimal digits
	operators - symbols '+', '-', '*' and '/',
	delimiters - symbols '(', ')' and ';',
	keywords - div and mod.
	Symbols can be separated by a sequence of spaces, tabs, and line breaks.

	There can be notes in the input. Notes are preceded by a sequence // and continue to the end of the line.\n

	White spaces and notes does not produce any lexical symbols.
*/
