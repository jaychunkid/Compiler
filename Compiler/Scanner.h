#pragma once
#include<exception>
#include<iostream>
#include<string>
#include<vector>
#include"Storage.h"
#define VALUE_WORD -2
#define VALUE_NONE -1


struct token {
	unsigned type_index;
	int val_index;
};

std::ostream &operator<<(std::ostream &, const token &);
bool isEqual(const token &, const std::string &);


class anotation_error :public std::runtime_error {
public:
	const unsigned pos;
	const unsigned row;

	anotation_error(unsigned pos, unsigned row):
		std::runtime_error("ע�ͽ��ȱʧ"),
		pos(pos), row(row) {}

	void print(std::ostream &out) {
		out << "��" << row << "��, ��" 
			<< pos << "��: " << what() << "\n";
	}
};


class string_error :public std::runtime_error {
public:
	const unsigned pos;
	const unsigned row;

	string_error(unsigned pos, unsigned row):
		std::runtime_error("�ַ�������ȱʧ"),
		pos(pos), row(row) {}

	void print(std::ostream &out) {
		out << "��" << row << "��, ��"
			<< pos << "��: " << what() << "\n";
	}
};


class character_error :public std::runtime_error {
public:
	const unsigned pos;
	const unsigned row;
	const char c;

	character_error(unsigned pos, unsigned row, 
		char c) :std::runtime_error("�Ƿ��ַ�"), 
		pos(pos), row(row), c(c) {}

	void print(std::ostream &out) {
		out << "��" << row << "��, ��"
			<< pos << "��: " << what() 
			<< " '" << c << "'\n";
	}
};


class word_error :public std::runtime_error {
public:
	const unsigned pos;
	const unsigned row;
	const char c;

	word_error(unsigned pos, unsigned row, 
		char c) :std::runtime_error("��Ӧ���ֵ��ַ�"),
		pos(pos), row(row), c(c) {}

	void print(std::ostream &out) {
		out << "��" << row << "��, ��"
			<< pos << "��: " << what()
			<< " '" << c << "'\n";
	}
};

//Scanner������״̬��ԭ��ʵ�ִʷ�����������Token
class Scanner {

	Storage *storage;

    //Token����
	token getResult(const std::string &, int);

    //״̬�ж�
	bool isDigital(char);
	bool isString(char);
	bool isDelimiter(char);
	bool isChar(char);

	bool isEmpty(char);
	bool isDoubleCharDelimiter(const std::string &);
	bool isAnotation(const std::string &);
	bool isAnotationEnd(const std::string &);
	bool isReserveWord(const std::string &);

    //״̬����
	token identifier(const std::string &, size_t &);
	token delimiter(const std::string &, size_t &, unsigned);
	token integer(const std::string &, size_t &, unsigned);
	token string(const std::string &, size_t &, unsigned);

public:
	Scanner(Storage *storage) :storage(storage) {}
    //������������ַ�����λ�ã�ÿ��ȡһ�����ʷ���
    //Ϊ�˸��õ���ʾ������Ϣ����Ҫ�����ַ����ڴ����ı��е��к�
	token scan(const std::string &, size_t &, unsigned);
};
