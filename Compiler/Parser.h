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
		std::runtime_error("�﷨����"),
		pos(pos) {}

	void print(std::ostream &out) {
		out << "��" << pos << "��: " 
			<< what() << "\n";
	}
};


class type_error :public std::runtime_error {
public:
	const unsigned pos;
	const std::string name;

	type_error(unsigned pos, 
		const std::string &name) :
		std::runtime_error("�������ʹ���"),
		pos(pos), name(name) {}

	void print(std::ostream &out) {
		out << "��" << pos << "��: "
			<< what() << " " << name << " \n";
	}
};


class declare_error :public std::runtime_error {
public:
	const unsigned pos;
	const std::string name;

	declare_error(unsigned pos, 
		const std::string &name) :
		std::runtime_error("δ��������"),
		pos(pos), name(name) {}

	void print(std::ostream &out) {
		out << "��" << pos << "��: "
			<< what() << " " << name << " \n";
	}
};

//Parser�����ô����ݵ��Զ����·�����ʵ���﷨����
//�ڽ����﷨������ͬʱ�����м�������ɣ�������Ԫʽ���
class Parser {

	Storage *storage;
    //���ɵ��м���봮
	std::vector<quaternary> IntermediateCode;
    //�ʷ�������õ�Token��
	std::vector<token> Tokens;
    //��¼�ı�������ÿ�ж�Ӧ�ĵ�һ��Tokenλ��
    //Ϊ�׳��쳣ʱ��д������Ϣ����
	std::vector<unsigned> rowMark;

	unsigned getRow(size_t);

    //��Ԫʽ����
	unsigned gen(const std::string &, 
		int, int, int);

    //��Ԫʽ�������ϲ�������
	void backpatch(unsigned, unsigned);
	unsigned merge(unsigned, unsigned);

    //�ж���һ��Token�Ƿ�Ϊ��Ӧ���ս��
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

    //ÿ�����ս����Ӧһ��������
    //���ݱ��ʽ������Ԫʽ
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
    //��������ļ����дʷ��������﷨������������Ԫʽ����
	void parse(std::ifstream &);
    //������ɵ���Ԫʽ����
	void printIntermediateCode(std::ostream &);

};
