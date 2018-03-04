#include<iterator>
#include<algorithm>
#include"Data.h"
using iter = std::vector<std::string>::const_iterator;


const std::vector<std::string> Data::coding_schedule = {
	UNDEFINED, "and", "array", "begin", "bool", "call", "case", 
	"char", "constant", "dim", "do", "else", "end", 
	"false", "for", "if", "input", "integer", "not", 
	"of", "or", "output", "procedure", "program", 
	"read", "real", "repeat", "set", "stop", "then", 
	"to", "true", "until", "var", "while", "write",
	IDENTIFIER, INTEGER, STRING, "(", ")", "*", "*/", 
	"+", ",", "-", ".", "..", "/", "/*", ":", ":=", 
	";", "<", "<=", "<>", "=", ">", ">=", "[", "]"
};

const std::vector<char> Data::single_char_delimiter = {
	'<', '>', ':', '/', '*', '.', '+', '-',
	'=', '(', ')', '[', ']', ';', ','
};

const std::vector<std::string> Data::double_char_delimiter = {
	"*/", "..", "/*", ":=", "<=", "<>", ">="
};

const std::vector<std::string> Data::relation = {
	"<", "<>", "<=", ">=", ">", "="
};

const std::vector<std::string> Data::boolean = {
	"true", "false"
};

const std::vector<std::string> Data::type = {
	"integer", "bool", "char"
};


unsigned Data::getCode(const std::string &word) {

	iter pos = std::find(coding_schedule.cbegin(),
		coding_schedule.cend(), word);
	if (pos != coding_schedule.cend())
		return pos - coding_schedule.cbegin();
	else 
		return 0;
}


std::string Data::getValue(unsigned code) {

	if (code >= coding_schedule.size())
		return UNDEFINED;
	else
		return coding_schedule[code];
}




