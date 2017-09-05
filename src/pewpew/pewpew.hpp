#ifndef PEWPEW_HPP
#define PEWPEW_HPP

#include <string>
#include <vector>
#include <functional>
#include <map>

#include "ast\ast.hpp"
#include "asmjit\asmjit.h"
#include "compile.hpp"

using namespace asmjit;
using namespace asmjit::x86;

class pewpew
{
public:
	pewpew(std::string source)
	{
		std::vector<Token> tokens;
		Lexer lexer;
		lexer.get_from_string(source, tokens);

		Parser parser(tokens);
		parser.parse(&ast);
	}

	JitRuntime rt;
	ASTNodeProg ast;
};

template<class ... Outs>
struct get {
	template<class ... Ins>
	struct var {
		static auto call(pewpew& pp, const std::string& name) 
			-> void(*)(Ins..., Outs...)
		{
			ASTNodeAssign* target;
			for (size_t i = 0; i < pp.ast.prog.size(); i++)
			{
				target = static_cast<ASTNodeAssign*>(pp.ast.prog[i]);
				auto var = target->left;

				if (var->name != name)
					target = nullptr;
				else
					break;
			}

			if (target == nullptr)
				throw 0;

			void(*fn)(Ins..., Outs...);

			if (target->right->type == ASTNodeType::LAMBDA)
				fn = compile<Outs...>::lambda<Ins...>::call(static_cast<ASTNodeLambda*>(target->right), pp.rt);
			else
				throw 0;

			return fn;
		}
	};
};

//template<class ... ARGS>
//auto pewpew::get_func(std::string name) -> void(*)(ARGS...)
//{
//	ASTNodeAssign* target;
//	for (size_t i = 0; i < ast.prog.size(); i++)
//	{
//		target = static_cast<ASTNodeAssign*>(ast.prog[i]);
//		auto var = target->left;
//
//		if (var->name != name)
//			target = nullptr;
//		else
//			break;
//	}
//
//	if (target == nullptr)
//		throw 0;
//
//	void(*fn)(ARGS...);
//
//	if (target->right->type == ASTNodeType::LAMBDA)
//		fn = compile_lambda<ARGS...>::call(static_cast<ASTNodeLambda*>(target->right), rt);
//	else
//		throw 0;
//
//	return fn;
//}

//template<>
//std::string pewpew::get(std::string name)
//{
//	for (size_t i = 0; i < ast.prog.size(); i++)
//	{
//		auto assign = static_cast<ASTNodeAssign*>(ast.prog[i]);
//		auto var = assign->left;
//
//		if (var->name != name)
//			continue;
//
//		if (assign->right->type == ASTNodeType::STRING)
//		{
//			return static_cast<ASTNodeString*>(assign->right)->value;
//		}
//		else if (assign->right->type == ASTNodeType::BINARY)
//		{
//			std::function<std::string(ASTNodeBinary*)> binary_to_string;
//			binary_to_string = [&binary_to_string](ASTNodeBinary* n) -> std::string
//			{
//				std::string str("");
//				if (n->left->type == ASTNodeType::STRING)
//					str += static_cast<ASTNodeString*>(n->left)->value;
//				else if (n->left->type == ASTNodeType::BINARY)
//					str += binary_to_string(static_cast<ASTNodeBinary*>(n->left));
//				else
//					throw 0;
//
//				if (n->right->type == ASTNodeType::STRING)
//				{
//					auto s = static_cast<ASTNodeString*>(n->right)->value;
//					if (n->o == "+")
//						str += s;
//					else
//						throw 0;
//				}
//				else if (n->right->type == ASTNodeType::BINARY)
//				{
//					auto s = binary_to_string(static_cast<ASTNodeBinary*>(n->right));
//					if (n->o == "+")
//						str += s;
//					else
//						throw 0;
//				}
//				else
//					throw 0;
//
//				return str;
//			};
//
//			return binary_to_string(static_cast<ASTNodeBinary*>(assign->right));
//		}
//		else
//			throw 0;
//	}
//
//	throw 0;
//}
//

