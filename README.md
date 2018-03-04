## 编译器
编译原理课程实验，根据提供的SAMPLE语言定义，实现对应的词法分析器，语法、语义分析器，并生成中间代码结果（四元式）。
* `Data.h`：Data类存放语言的单词编码表，单词集等信息。
* `Storage.h`：Storage类存放编译过程中生成的符号表。
* `Scanner.h`：Scanner类负责词法分析。
* `Parser.h`：Parser类负责语法、语义分析，并生成四元式串。
### SAMPLE语言定义
* <字符集> → <字母>│<数字>│<单界符>
* <字母> → A│B│…│Z│a│b│…│z
* <数字> → 0│1│2│…│9
* <单界符> → +│-│*│/│=│<│>│(│)│[│]│:│. │; │, │'
* <单词集> → <保留字>│<双界符>│<标识符>│<常数>│<单界符>
* <保留字> → and│array│begin│bool│call│case│char│constant│dim│do│else│end│false│for│if│input│integer│not│of│or│output│procedure│program│read│real│repeat│set│stop│then│to│true│until│var│while│write
* <双界符> → <>│<=│>=│:= │/*│*/│..
* <标识符> → <字母>│<标识符> <数字>│<标识符> <字母>
* <常数> → <整数>│<布尔常数>│<字符常数>
* <整数> → <数字>│<整数> <数字>
* <布尔常数> → true│false
* <字符常数> → ' 除 {'} 外的任意字符串 '
* <类型> → integer│bool│char
* <表达式> → <算术表达式>│<布尔表达式>│<字符表达式>
* <算术表达式> → <算术表达式> + <项>│<算术表达式> - <项>│<项>
* <项> → <项> * <因子>│<项> / <因子>│<因子>
* <因子> → <算术量>│- <因子>
* <算术量> → <整数>│<标识符>│（ <算术表达式> ）
* <布尔表达式> → <布尔表达式> or <布尔项>│<布尔项>
* <布尔项> → <布尔项> and <布因子>│<布因子>
* <布因子> → <布尔量>│not <布因子>
* <布尔量> → <布尔常量>│<标识符>│（ <布尔表达式> ）│ <标识符> <关系符> <标识符>│<算术表达式> <关系符> <算术表达式>
* <关系符> → <│<>│<=│>=│>│=
* <字符表达式> → <字符常数>│<标识符>
* <语句> → <赋值句>│<if句>│<while句>│<repeat句>│<复合句>
* <赋值句> → <标识符> := <算术表达式>
* <if句>→ if <布尔表达式> then <语句>│if <布尔表达式> then <语句> else <语句>
* <while句> → while <布尔表达式> do <语句>
* <repeat句> → repeat <语句> until <布尔表达式>
* <复合句> → begin <语句表> end
* <语句表> → <语句> ；<语句表>│<语句>
* <程序> → program <标识符> ；<变量说明> <复合语句> .
* <变量说明> → var <变量定义>│ε
* <变量定义> → <标识符表> ：<类型> ；<变量定义>│<标识符表> ：<类型> ；
* <标识符表> → <标识符> ，<标识符表>│<标识符>

### 改写后的语言定义
* \<scan> → \<string>|\<integer>|\<identifier>|\<delimiter>
* \<scan> → \<string>|\<integer>|\<identifier>|\<delimiter>
* \<string> → '除 ' 外的任意字符'
* \<integer> → \<digital>\<integer>
* \<identifier> → \<reserveWord>|\<identifier>
* \<delimiter> → \<anotation>|\<doubleCharDelimiter>|\<singleCharDelimiter>
* \<program> → program \<identifier> ; \<variableDefinition>\<compoundStatement>.
* \<variableDefinition> → var \<identifierTable>
* \<identifierTable> → \<identifier> , \<identifierTable>|\<identifier> : \<type>;
* \<compoundStatement> → begin \<statement List> end
* \<statementList> → \<statement> ; \<statement List>|\<statement>
* \<statement> → \<assignmentStatement>|\<ifStatement>|\<whileStatement>|\<repeatStatement>
* \<assignmentStatement> → \<identifier> := \<arithmeExpression>
* \<arithmeticExpression> → \<item> + \<arithmeticExpression>|\<item> - \<arithmeticExpression>|\<item>
* \<item> → \<divisor> * \<item>|\<divisor> / \<item>|\<divisor>
* \<divisor> → -\<divisor>|\<arithmeticUnit>
* \<arithmeticUnit> → \<identifier>|\<integer>|( \<arithmeticExpression> )
* \<ifStatement> → if \<booleanExpression> then \<statement> else \<statement>
* \<whileStatement> → while \<booleanExpression> do \<statement>
* \<repeatStatement> → repeat \<statement> until \<booleanExpression>
* \<booleanExpression> → \<booleanItem> or \<booleanExpression>
* \<booleanItem> → \<booleanDivisor> and \<booleanItem>
* \<booleanDivisor> → \<identifier>\<relation>\<identifier>|\<arithmeticExpression>\<relation>\<arithmeticExpression>|\<identifier>|( \<booleanExpression> )|\<boolean>

## Compiler
Scanner and Parser implementations for language SAMPLE.
* `Data.h`: Store the token code table and name sets of tokens.
* `Storage.h`: Store the symbol table which is built during compiling.
* `Scanner.h`：Lexical analysis.
* `Parser.h`：Syntactic analysis and semantic analysis. Generate quaternaries as a intermediatecode result.
