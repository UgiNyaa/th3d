#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

struct Token 
{
	std::string type;
	std::string value;
};

struct Rule
{
	std::string type;
	std::string pattern;
};

class Lexer
{
private:
	std::vector<Rule> rules;

public:
	Lexer();

	int get_from_string(const std::string &contentToProcess, std::vector<Token>& tokens);
	int get_from_file(const std::string& filename, std::vector<Token>& tokens);
};

#endif /* LEXER_HPP */