//template<>
//float pewpew::get(std::string name)
//{
//	for (size_t i = 0; i < ast.prog.size(); i++)
//	{
//		auto assign = static_cast<ASTNodeAssign*>(ast.prog[i]);
//		auto var = assign->left;
//
//		if (var->name != name)
//			continue;
//
//		if (assign->right->type == ASTNodeType::NUMBER)
//		{
//			return static_cast<ASTNodeNumber*>(assign->right)->value;
//		}
//		else if (assign->right->type == ASTNodeType::BINARY)
//		{
//			std::function<float(ASTNodeBinary*)> binary_to_float;
//			binary_to_float = [&binary_to_float](ASTNodeBinary* n) -> float
//			{
//				float f = 0;
//
//				//left
//				if (n->left->type == ASTNodeType::NUMBER)
//					f += static_cast<ASTNodeNumber*>(n->left)->value;
//				else if (n->left->type == ASTNodeType::BINARY)
//					f += binary_to_float(static_cast<ASTNodeBinary*>(n->left));
//				else
//					throw 0;
//
//				// right
//				if (n->right->type == ASTNodeType::NUMBER)
//				{
//					auto num = static_cast<ASTNodeNumber*>(n->right)->value;
//					if (n->o == "+")
//						f += num;
//					else if (n->o == "-")
//						f -= num;
//					else if (n->o == "*")
//						f *= num;
//					else if (n->o == "/")
//						f /= num;
//				}
//				else if (n->right->type == ASTNodeType::BINARY)
//				{
//					auto num = binary_to_float(static_cast<ASTNodeBinary*>(n->right));
//					if (n->o == "+")
//						f += num;
//					else if (n->o == "-")
//						f -= num;
//					else if (n->o == "*")
//						f *= num;
//					else if (n->o == "/")
//						f /= num;
//				}
//
//				return f;
//			};
//
//			return binary_to_float(static_cast<ASTNodeBinary*>(assign->right));
//		}
//	}
//}
//

//template<>
//auto pewpew::get_func(std::string name) -> void(*)(float*)
//{
//	ASTNodeAssign* target;
//	for (size_t i = 0; i < ast.prog.size(); i++)
//	{
//		target = static_cast<ASTNodeAssign*>(ast.prog[i]);
//		auto var = target->left;
//
//		if (var->name != name)
//			target = nullptr;
//		else
//			break;
//	}
//
//	if (target == nullptr)
//		throw 0;
//	
//	if (target->right->type != ASTNodeType::LAMBDA)
//		throw 0;
//
//	auto lambda = static_cast<ASTNodeLambda*>(target->right);
//
//	if (lambda->body->type != ASTNodeType::PROG)
//		throw 0;
//
//	auto prog = static_cast<ASTNodeProg*>(lambda->body);
//
//	CodeHolder code;
//	code.init(rt.getCodeInfo());
//
//	X86Assembler a(&code);
//
//	compile_prog(*prog, a, 0, 1);
//	//float f = 10;
//	//a.mov(out0, *reinterpret_cast<int*>(&f));
//	//a.ret();
//
//	void (*fn)(float*);
//	auto err = rt.add(&fn, &code);
//	if (err) throw 0;
//
//	return fn;
//}
//

