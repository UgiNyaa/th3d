#ifndef COMPILE_HPP
#define COMPILE_HPP

#include "asmjit/asmjit.h"
#include "ast/ast_node.hpp"

using namespace asmjit;
using namespace asmjit::x86;

template<int N, typename... Ts> using NthTypeOf =
typename std::tuple_element<N, std::tuple<Ts...>>::type;

template<class ... Outs>
struct compile {

	struct fetch
	{
		static X86Gp arg(X86Compiler& cc, int i)
		{
			if (ASMJIT_ARCH_64BIT)
			{
				if (ASMJIT_OS_WINDOWS)
				{
					if (i == 0)
						return rcx;
					else if (i == 1)
						return rdx;
				}
				else
				{
					if (i == 0)
						return rdx;
					else if (i == 1)
						return rsi;
				}
			}
			else
			{
				cc.mov(eax, dword_ptr(esp, (i + 1) * 4));
				return eax;
			}
		}
	};

	static int32_t min_offset(std::map<std::string, std::tuple<char, size_t, X86Mem>> vars)
	{
		int32_t min_offset = 0;
		for (auto& var : vars)
		{
			int32_t off = std::get<2>(var.second).getOffset();
			if (off < min_offset)
				min_offset = off;
		}
		return min_offset;
	}

	//static X86Mem fetch_arg(X86Compiler& cc, int i)
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
	//		cc.mov(eax, dword_ptr(esp, (i + 1) * 4));
	//		return dword_ptr(eax);
	//	}
	//}

	template<class ... Ins>
	struct lambda
	{
		static auto call(ASTNodeLambda* node, Runtime& rt)
			-> void(*)(Ins..., Outs...)
		{
			if (node->body->type != ASTNodeType::PROG)
				throw 0;

			auto prog = static_cast<ASTNodeProg*>(node->body);

			FileLogger logger(stdout);
			CodeHolder code;
			code.init(rt.getCodeInfo());
			code.setLogger(&logger);

			X86Compiler cc(&code);

			cc.addFunc(FuncSignatureT<void, Ins..., Outs...>());

			std::map<std::string, std::tuple<char, size_t, int>> ins;
			capture<Ins...>::all<sizeof...(Ins)-1>::call(node, ins);

			std::map<std::string, std::tuple<char, size_t, X86Mem>> vars;

			compile<Outs...>::prog<Ins...>::call(prog, cc, ins, vars);

			cc.endFunc();
			cc.finalize();

			void(*fn)(Ins..., Outs...);
			auto err = rt.add(&fn, &code);
			if (err) throw 0;

			return fn;
		}
	};

	template<class ... Ins>
	struct capture
	{
		template<int N, class T>
		struct one
		{
			static void call(ASTNodeLambda* node, std::map<std::string, std::tuple<char, size_t, int>>& ins);
		};

		template<int N>
		struct one<N, float>
		{
			static void call(ASTNodeLambda* node, std::map<std::string, std::tuple<char, size_t, int>>& ins)
			{
				ins.insert({
					node->ins[N],
					std::make_tuple('f', 4, N)
				});
			}
		};

		template<int N>
		struct one<N, int>
		{
			static void call(ASTNodeLambda* node, std::map<std::string, std::tuple<char, size_t, int>>& ins)
			{
				ins.insert({
					node->ins[N],
					std::make_tuple('i', 4, N)
				});
			}
		};

		template<int N>
		struct all
		{
			static void call(ASTNodeLambda* node, std::map<std::string, std::tuple<char, size_t, int>>& ins)
			{
				one<N, NthTypeOf<N, Ins...>>::call(node, ins);
				all<N - 1>::call(node, ins);
			}
		};

		template<>
		struct all<0>
		{
			static void call(ASTNodeLambda* node, std::map<std::string, std::tuple<char, size_t, int>>& ins)
			{
				one<0, NthTypeOf<0, Ins...>>::call(node, ins);
			}
		};

		template<>
		struct all<-1>
		{
			static void call(ASTNodeLambda* node, std::map<std::string, std::tuple<char, size_t, int>> ins) { }
		};
	};

	template<class ... Ins>
	struct prog
	{
		static void call(ASTNodeProg* node, X86Compiler& cc, std::map<std::string, std::tuple<char, size_t, int>>& ins, std::map<std::string, std::tuple<char, size_t, X86Mem>>& vars)
		{
			for (size_t i = 0; i < node->prog.size(); i++)
			{
				auto n = node->prog[i];

				if (n->type == ASTNodeType::RETURN)
					compile<Outs...>
					::ret<Ins...>
					::all<sizeof...(Outs)-1>
					::call(static_cast<ASTNodeReturn*>(n), cc, ins, vars);
				else if (n->type == ASTNodeType::ASSIGN)
					compile<Outs...>::assign::call(static_cast<ASTNodeAssign*>(n), cc, ins, vars);
				else
					throw 0;
			}
		}
	};

