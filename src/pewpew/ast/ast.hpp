#ifndef AST_HPP
#define AST_HPP

#include "ast_node.hpp"
#include "lexer.hpp"
#include "parser.hpp"

class AST
{
private:
	ASTNodeProg* r = nullptr;

public:
	~AST() { delete r; }

	int get_from_string(std::string data);
	int get_from_file(std::string filename);

	ASTNodeProg& root() const;
};

#endif /* AST_HPP */