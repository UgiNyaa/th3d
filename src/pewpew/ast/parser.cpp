#include <iostream>
#include <cstdlib>

#include "parser.hpp"

int Parser::parse(ASTNodeProg* root)
{
	int error = 0;

    while(offset < ts.size())
    {
        if (ts[offset].type == "COMMENT")
        {
            offset++;
            continue;
        }
        
        if (ts[offset].type == "VARIABLE")
        {
			if (ts[offset + 1].value == "=")
			{
				auto node = new ASTNodeAssign();
				error = parse_assign(node);
				if (error != 0)
					return error;
				root->prog.push_back(node);
			}
			else
				return UNFULFILED_EXPECTATION;

			offset++;
        }
    }

	return error;
}

int Parser::parse_assign(ASTNodeAssign* node)
{
	int error = 0;

    if (ts[offset].type != "VARIABLE")
		return UNFULFILED_EXPECTATION;

	{
		auto n = new ASTNodeVar();
		error = parse_var(n);
		if (error != 0)
			return error;
		node->left = n;
	}

    offset++;

    if (ts[offset].value != "=")
		return UNFULFILED_EXPECTATION;
	node->o = ts[offset].value;
    
    offset++;

	if (ts.size() > offset + 1 && ts[offset + 1].value == "=")
	{
		auto n = new ASTNodeAssign();
		error = parse_assign(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts.size() > offset + 1 && ts[offset + 1].type == "OPERATOR")
	{
		auto n = new ASTNodeBinary();
		error = parse_binary(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].value == "(")
	{
		auto n = new ASTNodeBinary();
		error = parse_binary(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].type == "VARIABLE")
	{
		auto n = new ASTNodeVar();
		error = parse_var(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].type == "NUMBER")
	{
		auto n = new ASTNodeNumber();
		error = parse_number(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].type == "STRING")
	{
		auto n = new ASTNodeString();
		error = parse_string(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].value == "true" || ts[offset].value == "false")
	{
		auto n = new ASTNodeBool();
		error = parse_bool(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].value == "lambda")
	{
		auto n = new ASTNodeLambda();
		error = parse_lambda(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else
		return UNFULFILED_EXPECTATION;
    
    return error;
}

int Parser::parse_binary(ASTNodeBinary* node)
{
	int error;

	// left value calculation
	if (ts[offset].value == "(")
	{
		offset++;
		auto n = new ASTNodeBinary();
		error = parse_binary(n);
		if (error != 0)
			return error;
		node->left = n;
	}
	else if (ts[offset].type == "VARIABLE")
	{
		auto n = new ASTNodeVar();
		error = parse_var(n);
		if (error != 0)
			return error;
		node->left = n;
	}
	else if (ts[offset].type == "NUMBER")
	{
		auto n = new ASTNodeNumber();
		error = parse_number(n);
		if (error != 0)
			return error;
		node->left = n;
	}
	else if (ts[offset].type == "STRING")
	{
		auto n = new ASTNodeString();
		error = parse_string(n);
		if (error != 0)
			return error;
		node->left = n;
	}
	else if (ts[offset].value == "true" || ts[offset].value == "false")
	{
		auto n = new ASTNodeBool();
		error = parse_bool(n);
		if (error != 0)
			return error;
		node->left = n;
	}
	else
		return UNFULFILED_EXPECTATION;

	offset++;

	// SPECIAL CASE:
	// if the parenthesis imediatly closes
	if (ts[offset].value == ")")
	{
		// set right to null
		node->right = nullptr;

		return error;
	}

	// operator calculation
	if (ts[offset].type != "OPERATOR")
		return UNFULFILED_EXPECTATION;
	node->o = ts[offset].value;

	offset++;

	// right value calculation
	if (ts[offset].value == "(")
	{
		offset++;
		auto n = new ASTNodeBinary();
		error = parse_binary(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts.size() > offset + 1 && ts[offset + 1].value == "=")
	{
		auto n = new ASTNodeAssign();
		error = parse_assign(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts.size() > offset + 1 && ts[offset + 1].type == "OPERATOR")
	{
		auto n = new ASTNodeBinary();
		error = parse_binary(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].type == "VARIABLE")
	{
		auto n = new ASTNodeVar();
		error = parse_var(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].type == "NUMBER")
	{
		auto n = new ASTNodeNumber();
		error = parse_number(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].type == "STRING")
	{
		auto n = new ASTNodeString();
		error = parse_string(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].value == "true" || ts[offset].value == "false")
	{
		auto n = new ASTNodeBool();
		error = parse_bool(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else if (ts[offset].value == "lambda")
	{
		auto n = new ASTNodeLambda();
		error = parse_lambda(n);
		if (error != 0)
			return error;
		node->right = n;
	}
	else
		return UNFULFILED_EXPECTATION;

	if (ts.size() > offset + 1 && ts[offset + 1].value == ")")
		offset++;

	return error;
}

int Parser::parse_var(ASTNodeVar* node)
{
	if (ts[offset].type != "VARIABLE")
		return UNFULFILED_EXPECTATION;
    
    node->name = ts[offset].value;
    return 0;
}

int Parser::parse_number(ASTNodeNumber* node)
{
	if (ts[offset].type != "NUMBER")
		return UNFULFILED_EXPECTATION;
    
    node->value = strtof(ts[offset].value.c_str(), nullptr);
    return 0;
}

int Parser::parse_string(ASTNodeString* node)
{
    if (ts[offset].type != "STRING")
		return UNFULFILED_EXPECTATION;
    
    node->value = ts[offset].value.substr(1, ts[offset].value.size() - 2);
    return 0;
}

int Parser::parse_bool(ASTNodeBool* node)
{
    if (ts[offset].value != "true" && ts[offset].value != "false")
		return UNFULFILED_EXPECTATION;
    
    node->value = ts[offset].value == "true";
    return 0;
}

int Parser::parse_return(ASTNodeReturn* node)
{
	int error = 0;

	if (ts[offset].value != "return")
		return UNFULFILED_EXPECTATION;

	do
	{
		offset++;
		if (ts.size() > offset + 1 && ts[offset + 1].type == "OPERATOR")
		{
			auto n = new ASTNodeBinary();
			error = parse_binary(n);
			if (error != 0)
				return error;
			node->nodes.push_back(n);
		}
		else if (ts[offset].type == "VARIABLE")
		{
			auto n = new ASTNodeVar();
			error = parse_var(n);
			if (error != 0)
				return error;
			node->nodes.push_back(n);
		}
		else if (ts[offset].type == "NUMBER")
		{
			auto n = new ASTNodeNumber();
			error = parse_number(n);
			if (error != 0)
				return error;
			node->nodes.push_back(n);
		}
		else if (ts[offset].type == "STRING")
		{
			auto n = new ASTNodeString();
			error = parse_string(n);
			if (error != 0)
				return error;
			node->nodes.push_back(n);
		}
		else if (ts[offset].value == "true" || ts[offset].value == "false")
		{
			auto n = new ASTNodeBool();
			error = parse_bool(n);
			if (error != 0)
				return error;
			node->nodes.push_back(n);
		}
		else
			return UNFULFILED_EXPECTATION;

		offset++;
	} while (ts[offset].value == ",");

	return error;
}

int Parser::parse_lambda(ASTNodeLambda* node)
{
	int error = 0;

    if (ts[offset].value != "lambda")
		return UNFULFILED_EXPECTATION;

    offset++;

    if (ts[offset].value != "(")
		return UNFULFILED_EXPECTATION;

	offset++;
    
    while (ts[offset].type == "VARIABLE")
    {
        node->ins.push_back(ts[offset].value);
        offset++;

		if (ts[offset].value == ",") offset++;
		else if (ts[offset].value == ")") break;
		else
			return UNFULFILED_EXPECTATION;
    }

    if (ts[offset].value != ")")
		return UNFULFILED_EXPECTATION;

    offset++;

	if (ts[offset].value == "->")
	{
		offset++;

		if (ts[offset].value != "(")
			return UNFULFILED_EXPECTATION;

		offset++;

		while (ts[offset].type == "VARIABLE")
		{
			node->outs.push_back(ts[offset].value);
			offset++;

			if (ts[offset].value == ",") offset++;
			else if (ts[offset].value == ")") break;
			else
				return UNFULFILED_EXPECTATION;
		}

		if (ts[offset].value != ")")
			return UNFULFILED_EXPECTATION;

		offset++;
	}

	if (ts.size() > offset + 1 && ts[offset + 1].type == "OPERATOR")
	{
		auto n = new ASTNodeBinary();
		error = parse_binary(n);
		if (error != 0)
			return error;
		node->body = n;
	}
	else if (ts[offset].type == "VARIABLE")
	{
		auto n = new ASTNodeVar();
		error = parse_var(n);
		if (error != 0)
			return error;
		node->body = n;
	}
	else if (ts[offset].type == "NUMBER")
	{
		auto n = new ASTNodeNumber();
		error = parse_number(n);
		if (error != 0)
			return error;
		node->body = n;
	}
	else if (ts[offset].type == "STRING")
	{
		auto n = new ASTNodeString();
		error = parse_string(n);
		if (error != 0)
			return error;
		node->body = n;
	}
	else if (ts[offset].value == "true" || ts[offset].value == "false")
	{
		auto n = new ASTNodeBool();
		error = parse_bool(n);
		if (error != 0)
			return error;
		node->body = n;
	}
	else if (ts[offset].value == "{")
	{
		auto n = new ASTNodeProg();
		error = parse_prog(n);
		if (error != 0)
			return error;
		node->body = n;
	}
	else
		return UNFULFILED_EXPECTATION;
    
    return error;
}

int Parser::parse_prog(ASTNodeProg* node)
{
	int error;

    if (ts[offset].value != "{")
		return UNFULFILED_EXPECTATION;

    offset++;

    while(ts[offset].value != "}")
    {
        if (ts[offset].type == "COMMENT")
        {
            offset++;
            continue;
        }

		if (ts.size() > offset + 1 && ts[offset + 1].type == "OPERATOR")
		{
			if (ts[offset].type == "VARIABLE" && ts[offset + 1].value == "=")
			{
				auto n = new ASTNodeAssign();
				error = parse_assign(n);
				if (error != 0)
					return error;
				node->prog.push_back(n);
			}
			else
			{
				auto n = new ASTNodeBinary();
				error = parse_binary(n);
				if (error != 0)
					return error;
				node->prog.push_back(n);
			}

			offset++;
		}
		else if (ts[offset].type == "VARIABLE")
		{
			auto n = new ASTNodeCall();
			error = parse_call(n);
			if (error != 0)
				return error;
			node->prog.push_back(n);
		}
		else if (ts[offset].value == "return")
		{
			auto n = new ASTNodeReturn();
			error = parse_return(n);
			if (error != 0)
				return error;
			node->prog.push_back(n);
		}
		else
			return UNFULFILED_EXPECTATION;
    }

    return error;
}

int Parser::parse_call(ASTNodeCall* node)
{
	int error = 0;

	if (ts[offset].type != "VARIABLE")
		return UNFULFILED_EXPECTATION;

	offset++;

	if (ts[offset].value != "(")
		return UNFULFILED_EXPECTATION;

	offset++;

	while (ts[offset].value != ")")
	{
		ASTNode* arg;
		if (ts.size() > offset + 1 && ts[offset + 1].type == "OPERATOR")
		{
			auto n = new ASTNodeBinary();
			error = parse_binary(n);
			if (error != 0)
				return error;
			arg = n;
		}
		else if (ts[offset].type == "VARIABLE")
		{
			auto n = new ASTNodeVar();
			error = parse_var(n);
			if (error != 0)
				return error;
			arg = n;
		}
		else if (ts[offset].type == "NUMBER")
		{
			auto n = new ASTNodeNumber();
			error = parse_number(n);
			if (error != 0)
				return error;
			arg = n;
		}
		else if (ts[offset].type == "STRING")
		{
			auto n = new ASTNodeString();
			error = parse_string(n);
			if (error != 0)
				return error;
			arg = n;
		}
		else if (ts[offset].value == "true" || ts[offset].value == "false")
		{
			auto n = new ASTNodeBool();
			error = parse_bool(n);
			if (error != 0)
				return error;
			arg = n;
		}
		else
			return UNFULFILED_EXPECTATION;

		node->args.push_back(arg);

		offset++;

		if (ts[offset].value != ")" && ts[offset].value != ",")
			return UNFULFILED_EXPECTATION;
	}

	if (ts[offset].value != ")")
		return UNFULFILED_EXPECTATION;

	offset++;

	return error;
}

bool Parser::validate_expression(int& range, int& max_level)
{
	range = 0;
	max_level = 0;
	int curr_level = 0;
	std::string pre = "";

	auto is_vnsb = [](Token t) -> bool
	{
		return t.type == "VARIABLE" ||
			t.type == "NUMBER" ||
			t.type == "STRING" ||
			t.value == "true" ||
			t.value == "false";
	};

	while (true)
	{
		if (ts.size() < offset + range)
			break;

		auto t = ts[offset + range];

		if (t.value == "(")
		{
			if (max_level == curr_level)
				max_level++;
			curr_level++;
		}

		if (t.value == ")")
			curr_level--;

		if (range > 0)
		{
			// token last
			auto tl = ts[offset + range - 1];

			// if the current value is a "("
			// and if the last value was no operator
			if (t.value == "(" && tl.type != "OPERATOR")
			{
				// then the only two excuse shall be another "("
				// or a variable, which would indicate a function call
				// if it does not meet this requirement either
				if (tl.value != "(" && tl.type != "VARIABLE")
					// then it ends here with error
					return false;
			}
		}

		if (ts.size() > offset + range + 1)
		{
			// token next
			auto tn = ts[offset + range + 1];

			// if the current token is a vnsb
			// and if the next token is no parenthesis or operator
			if (is_vnsb(t) && is_vnsb(tn))
				// then it ends there
				break;

			// if there are parenthesis without content
			if (t.value == "(" && tn.value == ")")
				// then it ends there with error
				return false;

			// if there are two operators next to each other
			if (t.type == "OPERATOR" && tn.type == "OPERATOR")
				// then it ends here with error
				return false;
		}

		range++;
	}

	if (ts[offset + range].type == "OPERATOR")
		return false;

	if (curr_level != 0)
		return false;

	return range;
}

int Parser::count_value(std::string v)
{
	int c = 0;
	while (ts[offset + c].value == v) c++;
	return c;
}