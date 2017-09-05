#ifndef COMPILE_HPP
#define COMPILE_HPP

#include "asmjit/asmjit.h"
#include "ast/ast_node.hpp"

using namespace asmjit;
using namespace asmjit::x86;

template<int N, typename... Ts> using NthTypeOf =
typename std::tuple_element<N, std::tuple<Ts...>>::type;

struct Type
{
	size_t size;
	char t;

	template<class T>
	static Type from();

	template<>
	static Type from<float>() { return{ sizeof(float), 'f' }; }

	template<>
	static Type from<int>() { return{ sizeof(int), 'i' }; }
};

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

			CodeHolder code;
			code.init(rt.getCodeInfo());

			X86Compiler cc(&code);

			cc.addFunc(FuncSignatureT<void, Ins..., Outs...>());
			
			std::map<std::string, Type> typemap;
			typenize<Ins...>::all<sizeof...(Ins)-1>::call(node, typemap);

			std::map<std::string, int> ins;
			for (size_t i = 0; i < sizeof...(Ins); i++)
				ins.insert({ node->ins[i], i });

			std::map<std::string, X86Mem> vars;

			compile<Outs...>::prog<Ins...>::call(prog, cc, typemap, ins, vars);

			cc.endFunc();
			cc.finalize();

			void(*fn)(Ins..., Outs...);
			auto err = rt.add(&fn, &code);
			if (err) throw 0;

			return fn;
		}
	};

	template<class ... Ins>
	struct typenize
	{
		template<int N, class T>
		struct one
		{
			static void call(ASTNodeLambda* node, std::map<std::string, Type>& typemap)
			{
				typemap.insert({
					node->ins[N],
					Type::from<T>()
				});
			}
		};

		template<int N>
		struct all
		{
			static void call(ASTNodeLambda* node, std::map<std::string, Type>& typemap)
			{
				one<N, NthTypeOf<N, Ins...>>::call(node, typemap);
				all<N - 1>::call(node, typemap);
			}
		};

		template<>
		struct all<0>
		{
			static void call(ASTNodeLambda* node, std::map<std::string, Type>& typemap)
			{
				one<0, NthTypeOf<0, Ins...>>::call(node, typemap);
			}
		};

		template<>
		struct all<-1>
		{
			static void call(ASTNodeLambda* node, std::map<std::string, Type>& typemap) { }
		};
	};

	//template<class ... Ins>
	//struct typenize<0, Ins...>
	//{
	//	static void call(ASTNodeLambda* node, std::map<std::string, Type>& typemap)
	//	{
	//		typemap.insert({
	//			node->ins[0],
	//			Type::from<NthTypeOf<0, Ins...>>()
	//		});
	//	}
	//};

	template<class ... Ins>
	struct prog
	{
		static void call(ASTNodeProg* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars)
		{
			for (size_t i = 0; i < node->prog.size(); i++)
			{
				auto n = node->prog[i];

				if (n->type == ASTNodeType::RETURN)
					compile<Outs...>
					::ret<Ins...>
					::all<sizeof...(Outs)-1>
					::call(static_cast<ASTNodeReturn*>(n), cc, typemap, ins, vars);
				else
					throw 0;
			}
		}
	};

	struct binary
	{
		static void call(ASTNodeBinary* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars)
		{
			if (node->left->type == ASTNodeType::NUMBER)
			{
				auto num = static_cast<ASTNodeNumber*>(node->left)->value;
				auto mem = cc.newFloatConst(kConstScopeLocal, num);

				cc.fld(mem);
			}
			else if (node->left->type == ASTNodeType::BINARY)
				compile<Outs...>::binary::call(static_cast<ASTNodeBinary*>(node->left), cc, typemap, ins, vars);
			else
				throw 0;

			if (node->right->type == ASTNodeType::NUMBER)
			{
				auto num = static_cast<ASTNodeNumber*>(node->right)->value;
				auto mem = cc.newFloatConst(kConstScopeLocal, num);

				cc.fld(mem);
			}
			else if (node->right->type == ASTNodeType::BINARY)
				compile<Outs...>::binary::call(static_cast<ASTNodeBinary*>(node->right), cc, typemap, ins, vars);
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
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars);
		};

		template<int N>
		struct one<N, float*>
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars)
			{
				if (node->nodes[N]->type == ASTNodeType::NUMBER)
				{
					auto num = static_cast<ASTNodeNumber*>(node->nodes[N])->value;
					auto mem = cc.newFloatConst(kConstScopeLocal, num);

					cc.fld(mem);
				}
				else if (node->nodes[N]->type == ASTNodeType::VAR)
				{
					auto name = static_cast<ASTNodeVar*>(node->nodes[N])->name;

					auto in_it = ins.find(name);
					if (in_it != ins.end())
					{
						auto in = fetch::arg(cc, ins[name]);
						cc.push(in);
						if (typemap[name].t == 'f')
							cc.fld(dword_ptr(esp));
						else if (typemap[name].t == 'i')
							cc.fild(dword_ptr(esp));
						cc.pop(in);
					}
					else
						throw;
				}
				else if (node->nodes[N]->type == ASTNodeType::BINARY)
					compile<Outs...>::binary::call(static_cast<ASTNodeBinary*>(node->nodes[N]), cc, typemap, ins, vars);
				else
					throw 0;

				auto arg = fetch::arg(cc, N + sizeof...(Ins));
				cc.fst(dword_ptr(arg));
			}
		};

		template<int N>
		struct all
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars)
			{
				one<N, NthTypeOf<N, Outs...>>::call(node, cc, typemap, ins, vars);
				all<N - 1>::call(node, cc, typemap, ins, vars);
			}
		};

		template<>
		struct all<0>
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars)
			{
				one<0, NthTypeOf<0, Outs...>>::call(node, cc, typemap, ins, vars);
			}
		};

		template<>
		struct all<-1>
		{
			static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars)
			{
			}
		};
	};

	//template<unsigned OFF, unsigned INC>
	//struct ret<OFF, INC, float*>
	//{
	//	static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars)
	//	{
	//		if (node->nodes[OFF]->type == ASTNodeType::NUMBER)
	//		{
	//			auto num = static_cast<ASTNodeNumber*>(node->nodes[OFF])->value;
	//			auto mem = cc.newFloatConst(kConstScopeLocal, num);

	//			cc.fld(mem);
	//		}
	//		else if (node->nodes[OFF]->type == ASTNodeType::VAR)
	//		{
	//			auto name = static_cast<ASTNodeVar*>(node->nodes[OFF])->name;

	//			auto in_it = ins.find(name);
	//			if (in_it != ins.end())
	//			{
	//				auto in = fetch::arg(cc, ins[name]);
	//				cc.push(in);
	//				if (typemap[name].t == 'f')
	//					cc.fld(dword_ptr(esp));
	//				else if (typemap[name].t == 'i')
	//					cc.fild(dword_ptr(esp));
	//				cc.pop(in);
	//			}
	//			else
	//				throw;
	//		}
	//		else if (node->nodes[OFF]->type == ASTNodeType::BINARY)
	//			compile<Outs...>::binary::call(static_cast<ASTNodeBinary*>(node->nodes[OFF]), cc, typemap, ins, vars);
	//		else
	//			throw 0;

	//		auto arg = fetch::arg(cc, OFF + INC);
	//		cc.fst(dword_ptr(arg));

	//		ret<OFF - 1, INC, NthTypeOf<OFF - 1, Outs...>>::call(node, cc, ins, vars);
	//	}
	//};

	//template<unsigned INC>
	//struct ret<0, INC, float*>
	//{
	//	static void call(ASTNodeReturn* node, X86Compiler& cc, std::map<std::string, Type>& typemap, std::map<std::string, int>& ins, std::map<std::string, X86Mem>& vars)
	//	{
	//		if (node->nodes[0]->type == ASTNodeType::NUMBER)
	//		{
	//			auto num = static_cast<ASTNodeNumber*>(node->nodes[0])->value;
	//			auto mem = cc.newFloatConst(kConstScopeLocal, num);

	//			cc.fld(mem);
	//		}
	//		else if (node->nodes[0]->type == ASTNodeType::VAR)
	//		{
	//			auto name = static_cast<ASTNodeVar*>(node->nodes[0])->name;

	//			auto in_it = ins.find(name);
	//			if (in_it != ins.end())
	//			{
	//				auto in = fetch::arg(cc, ins[name]);
	//				cc.push(in);
	//				if (typemap[name].t == 'f')
	//					cc.fld(dword_ptr(esp));
	//				else if (typemap[name].t == 'i')
	//					cc.fild(dword_ptr(esp));
	//				cc.pop(in);
	//			}
	//			else
	//				throw;
	//		}
	//		else if (node->nodes[0]->type == ASTNodeType::BINARY)
	//			compile<Outs...>::binary::call(static_cast<ASTNodeBinary*>(node->nodes[0]), cc, typemap, ins, vars);
	//		else
	//			throw 0;

	//		auto arg = fetch::arg(cc, INC);
	//		cc.fst(dword_ptr(arg));

	//		cc.ret();
	//	}
	//};

};

#endif /* COMPILE_HPP */
