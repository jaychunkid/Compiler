#include<iostream>
#include<fstream>
#include<string>
#include"Scanner.h"
#include"Storage.h"


int lab1() {
	std::string str, program;
	Storage *storage = new Storage();
	Scanner test(storage);
	std::ifstream is;
	std::cout << "林逸帆" << '\t' << "计算机科学与技术2班"
		<< '\t' << "201530541753" << "\n\n";
	std::cout << "请输入测试程序" << '\n';
	std::cin >> program;
	std::cin.get();
	is.open(program);
	if (is) {
		std::cout << "\n词法分析结果如下：\n";
		unsigned count = 0;
		unsigned row = 0;
		while (std::getline(is, str)) {
			for (unsigned i = 0; i < str.length();) {
				try {
					token result = test.scan(str, i, row);
					if (result.val_index != VALUE_NONE) {
						++count;
						std::cout << result << '\t';
						if (count % 5 == 0)
							std::cout << '\n';
					}
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
			}
			++row;
		}
		is.close();
	}

	delete storage;
	std::cout << "\n输入任意字符退出程序\n";
	std::cin.get();
	return 0;
}


//int main() {
//	return lab1();
//}
