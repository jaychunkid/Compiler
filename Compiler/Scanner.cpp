#include<algorithm>
#include<iterator>
#include"Scanner.h"
#include"Data.h"
using iter = std::vector<std::string>::const_iterator;


std::ostream &operator<<(std::ostream &out, const token &w) {
	out << "(" << w.type_index << ", ";
	if (w.val_index == VALUE_WORD)
		out << "-)";
	else
		out << w.val_index << ')';

	return out;
}


token Scanner::getResult(const std::string &str,
	int index = VALUE_NONE) {
	if (str == UNDEFINED || str == "/*") {
		return token{
			Data::getCode(str), 
			VALUE_NONE 
		};
	}
	else if (str == INTEGER || str == STRING || 
		str == IDENTIFIER) {
		return token{
			Data::getCode(str),
			index
		};
	}
	else {
		return token{
			Data::getCode(str),
			VALUE_WORD
		};
	}
}


bool Scanner::isDigital(char c) {

	if (c >= '0' && c <= '9')
		return true;
	else
		return false;
}


bool Scanner::isString(char c) {

	if (c == '\'')
		return true;
	else
		return false;
}


bool Scanner::isDelimiter(char c) {

	for (char x : Data::single_char_delimiter)
		if (c == x)
			return true;

	return false;
}


bool Scanner::isChar(char c) {

	if ((c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z'))
		return true;
	else
		return false;
}


bool Scanner::isEmpty(char c) {

	if (c == 9 || c == 10 || c == 13 || c == 32)
		return true;
	else
		return false;
}


bool Scanner::isDoubleCharDelimiter(const std::string &str) {

	for (const std::string &s : Data::double_char_delimiter)
		if (str == s)
			return true;

	return false;
}


bool Scanner::isAnotation(const std::string &str) {

	if (str == "/*")
		return true;
	else
		return false;
}


bool Scanner::isAnotationEnd(const std::string &str) {

	if (str == "*/")
		return true;
	else
		return false;
}


bool Scanner::isReserveWord(const std::string &str) {

	if (str.length() > MAX_RESERVE_WORD_LEN ||
		str.length() < MIN_RESERVE_WORD_LEN)
		return false;

	if (Data::getCode(str) >= FIRST_RESERVE_WORD && 
		Data::getCode(str) <= LAST_RESERVE_WORD)
		return true;

	return false;
}


token Scanner::identifier(const std::string &str,
	size_t &i) {

	size_t j = i + 1;
	for (; j < str.length(); ++j) {
		if (!isDigital(str[j]) && !isChar(str[j])) 
			break; 
	}

	std::string ide = str.substr(i, j - i);
	i += ide.length();
	if (isReserveWord(ide))
		return getResult(ide);
	else {
		int index = storage->setIdentifer(ide);
		return getResult(IDENTIFIER, index);
	}
}


token Scanner::delimiter(const std::string &str,
	size_t &i, unsigned r) {

	if (str.length() > i + 1) {
		std::string substr = str.substr(i, 2);
		if (isDoubleCharDelimiter(substr)) {
			if (isAnotation(substr)) {
				size_t j = i + 2;
				for (; j < str.length() - 1; ++j)
					if (str.substr(j, 2) == "*/") {
						i = j + 2;
						return getResult("/*");
					}
				throw anotation_error(i, r);
			}
			else if (isAnotationEnd(substr)) 
				throw word_error(i, r, str[i]);
			else {
				i += 2;
				return getResult(substr);
			}
		}
	}
	
	return getResult(str.substr(i++, 1));
}


token Scanner::integer(const std::string &str,
	size_t &i, unsigned r) {

	size_t j = i + 1;
	for (; j < str.length(); ++j)
		if (!isDigital(str[j])) {
			if (isDelimiter(str[j]) ||
				isEmpty(str[j]))
				break;
			else
				throw word_error(j, r, str[j]);
		}

	std::string value = str.substr(i, j - i);
	int index = storage->setIdentifer(value);
	i = j;
	return getResult(INTEGER, index);
}


token Scanner::string(const std::string &str,
	size_t &i, unsigned r) {

	size_t j = i + 1;
	for (; j < str.length(); ++j) {
		if (isString(str[j]))
			break;
	}

	if (j == str.length())
		throw string_error(i, r);

	else {
		std::string value = str.substr(i, j - i + 1);
		int index = storage->setIdentifer(value);
		i = j + 1;
		return getResult(STRING, index);
	}
}


token Scanner::scan(const std::string &str, size_t &i, unsigned r) {

	for (; i < str.length(); ++i) {

		if (isEmpty(str[i]))
			continue;

		else if (isString(str[i])) 
			return string(str, i, r);

		else if (isDigital(str[i]))
			return integer(str, i, r);

		else if (isChar(str[i])) 
			return identifier(str, i);

		else if(isDelimiter(str[i]))
			return delimiter(str, i, r);

		else
			throw character_error(i, r, str[i]);
	}

	return getResult(UNDEFINED);
}
