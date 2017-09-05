#include <regex>
#include <fstream>
#include <iostream>

#include "lexer.hpp"

Lexer::Lexer()
	: rules{
		{ "COMMENT", "^(//).*" },
		{ "COMMENT", "^(#).*" },
		{ "COMMENT", "^\\/\\*([\\s\\S]*)\\*\\/" },
		{ "KEYWORD", "^(lambda)|^(if)|^(then)|^(else)|^(true)|^(false)|^(return)" },
		{ "STRING", "^\"(.*?)\"" },
		{ "VARIABLE", "^[a-zA-Z_][a-zA-Z_0-9]*" },
		{ "OPERATOR", "^(\\+)|^(-[^>])|^(/)|^(\\*)|^(%)|^(==)|^(<=)|^(>=)|^(<)|^(>)|^(=)" },
		{ "NUMBER", "^[-\\+]?[0-9]+\\.?[0-9]*" },
		{ "PUNCTUATION", "^(\\()|^(\\))|^(\\{)|^(\\})|^(;)|^(:)|^(,)|^(->)|^(.)|^(..)|^(...)" }
	}
{
}

int Lexer::get_from_string(const std::string & contentToProcess, std::vector<Token>& tokens)
{
	std::string data = contentToProcess;

	// replacing all newlines with spaces
	for (size_t i = 0; i < data.size(); i++)
		if (data[i] == '\n' || data[i] == '\t' || data[i] == '\r')
			data[i] = ' ';

	auto strIt = data.begin();

	for (int i = 0; strIt != data.end() && i < rules.size(); i++)
	{
		// when there is whitespace
		// restart loop
		if (*strIt == ' ')
		{
			strIt++;
			i = -1;
			continue;
		}

		std::regex e(rules[i].pattern);
		std::smatch m;
		std::string s(strIt, data.end());

		// when there is a match
		// check where it is
		if (std::regex_search(s, m, e))
		{
			strIt += m[0].str().size();

			// everything successfull
			// add the token
			Token token;
			token.type = rules[i].type;
			token.value = std::string(m[0].first, m[0].second);
			tokens.push_back(token);

			// restart loop
			i = -1;
		}

		// if no rule applied, end this
		if (i == rules.size() - 1)
			strIt = data.end();
	}

	return 0;
}

int Lexer::get_from_file(const std::string & filename, std::vector<Token>& tokens)
{
	std::ifstream file(filename);

	if (file.is_open())
	{
		std::string fileContents;

		file.seekg(0, std::ios::end);
		fileContents.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		fileContents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		file.close();

		return get_from_string(fileContents, tokens);
	}
	else
	{
		return -1;
	}
}