	struct assign
	{
		static void call(ASTNodeAssign* node, X86Compiler& cc, std::map<std::string, std::tuple<char, size_t, int>>& ins, std::map<std::string, std::tuple<char, size_t, X86Mem>>& vars)
		{
			auto it = vars.find(node->left->name);
			if (it == vars.end())
			{
				vars.insert({ node->left->name, std::make_tuple('f', sizeof(float), ptr(esp, min_offset(vars) - sizeof(float), sizeof(float))) });
			}
			auto var = vars[node->left->name];

			if (node->right->type == ASTNodeType::NUMBER)
			{
				auto num = static_cast<ASTNodeNumber*>(node->right)->value;
				auto mem = cc.newFloatConst(kConstScopeLocal, num);

				cc.fld(mem);
				cc.fstp(std::get<2>(var));
			}
			else if (node->right->type == ASTNodeType::BINARY)
			{
				binary::call(static_cast<ASTNodeBinary*>(node->right), cc, ins, vars);

				cc.fstp(std::get<2>(var));
			}
			else
				throw 0;

			std::string comment("variable " + node->left->name + " is now stored in stack");
			cc.comment(comment.c_str());
		}
	};

	struct binary
	{
		static void call(ASTNodeBinary* node, X86Compiler& cc, std::map<std::string, std::tuple<char, size_t, int>>& ins, std::map<std::string, std::tuple<char, size_t, X86Mem>>& vars)
		{
			if (node->left->type == ASTNodeType::NUMBER)
			{
				auto num = static_cast<ASTNodeNumber*>(node->left)->value;
				auto mem = cc.newFloatConst(kConstScopeLocal, num);

				cc.fld(mem);
			}
			else if (node->left->type == ASTNodeType::BINARY)
				compile<Outs...>::binary::call(static_cast<ASTNodeBinary*>(node->left), cc, ins, vars);
			else
				throw 0;

			if (node->right->type == ASTNodeType::NUMBER)
			{
				auto num = static_cast<ASTNodeNumber*>(node->right)->value;
				auto mem = cc.newFloatConst(kConstScopeLocal, num);

				cc.fld(mem);
			}
			else if (node->right->type == ASTNodeType::BINARY)
				compile<Outs...>::binary::call(static_cast<ASTNodeBinary*>(node->right), cc, ins, vars);
			else
				throw 0;

			cc.faddp();
		}
	};

	template<class ... Ins>
	struct ret
	{
		template<int N, class T>
		struct one 
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, std::tuple<char, size_t, int>>& ins, std::map<std::string, std::tuple<char, size_t, X86Mem>>& vars);
		};

		template<int N>
		struct one<N, float*>
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, std::tuple<char, size_t, int>>& ins, std::map<std::string, std::tuple<char, size_t, X86Mem>>& vars)
			{
				if (node->nodes[N]->type == ASTNodeType::NUMBER)
				{
					auto num = static_cast<ASTNodeNumber*>(node->nodes[N])->value;
					auto mem = cc.newFloatConst(kConstScopeLocal, num);

					cc.fld(mem);

					std::string comment("returning number " + std::to_string(num) + " is now stored in float stack");
					cc.comment(comment.c_str());
				}
				else if (node->nodes[N]->type == ASTNodeType::VAR)
				{
					auto name = static_cast<ASTNodeVar*>(node->nodes[N])->name;

					auto in_it = ins.find(name);
					auto var_it = vars.find(name);

					if (in_it != ins.end())
					{
						auto in = fetch::arg(cc, std::get<2>(ins[name]));
						int size = std::get<1>(ins[name]);
						int off = min_offset(vars) - size;

						cc.mov(ptr(esp, off, size), in);
						if (std::get<0>(ins[name]) == 'f')
							cc.fld(ptr(esp, off, size));
						else if (std::get<0>(ins[name]) == 'i')
							cc.fild(ptr(esp, off, size));
						else
							throw 0;

						std::string comment("returning argument " + name + " is now stored in float stack");
						cc.comment(comment.c_str());
					}
					else if (var_it != vars.end())
					{
						auto var = var_it->second;

						if (std::get<0>(var) == 'f')
							cc.fld(std::get<2>(var));
						else if (std::get<0>(var) == 'i')
							cc.fild(std::get<2>(var));
						else
							throw 0;

						std::string comment("returning variable " + var_it->first + " is now stored in float stack");
						cc.comment(comment.c_str());
					}
					else
						throw 0;
				}
				else if (node->nodes[N]->type == ASTNodeType::BINARY)
					compile<Outs...>::binary::call(static_cast<ASTNodeBinary*>(node->nodes[N]), cc, ins, vars);
				else
					throw 0;

				auto arg = fetch::arg(cc, N + sizeof...(Ins));
				cc.fstp(dword_ptr(arg));

				std::string comment("returned at " + std::to_string(N + sizeof...(Ins)));
				cc.comment(comment.c_str());
			}
		};

		template<int N>
		struct all
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, std::tuple<char, size_t, int>>& ins, std::map<std::string, std::tuple<char, size_t, X86Mem>>& vars)
			{
				one<N, NthTypeOf<N, Outs...>>::call(node, cc, ins, vars);
				all<N - 1>::call(node, cc, ins, vars);
			}
		};

		template<>
		struct all<0>
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, std::tuple<char, size_t, int>>& ins, std::map<std::string, std::tuple<char, size_t, X86Mem>>& vars)
			{
				one<0, NthTypeOf<0, Outs...>>::call(node, cc, ins, vars);
			}
		};

		template<>
		struct all<-1>
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, std::tuple<char, size_t, int>>& ins, std::map<std::string, std::tuple<char, size_t, X86Mem>>& vars)
			{
			}
		};
	};
};

#endif /* COMPILE_HPP */
