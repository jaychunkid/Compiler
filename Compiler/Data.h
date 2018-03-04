#pragma once
#include<map>
#include<string>
#include<vector>
#define IDENTIFIER "<identifier>"
#define INTEGER "<integer>"
#define STRING "<string>"
#define UNDEFINED "<empty>"
#define MAX_RESERVE_WORD_LEN 9
#define MIN_RESERVE_WORD_LEN 2
#define FIRST_RESERVE_WORD 1
#define LAST_RESERVE_WORD 35


class Data {

	const static std::vector<std::string> coding_schedule;

public:
	const static std::vector<char> single_char_delimiter;
	const static std::vector<std::string> double_char_delimiter;
	const static std::vector<std::string> relation;
	const static std::vector<std::string> boolean;
	const static std::vector<std::string> type;

	static unsigned getCode(const std::string &);
	static std::string getValue(unsigned);

};

