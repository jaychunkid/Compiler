#pragma once
#include<vector>
#include<string>
#define TEMPORARY_VARIABLE "<T>"

struct ide {
	std::string name = "";
	unsigned type = 0;
	bool is_declare = false;
};

class Storage {

	std::vector<ide> Identifers;
	unsigned tem_count = 0;
	
public:

	unsigned setIdentifer(const std::string &);
	void setDeclare(unsigned);
	void setType(unsigned, unsigned);

	bool isDeclare(unsigned);
	bool isSameType(unsigned, unsigned);

	unsigned getType(unsigned);
	std::string getName(unsigned);

	void clear();

};
