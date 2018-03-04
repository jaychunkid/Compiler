#include<iostream>
#include<fstream>
#include<string>
#include"Data.h"
#include"Parser.h"


unsigned Parser::getRow(size_t i) {
	for (unsigned j = 0; i < rowMark.size(); ++j)
		if (i < rowMark[j])
			return j;

	return rowMark.size();
}


unsigned Parser::gen(const std::string &ope,
	int ide_l, int ide_r, int ide) {

	IntermediateCode.push_back(quaternary{
		ope, ide_l, ide_r, ide });
	return IntermediateCode.size() - 1;
}


void Parser::backpatch(unsigned p, unsigned t) {

	for (int i = p; i != EMPTY;) {
		int tmp = IntermediateCode[i].ide;
		IntermediateCode[i].ide = t;
		i = tmp;
	}
}


unsigned Parser::merge(unsigned p1, unsigned p2) {

	if (p2 == EMPTY_CHAIN)
		return p1;

	else {
		int i;
		for (i = p2; IntermediateCode[i].ide != EMPTY;) 
			i = IntermediateCode[i].ide;

		IntermediateCode[i].ide = p1;
		return p2;
	}
}


bool Parser::isType(size_t code) {

	for (const std::string &str : Data::type)
		if(code == Data::getCode(str))
			return true;

	return false;
}


bool Parser::isBoolType(size_t code) {

	if (code == Data::getCode("bool"))
		return true;
	else
		return false;
}


bool Parser::isIntType(size_t code) {

	if (code == Data::getCode("integer"))
		return true;
	else
		return false;
}


bool Parser::isRelation(size_t code) {

	for (const std::string &str : Data::relation)
		if (code == Data::getCode(str))
			return true;

	return false;
}


bool Parser::isBoolean(size_t code) {

	for (const std::string &str : Data::boolean)
		if (code == Data::getCode(str))
			return true;

	return false;
}


bool Parser::isEqual(const token &t,
	const std::string &str) {

	if (t.type_index == Data::getCode(str))
		return true;
	else
		return false;
}


bool Parser::isProgram(size_t i) {

	if (i + 2 < Tokens.size() &&
		isEqual(Tokens[i], "program") &&
		isEqual(Tokens[i + 1], IDENTIFIER) &&
		isEqual(Tokens[i + 2], ";"))
		return true;
	else
		return false;
}


bool Parser::isVariableDeclaration(size_t i) {

	if (i < Tokens.size() &&
		isEqual(Tokens[i], "var"))
		return true;
	else
		return false;
}


bool Parser::isVariableDefinitionEnd(size_t i) {

	if (i + 2 < Tokens.size() &&
		isEqual(Tokens[i], ":") &&
		isType(Tokens[i + 1].type_index) &&
		isEqual(Tokens[i + 2], ";"))
		return true;
	else
		return false;
}


bool Parser::isStatement(size_t i) {
	if (i < Tokens.size() && (
		isEqual(Tokens[i], IDENTIFIER) ||
		isEqual(Tokens[i], "if") ||
		isEqual(Tokens[i], "while") ||
		isEqual(Tokens[i], "repeat")))
		return true;
	else
		return false;
}


bool Parser::isCompoundStatement(size_t i) {

	if (i < Tokens.size() &&
		isEqual(Tokens[i], "begin"))
		return true;
	else
		return false;
}


bool Parser::program(size_t &i) {

	gen("program", Tokens[i + 1].val_index,
		EMPTY, EMPTY);

	i += 3;

	if (i < Tokens.size()) {

		if (isVariableDeclaration(i)) {
			if (!variableDefinition(++i))
				return false;
		}
		else
			return false;
		
		if (isCompoundStatement(i)) {
			if (!compoundStatement(i))
				return false;
		}
		else
			return false;
	}
	else 
		return false;

	if (i == Tokens.size() - 1 &&
		isEqual(Tokens[i], ".")) {
		
		gen("sys", EMPTY, EMPTY, EMPTY);
		return true;
	}
	else
		return false;
}


bool Parser::variableDefinition(size_t &i) {

	bool mark = false;
	unsigned type = 0;

	while (i < Tokens.size() &&
		isEqual(Tokens[i], IDENTIFIER)) {
		mark = true;
		if (!identifierTable(i, type))
			return false;
	}

	return mark;
}


