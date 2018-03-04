#pragma once
#include<vector>
#include<string>
#define TEMPORARY_VARIABLE "<T>"

struct ide {
	std::string name = "";
	unsigned type = 0;
	bool is_declare = false;
};

//Storage类存储程序编译过程中的符号表
//词法分析过程向符号表添加符号
//语法分析过程为符号设置类型等信息
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
