#include<fstream>
#include<iostream>
#include<string>
#include"Parser.h"


int lab2() {
	std::string str, program;
	Storage *storage = new Storage();
	Parser test(storage);
	std::ifstream is;
	std::cout << "林逸帆" << '\t' << "计算机科学与技术2班"
		<< '\t' << "201530541753" << "\n\n";
	std::cout << "请输入测试程序" << '\n';
	std::cin >> program;
	std::cin.get();
	is.open(program);
	if (is) {
		std::cout << "\n语法分析、语义分析结果如下：\n";
		try {
			test.parse(is);
		}
		catch (string_error e) {
			e.print(std::cerr);
			delete storage;
			std::cin.get();
			return 1;
		}
		catch (anotation_error e) {
			e.print(std::cerr);
			delete storage;
			std::cin.get();
			return 1;
		}
		catch (character_error e) {
			e.print(std::cerr);
			delete storage;
			std::cin.get();
			return 1;
		}
		catch (word_error e) {
			e.print(std::cerr);
			delete storage;
			std::cin.get();
			return 1;
		}
		catch (syntax_error e) {
			e.print(std::cerr);
			std::cin.get();
			delete storage;
			return 1;
		}
		catch (type_error e) {
			e.print(std::cerr);
			std::cin.get();
			delete storage;
			return 1;
		}
		catch (declare_error e) {
			e.print(std::cerr);
			std::cin.get();
			delete storage;
			return 1;
		}
	}
	else {
		delete storage;
		std::cerr << "文件操作失败" << '\n';
		return 1;
	}
	test.printIntermediateCode(std::cout);
	std::cout << "\n输入任意字符退出程序\n";
	std::cin.get();
	delete storage;
	return 0;
}


//int main() {
//	return lab2();
//}