bool Parser::identifierTable(size_t &i, unsigned &type) {

	storage->setDeclare(Tokens[i].val_index);
	size_t mark = i;

	if (i + 1 < Tokens.size() &&
		isEqual(Tokens[i + 1], ",")) {

		i += 2;

		if (i < Tokens.size() &&
			isEqual(Tokens[i], IDENTIFIER)) {

			if (identifierTable(i, type)) {
				storage->setType(
					Tokens[mark].val_index, type);
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	else if (i + 3 < Tokens.size() &&
		isEqual(Tokens[i + 1], ":") &&
		isType(Tokens[i + 2].type_index) &&
		isEqual(Tokens[i + 3], ";")) {

		type = Tokens[i + 2].type_index;
		storage->setType(
			Tokens[mark].val_index, type);
		i += 4;
		return true;
	}

	else
		return false;

}


bool Parser::statement(size_t &i, unsigned &chain) {

	if (i < Tokens.size()) {
		if (isEqual(Tokens[i], IDENTIFIER))
			return assignmentStatement(i);
		else if (isEqual(Tokens[i], "if"))
			return ifStatement(i, chain);
		else if (isEqual(Tokens[i], "while"))
			return whileStatement(i, chain);
		else if (isEqual(Tokens[i], "repeat"))
			return repeatStatement(i, chain);
	}

	return false;
}


bool Parser::assignmentStatement(size_t &i) {

	i += 1;

	if (i < Tokens.size() &&
		isEqual(Tokens[i], ":=")) {
		
		unsigned mark = i;
		int ide = EMPTY;
		if (arithmeticExpression(++i, ide)) {

			if (!storage->isSameType(
				Tokens[mark - 1].val_index, ide))
				throw type_error(getRow(mark - 1), 
					storage->getName(
						Tokens[mark - 1].val_index));

			else {
				gen(":=", ide,
					EMPTY, Tokens[mark - 1].val_index);
				return true;
			}
		}
	}
		
	return false;
}


bool Parser::ifStatement(size_t &i, unsigned &chain) {

	i += 1;
	unsigned tl, fl, then_chain = EMPTY_CHAIN,
		else_chain = EMPTY_CHAIN;

	if (booleanExpression(i, tl, fl))

		if (i < Tokens.size() &&
			isEqual(Tokens[i], "then")) {

			backpatch(tl, IntermediateCode.size());

			if (statement(++i, then_chain)) {
				if (i < Tokens.size() &&
					isEqual(Tokens[i], "else")) {

					unsigned mark = IntermediateCode.size();
					gen("j", EMPTY, EMPTY, EMPTY);
					backpatch(fl, IntermediateCode.size());
					unsigned tmp_chain = merge(mark, then_chain);
					
					if (statement(++i, else_chain)) {
						chain = merge(tmp_chain, else_chain);
						return true;
					}
				}
				else {
					chain = merge(fl, then_chain);
					return true;
				}
			}
		}

	return false;
}


bool Parser::whileStatement(size_t &i, unsigned &chain) {

	i += 1;
	unsigned tl, fl, tmp_chain = EMPTY_CHAIN, 
		mark = IntermediateCode.size();

	if (booleanExpression(i, tl, fl)) {

		if (i < Tokens.size() &&
			isEqual(Tokens[i], "do")) {
			
			backpatch(tl, IntermediateCode.size());
			
			if (statement(++i, tmp_chain)) {

				backpatch(tmp_chain, mark);
				gen("j", EMPTY, EMPTY, mark);
				chain = fl;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}


bool Parser::repeatStatement(size_t &i, unsigned &chain) {

	i += 1;
	unsigned tl, fl;
	unsigned mark = IntermediateCode.size();

	if (statement(i, chain)) {

		if (i < Tokens.size() &&
			isEqual(Tokens[i], "until")) {
			if (booleanExpression(++i, tl, fl)) {
				chain = tl;
				backpatch(fl, mark);
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}


bool Parser::compoundStatement(size_t &i) {
	
	i += 1;
	if (statementList(i)) {

		if (i < Tokens.size() &&
			isEqual(Tokens[i], "end")) {
			++i;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}


bool Parser::statementList(size_t &i) {

	i -= 1;
	unsigned chain = EMPTY_CHAIN;
	do {
		++i;
		if (!statement(i, chain))
			return false;
		else {
			if (chain != EMPTY_CHAIN)
				backpatch(chain, IntermediateCode.size());
		}
	} while (i < Tokens.size() &&
		isEqual(Tokens[i], ";"));

	return true;
}


bool Parser::arithmeticExpression(size_t &i, int &ide) {

	if (item(i, ide)) {

		if (i < Tokens.size() && (
			isEqual(Tokens[i], "+") ||
			isEqual(Tokens[i], "-"))) {

			unsigned mark = i;
			int ide_r = EMPTY;
			if (arithmeticExpression(++i, ide_r)) {
				int index = storage->
					setIdentifer(TEMPORARY_VARIABLE);
				storage->setType(index, 
					storage->getType(ide));
				gen(Data::getValue(Tokens[mark].
					type_index), ide, ide_r, index);
				ide = index;
				return true;
			}
			else
				return false;
		}
		else 
			return true;
	}
	else
		return false;

}


bool Parser::item(size_t &i, int &ide) {

	if (divisor(i, ide)) {

		if (i < Tokens.size() && (
			isEqual(Tokens[i], "*") ||
			isEqual(Tokens[i], "/"))) {

			unsigned mark = i;
			int ide_r = EMPTY;

			if (item(++i, ide_r)) {
				int index = storage->
					setIdentifer(TEMPORARY_VARIABLE);
				storage->setType(index,
					storage->getType(ide));
				gen(Data::getValue(Tokens[mark].
					type_index), ide, ide_r, index);
				ide = index;
				return true;
			}
			else
				return false;
		}
		else 
			return true;

	}
	else
		return false;

}


bool Parser::divisor(size_t &i, int &ide) {

	if (i < Tokens.size() &&
		isEqual(Tokens[i], "-")) {

		unsigned mark = i;

		if (divisor(++i, ide)) {
			int index = storage->
				setIdentifer(TEMPORARY_VARIABLE);
			storage->setType(index, 
				storage->getType(ide));
			gen(Data::getValue(Tokens[mark].
				type_index), ide, EMPTY, index);
			ide = index;
			return true;
		}
		else
			return false;
	}

	else 
		return arithmeticUnit(i, ide);
}


bool Parser::arithmeticUnit(size_t &i, int &ide) {
	
	if (i < Tokens.size()) {
		
		if (isEqual(Tokens[i], IDENTIFIER)) {
			
			if (storage->isDeclare(
				Tokens[i].val_index) &&
				isIntType(storage->getType(
					Tokens[i].val_index))) {

				ide = Tokens[i].val_index;
				++i;
				return true;
			}
			else
				throw declare_error(getRow(i), 
					storage->getName(
						Tokens[i].val_index));

		}
		else if (isEqual(Tokens[i], INTEGER)) {
			
			storage->setType(Tokens[i].val_index, 
				Data::getCode("integer"));
			ide = Tokens[i].val_index;
			++i;
			return true;
		}
		else if (isEqual(Tokens[i], "(")) {
			
			if (arithmeticExpression(++i, ide)) {
				if (isEqual(Tokens[i], ")")) {
					++i;
					return true;
				}
			}
		}
	}

	return false;
}


bool Parser::booleanExpression(size_t &i,
	unsigned &tl, unsigned &fl) {

	unsigned tl_1, fl_1, tl_2, fl_2;

	if (booleanItem(i, tl_1, fl_1)) {

		if (i < Tokens.size() &&
			isEqual(Tokens[i], "or")) {

			backpatch(fl_1, IntermediateCode.size());
			booleanExpression(++i, tl_2, fl_2);
			fl = fl_2;
			tl = merge(tl_1, tl_2);
			return true;
		}
		else {
			tl = tl_1;
			fl = fl_1;
			return true;
		}
	}
	else
		return false;

}


bool Parser::booleanItem(size_t &i,
	unsigned &tl, unsigned &fl) {

	unsigned tl_1, fl_1, tl_2, fl_2;
	
	if (booleanDivisor(i, tl_1, fl_1)) {
		
		if (i < Tokens.size() &&
			isEqual(Tokens[i], "and")) {
			
			backpatch(tl_1, IntermediateCode.size());
			booleanItem(++i, tl_2, fl_2);
			tl = tl_2;
			fl = merge(fl_1, fl_2);
			return true;
		}
		else {
			tl = tl_1;
			fl = fl_1;
			return true;
		}
	}
	else
		return false;

}


bool Parser::booleanDivisor(size_t &i,
	unsigned &tl, unsigned &fl) {

	if (i < Tokens.size() &&
		isEqual(Tokens[i], "not"))
		return booleanDivisor(++i, fl, tl);
	else
		return booleanUnit(i, tl, fl);
}


bool Parser::booleanUnit(size_t &i, 
	unsigned &tl, unsigned &fl) {

	if (i + 2 < Tokens.size() &&
		isEqual(Tokens[i], IDENTIFIER) &&
		isRelation(Tokens[i + 1].type_index)
		&& isEqual(Tokens[i + 2], IDENTIFIER)) {
		
		if (isIntType(storage->getType(
			Tokens[i].val_index)) &&
			storage->isSameType(Tokens[i].val_index, 
				Tokens[i + 2].val_index)) {

			tl = IntermediateCode.size();
			gen(std::string("j") +
				Data::getValue(Tokens[i + 1].type_index),
				Tokens[i].val_index,
				Tokens[i + 2].val_index, EMPTY);

			fl = IntermediateCode.size();
			gen("j", EMPTY, EMPTY, EMPTY);

			i += 3;
			return true;
		}
		else
			throw type_error(getRow(i), 
				storage->getName(
					Tokens[i].val_index));
	}

	else if (i < Tokens.size()) {
		
		unsigned tmp = IntermediateCode.size();

		int ide1, ide2;
		if (arithmeticExpression(i, ide1)) {
			unsigned mark = i;
			if (i < Tokens.size() &&
				isRelation(Tokens[i].
					type_index))
				if (arithmeticExpression(++i, ide2)) {
					tl = IntermediateCode.size();
					gen("j" + Data::getValue(
						Tokens[mark].
						type_index), ide1, ide2,
						EMPTY);
					fl = IntermediateCode.size();
					gen("j", EMPTY, EMPTY, EMPTY);
					return true;
				}
		}

		while (tmp < IntermediateCode.size())
			IntermediateCode.pop_back();

		if (isEqual(Tokens[i], IDENTIFIER)) {

			if (!isBoolType(storage->getType(
				Tokens[i].val_index)) && !isIntType(
					storage->getType(Tokens[i].val_index)))
				throw type_error(i, storage->getName(Tokens[i].val_index));

			else {
				tl = IntermediateCode.size();
				gen("jnz", Tokens[i].val_index, EMPTY, EMPTY);
				fl = IntermediateCode.size();
				gen("j", EMPTY, EMPTY, EMPTY);
				++i;
				return true;
			}
		}

		else if (isEqual(Tokens[i], "(")) {

			unsigned tmp = i + 1;
			unsigned mark = IntermediateCode.size();
			if (booleanExpression(++i, tl, fl))
				if (isEqual(Tokens[i], ")")) {
					++i;
					return true;
				}
				else
					return false;
			else {
				i = tmp;
				while (IntermediateCode.size() > mark)
					IntermediateCode.pop_back();
			}
		}

		else if (isBoolean(Tokens[i].type_index)) {
			tl = IntermediateCode.size();
			gen("jnz", Tokens[i].type_index, EMPTY, EMPTY);
			fl = IntermediateCode.size();
			gen("j", EMPTY, EMPTY, EMPTY);
			++i;
			return true;
		}

	}

	return false;
}


void Parser::parse(std::ifstream &is) {

	std::string str;
	Scanner scanner(storage);
	unsigned row = 0;
	while (std::getline(is, str)) {
		size_t i = 0;
		while (i < str.length()) {
			token token = scanner.scan(str, i, row);
			if (token.val_index != VALUE_NONE)
				Tokens.push_back(token);
		}
		rowMark.push_back(Tokens.size());
		++row;
	}

	size_t i = 0;
	if (isProgram(i) && program(i))
		return;
	else
		throw syntax_error(i);
}


void Parser::printIntermediateCode(std::ostream &out) {

	unsigned i = 0;
	for (quaternary q : IntermediateCode) {

		out << "(" << i << ")" << "\t(" << q.ope << ", ";

		if (q.ide_l == EMPTY)
			out << "-, ";
		else if (q.ope == "jnz") {
			if (isBoolean(q.ide_l))
				out << Data::getValue(q.ide_l) << ", ";
			else
				out << storage->getName(q.ide_l) << ", ";
		}
		else
			out << storage->getName(q.ide_l) << ", ";

		if (q.ide_r == EMPTY)
			out << "-, ";
		else
			out << storage->getName(q.ide_r) << ", ";

		if (q.ope[0] == 'j')
			out << q.ide;
		else if (q.ide == EMPTY)
			out << "-";
		else
			out << storage->getName(q.ide);

		out << ")\n";

		++i;
	}
}