//template<>
//auto pewpew::get_func(std::string name) -> void(*)(float*,float*)
//{
//	ASTNodeAssign* target;
//	for (size_t i = 0; i < ast.prog.size(); i++)
//	{
//		target = static_cast<ASTNodeAssign*>(ast.prog[i]);
//		auto var = target->left;
//
//		if (var->name != name)
//			target = nullptr;
//		else
//			break;
//	}
//
//	if (target == nullptr)
//		throw 0;
//
//	if (target->right->type != ASTNodeType::LAMBDA)
//		throw 0;
//
//	auto lambda = static_cast<ASTNodeLambda*>(target->right);
//
//	if (lambda->body->type != ASTNodeType::PROG)
//		throw 0;
//
//	auto prog = static_cast<ASTNodeProg*>(lambda->body);
//
//	CodeHolder code;
//	code.init(rt.getCodeInfo());
//
//	X86Assembler a(&code);
//
//	//X86Mem out0;
//	//X86Mem out1;
//	//if (ASMJIT_ARCH_64BIT)
//	//{
//	//	if (ASMJIT_OS_WINDOWS)
//	//	{
//	//		out0 = dword_ptr(rcx);
//	//		out1 = dword_ptr(rdx);
//	//	}
//	//	else
//	//	{
//	//		out0 = dword_ptr(rdx);
//	//		out1 = dword_ptr(rsi);
//	//	}
//	//}
//	//else
//	//{
//	//	a.mov(eax, dword_ptr(ebp, 4));
//	//	out0 = dword_ptr(eax);
//	//	a.mov(eax, dword_ptr(ebp, 8));
//	//	out1 = dword_ptr(eax);
//	//}
//
//	compile_prog(*prog, a, 0, 2);
//	//float f = 10;
//	//a.mov(out0, *reinterpret_cast<int*>(&f));
//	//a.ret();
//
//	void(*fn)(float*,float*);
//	auto err = rt.add(&fn, &code);
//	if (err) throw 0;
//
//	return fn;
//}
//

//X86Mem pewpew::fetch_out(X86Assembler& a, int i)
//{
//	if (ASMJIT_ARCH_64BIT)
//	{
//		if (ASMJIT_OS_WINDOWS)
//		{
//			if (i == 0)
//				return dword_ptr(rcx);
//			else if (i == 1)
//				return dword_ptr(rdx);
//		}
//		else
//		{
//			if (i == 0)
//				return dword_ptr(rdx);
//			else if (i == 1)
//				return dword_ptr(rsi);
//		}
//	}
//	else
//	{
//		a.mov(eax, dword_ptr(esp, (i+1) * 4));
//		return dword_ptr(eax);
//	}
//}
//
//

//void pewpew::compile_prog(ASTNodeProg & p, X86Assembler & a, int in_count, int out_count)
//{
//	std::function<void(ASTNodeReturn*)> compile_return;
//	std::function<void(ASTNodeBinary*)> compile_binary;
//
//	compile_return = [this, &a, &in_count, &out_count, &compile_binary](ASTNodeReturn* node) -> void
//	{
//		for (int i = 0; i < out_count; i++)
//		{
//			if (node->nodes[0]->type == ASTNodeType::NUMBER)
//			{
//				auto num = static_cast<ASTNodeNumber*>(node->nodes[i])->value;
//				a.mov(fetch_out(a, i), *reinterpret_cast<int*>(&num));
//			}
//		}
//		a.ret();
//	};
//
//	compile_binary = [this, &a, &in_count, &out_count, &compile_binary](ASTNodeBinary* node) -> void
//	{
//		if (node->left->type == ASTNodeType::NUMBER)
//		{
//			auto value = static_cast<ASTNodeNumber*>(node->left)->value;
//			auto mem = cc.newFloatConst(kConstScopeLocal, value);
//
//			cc.fld(mem);
//		}
//		else if (node->left->type == ASTNodeType::VAR)
//		{
//			auto name = static_cast<ASTNodeVar*>(node->left)->name;
//			auto it = vars.find(name);
//			if (it == vars.end())
//				throw 0;
//			auto var = it->second;
//
//			cc.fld(var);
//		}
//		else if (node->left->type == ASTNodeType::BINARY)
//			compile_binary(static_cast<ASTNodeBinary*>(node->left));
//		else
//			throw 0;
//
//		if (node->right->type == ASTNodeType::NUMBER)
//		{
//			auto value = static_cast<ASTNodeNumber*>(node->right)->value;
//			auto mem = cc.newFloatConst(kConstScopeLocal, value);
//			cc.fld(mem);
//		}
//		else if (node->right->type == ASTNodeType::VAR)
//		{
//			auto name = static_cast<ASTNodeVar*>(node->right)->name;
//			auto it = vars.find(name);
//			if (it == vars.end())
//				throw 0;
//			auto var = it->second;
//
//			cc.fld(var);
//		}
//		else if (node->right->type == ASTNodeType::BINARY)
//			compile_binary(static_cast<ASTNodeBinary*>(node->right));
//		else
//			throw 0;
//
//		cc.faddp();
//	};
//
//	for (size_t i = 0; i < p.prog.size(); i++)
//	{
//		auto node = p.prog[i];
//
//		if (node->type == ASTNodeType::RETURN)
//			compile_return(static_cast<ASTNodeReturn*>(node));
//		else
//			throw 0;
//	}
//}
//

