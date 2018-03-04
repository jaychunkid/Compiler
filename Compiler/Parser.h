#pragma once
#include<exception>
#include<iostream>
#include<vector>
#include"Scanner.h"
#include"Storage.h"
#define EMPTY -1
#define EMPTY_CHAIN 0


struct quaternary {
	const std::string ope;
	int ide_l;
	int ide_r;
	int ide;
};


class syntax_error :public std::runtime_error {
public:
	const unsigned pos;

	syntax_error(unsigned pos) :
		std::runtime_error("语法错误"),
		pos(pos) {}

	void print(std::ostream &out) {
		out << "第" << pos << "行: " 
			<< what() << "\n";
	}
};


class type_error :public std::runtime_error {
public:
	const unsigned pos;
	const std::string name;

	type_error(unsigned pos, 
		const std::string &name) :
		std::runtime_error("变量类型错误"),
		pos(pos), name(name) {}

	void print(std::ostream &out) {
		out << "第" << pos << "行: "
			<< what() << " " << name << " \n";
	}
};


class declare_error :public std::runtime_error {
public:
	const unsigned pos;
	const std::string name;

	declare_error(unsigned pos, 
		const std::string &name) :
		std::runtime_error("未声明变量"),
		pos(pos), name(name) {}

	void print(std::ostream &out) {
		out << "第" << pos << "行: "
			<< what() << " " << name << " \n";
	}
};


class Parser {

	Storage *storage;
	std::vector<quaternary> IntermediateCode;
	std::vector<token> Tokens;
	std::vector<unsigned> rowMark;

	unsigned getRow(size_t);

	unsigned gen(const std::string &, 
		int, int, int);
	void backpatch(unsigned, unsigned);
	unsigned merge(unsigned, unsigned);

	bool isType(size_t);
	bool isBoolType(size_t);
	bool isIntType(size_t);
	bool isRelation(size_t);
	bool isBoolean(size_t);
	bool isEqual(const token &, 
		const std::string &);

	bool isProgram(size_t);
	bool isVariableDeclaration(size_t);
	bool isVariableDefinitionEnd(size_t);
	bool isCompoundStatement(size_t);
	bool isStatement(size_t);

	bool program(size_t &);
	bool variableDefinition(size_t &);
	bool identifierTable(size_t &, unsigned &);
	
	bool statement(size_t &, unsigned &);
	bool assignmentStatement(size_t &);
	bool ifStatement(size_t &, unsigned &);
	bool whileStatement(size_t &, unsigned &);
	bool repeatStatement(size_t &, unsigned &);
	bool compoundStatement(size_t &);
	bool statementList(size_t &);

	bool arithmeticExpression(size_t &, int &);
	bool item(size_t &, int &);
	bool divisor(size_t &, int &);
	bool arithmeticUnit(size_t &, int &);
	bool booleanExpression(size_t &, unsigned &, unsigned &);
	bool booleanItem(size_t &, unsigned &, unsigned &);
	bool booleanDivisor(size_t &, unsigned &, unsigned &);
	bool booleanUnit(size_t &, unsigned &, unsigned &);

public:

	Parser(Storage *storage) :storage(storage) {}
	void parse(std::ifstream &);
	void printIntermediateCode(std::ostream &);

};
