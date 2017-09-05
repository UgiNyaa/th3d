#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <vector>
#include <string>

enum struct ASTNodeType
{
	NONE,
    NUMBER,
    STRING,
    BOOL,
    VAR,
    LAMBDA,
    CALL,
    IF,
    ASSIGN,
    BINARY,
    PROG,
	RETURN
};

struct ASTNode
{
	const ASTNodeType type;
	ASTNode(ASTNodeType type) : type(type) { }
	virtual ~ASTNode() { }
};

struct ASTNodeNumber : public ASTNode
{
    float value;
	ASTNodeNumber() : ASTNode(ASTNodeType::NUMBER) { }
};

struct ASTNodeString : public ASTNode
{
    std::string value;
	ASTNodeString() : ASTNode(ASTNodeType::STRING) { }
};

struct ASTNodeBool : public ASTNode
{
    bool value;
	ASTNodeBool() : ASTNode(ASTNodeType::BOOL) { }
};

struct ASTNodeVar : public ASTNode
{
    std::string name;
	ASTNodeVar() : ASTNode(ASTNodeType::VAR) { }
};

struct ASTNodeLambda : public ASTNode
{
    std::vector<std::string> ins;
	std::vector<std::string> outs;
    ASTNode* body;
	ASTNodeLambda() : ASTNode(ASTNodeType::LAMBDA) { }
	~ASTNodeLambda() { delete body; }
};

struct ASTNodeCall : public ASTNode
{
    ASTNode* func;
    std::vector<ASTNode*> args;
	ASTNodeCall() : ASTNode(ASTNodeType::CALL) { }
	~ASTNodeCall() { for (auto& arg : args) delete arg; }
};

struct ASTNodeIf : public ASTNode
{
    ASTNode* cond;
    ASTNode* then;
    ASTNode* els;
	ASTNodeIf() : ASTNode(ASTNodeType::IF) { }
	~ASTNodeIf() { delete cond, then, els; }
};

struct ASTNodeAssign : public ASTNode
{
    std::string o = "=";
    ASTNodeVar* left;
    ASTNode* right;
	ASTNodeAssign() : ASTNode(ASTNodeType::ASSIGN) { }
	~ASTNodeAssign() { delete left, right; }
};

struct ASTNodeBinary : public ASTNode
{
    std::string o;
    ASTNode* left;
    ASTNode* right;
	ASTNodeBinary() : ASTNode(ASTNodeType::BINARY) { }
	~ASTNodeBinary() { delete left, right; }
};

struct ASTNodeProg : public ASTNode
{
    std::vector<ASTNode*> prog;
	ASTNodeProg() : ASTNode(ASTNodeType::PROG) { }
	~ASTNodeProg() { for (auto& p : prog) delete p; }
};

struct ASTNodeReturn : public ASTNode
{
	std::vector<ASTNode*> nodes;
	ASTNodeReturn() : ASTNode(ASTNodeType::RETURN) { }
	~ASTNodeReturn() { for (auto& n : nodes) delete n; }
};

#endif