//template<typename ... ARGS>
//auto pewpew::get_func(std::string name) -> void(*)(ARGS...)
//{
//	for (size_t i = 0; i < ast.prog.size(); i++)
//	{
//		auto assign = static_cast<ASTNodeAssign*>(ast.prog[i]);
//		auto var = assign->left;
//
//		if (var->name != name)
//			continue;
//
//		if (assign->right->type != ASTNodeType::LAMBDA)
//			throw 0;
//
//		auto lambda = static_cast<ASTNodeLambda*>(assign->right);
//		
//		int out_size = 0;
//		{
//			ASTNodeReturn* ret = nullptr;
//			if (lambda->body->type == ASTNodeType::PROG)
//			{
//				auto prog = static_cast<ASTNodeProg*>(lambda->body);
//				for (size_t i = 0; i < prog->prog.size(); i++)
//				{
//					auto node = prog->prog[i];
//					if (node->type == ASTNodeType::RETURN)
//						ret = static_cast<ASTNodeReturn*>(node);
//				}
//			}
//
//			if (ret != nullptr)
//			{
//				out_size = sizeof...(ARGS) - lambda->ins.size();
//				// TODO: Array implementation (out_size increases)
//			}
//		}
//
//		using namespace asmjit;
//		using namespace asmjit::x86;
//		void(*fn)(ARGS...);
//
//		CodeHolder code;
//		code.init(rt.getCodeInfo());
//
//		X86Compiler cc(&code);
//		cc.addFunc(FuncSignatureT<void, ARGS...>());
//
//		auto& StackBasePtrReg = edx;
//		auto& CalcReg = eax;
//		auto& StackPtrReg = esp;
//		auto tmp1 = cc.newIntPtr("tmp1");
//		auto tmp2 = cc.newIntPtr("tmp2");
//
//		cc.mov(StackBasePtrReg, StackPtrReg);
//		
//		std::map<std::string, X86Mem> vars;
//		auto zero = cc.newFloatConst(kConstScopeLocal, 0.0f);
//
//		//for (size_t i = 0; i < lambda->ins.size(); i++)
//		//{
//		//	cc.setArg(i, tmp);
//		//	vars.insert({ lambda->ins[i], dword_ptr(StackPtrReg) });
//		//	cc.push(tmp);
//		//}
//
//		std::vector<X86Mem> outs;
//		cc.setArg(lambda->ins.size() + i, tmp1);
//		outs.push_back(dword_ptr(tmp1));
//		cc.setArg(lambda->ins.size() + i, tmp2);
//		outs.push_back(dword_ptr(tmp2));
//		//for (size_t i = 0; i < out_size; i++)
//		//{
//		//	cc.setArg(lambda->ins.size() + i, tmp);
//		//	outs.push_back(dword_ptr(tmp));
//		//}
//
//		if (lambda->body->type == ASTNodeType::PROG)
//		{
//			auto prog = static_cast<ASTNodeProg*>(lambda->body);
//
//			std::function<void(ASTNodeAssign*)> compile_assign;
//			std::function<void(ASTNodeBinary*)> compile_binary;
//			std::function<void(ASTNodeReturn*)> compile_return;
//
//			compile_return = [&cc, &outs, &vars, &compile_binary, &StackBasePtrReg](ASTNodeReturn* node) -> void
//			{
//				if (outs.size() > node->nodes.size())
//					throw 0;
//
//				for (size_t i = 0; i < outs.size(); i++)
//				{
//					if (node->nodes[i]->type == ASTNodeType::NUMBER)
//					{
//						auto value = static_cast<ASTNodeNumber*>(node->nodes[i])->value;
//						auto mem = cc.newFloatConst(kConstScopeLocal, value);
//
//						cc.mov(outs[i], 10);
//						//cc.fld(dword_ptr(StackBasePtrReg));
//						//cc.fst(outs[i]);
//					}
//					//else if (node->nodes[i]->type == ASTNodeType::BINARY)
//					//{
//					//	compile_binary(static_cast<ASTNodeBinary*>(node->nodes[i]));
//
//					//	cc.fst(outs[i]);
//					//}
//					//else if (node->nodes[i]->type == ASTNodeType::VAR)
//					//{
//					//	auto name = static_cast<ASTNodeVar*>(node->nodes[i])->name;
//					//	auto it = vars.find(name);
//					//	if (it == vars.end())
//					//		throw 0;
//
//					//	auto var = it->second;
//
//					//	cc.fld(var);
//					//	cc.fst(outs[i]);
//					//}
//					else
//						throw 0;
//				}
//			};
//
//			compile_assign = [&cc, &vars, &zero, &compile_binary, &CalcReg, &StackPtrReg](ASTNodeAssign* node) -> void
//			{
//				auto var_it = vars.find(node->left->name);
//				if (var_it == vars.end())
//				{
//					vars.insert({ node->left->name, dword_ptr(StackPtrReg) });
//					cc.push(zero);
//				}
//				auto var = vars[node->left->name];
//
//				if (node->right->type == ASTNodeType::NUMBER)
//				{
//					auto num = static_cast<ASTNodeNumber*>(node->right)->value;
//					auto mem = cc.newFloatConst(kConstScopeLocal, num);
//
//					cc.mov(CalcReg, mem);
//					cc.mov(var, CalcReg);
//				}
//				else
//					throw 0;
//			};
//
//			compile_binary = [&cc, &vars, &compile_binary](ASTNodeBinary* node) -> void
//			{
//				if (node->left->type == ASTNodeType::NUMBER)
//				{
//					auto value = static_cast<ASTNodeNumber*>(node->left)->value;
//					auto mem = cc.newFloatConst(kConstScopeLocal, value);
//					cc.fld(mem);
//				}
//				else if (node->left->type == ASTNodeType::VAR)
//				{
//					auto name = static_cast<ASTNodeVar*>(node->left)->name;
//					auto it = vars.find(name);
//					if (it == vars.end())
//						throw 0;
//					auto var = it->second;
//
//					cc.fld(var);
//				}
//				else if (node->left->type == ASTNodeType::BINARY)
//					compile_binary(static_cast<ASTNodeBinary*>(node->left));
//				else
//					throw 0;
//
//				if (node->right->type == ASTNodeType::NUMBER)
//				{
//					auto value = static_cast<ASTNodeNumber*>(node->right)->value;
//					auto mem = cc.newFloatConst(kConstScopeLocal, value);
//					cc.fld(mem);
//				}
//				else if (node->right->type == ASTNodeType::VAR)
//				{
//					auto name = static_cast<ASTNodeVar*>(node->right)->name;
//					auto it = vars.find(name);
//					if (it == vars.end())
//						throw 0;
//					auto var = it->second;
//
//					cc.fld(var);
//				}
//				else if (node->right->type == ASTNodeType::BINARY)
//					compile_binary(static_cast<ASTNodeBinary*>(node->right));
//				else
//					throw 0;
//
//				cc.faddp();
//			};
//
//			for (size_t i = 0; i < prog->prog.size(); i++)
//			{
//				auto node = prog->prog[i];
//
//				if (node->type == ASTNodeType::RETURN)
//					compile_return(static_cast<ASTNodeReturn*>(node));
//				else if (node->type == ASTNodeType::ASSIGN)
//					compile_assign(static_cast<ASTNodeAssign*>(node));
//				else
//					throw 0;
//			}
//		}
//		else throw 0;
//
//		// reset stack
//		cc.mov(StackPtrReg, StackBasePtrReg);
//
//		cc.endFunc();
//		cc.finalize();
//		auto err = rt.add(&fn, &code);
//
//		if (err)
//			throw 0;
//
//		return fn;
//	}
//
//	throw 0;
//}

#endif /* PEWPEW_HPP */