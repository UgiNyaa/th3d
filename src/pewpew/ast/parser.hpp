#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast_node.hpp"
#include "lexer.hpp"

#define UNFULFILED_EXPECTATION 1

class Parser
{
private:
    std::vector<Token> ts;
    size_t offset = 0;
    
    int parse_assign(ASTNodeAssign* node);
	int parse_binary(ASTNodeBinary* node);
    int parse_var(ASTNodeVar* node);
    int parse_number(ASTNodeNumber* node);
    int parse_string(ASTNodeString* node);
    int parse_bool(ASTNodeBool* node);
    int parse_lambda(ASTNodeLambda* node);
	int parse_prog(ASTNodeProg* node);
	int parse_call(ASTNodeCall* node);
	int parse_return(ASTNodeReturn* node);

	bool validate_expression(int& range, int& max_level);
	int count_value(std::string v);

public:
    Parser(std::vector<Token> ts) : ts(ts) { }
    int parse(ASTNodeProg* root);
};

#endif
