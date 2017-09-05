#include "ast.hpp"

int AST::get_from_string(std::string data)
{
	if (r != nullptr)
		return 1;

	std::vector<Token> tokens;
	Lexer lexer;
	lexer.get_from_string(data, tokens);

	r = new ASTNodeProg();
	Parser parser(tokens);
	int error = parser.parse(r);
	if (error != 0)
		return error;

	return 0;
}

int AST::get_from_file(std::string filename)
{
	if (r != nullptr)
		return 1;

	Lexer lexer;
	std::vector<Token> tokens;
	lexer.get_from_file(filename, tokens);

	r = new ASTNodeProg();
	Parser parser(tokens);
	int error = parser.parse(r);
	if (error != 0)
		return error;

	return 0;
}

ASTNodeProg & AST::root() const
{
	return *r;
}
