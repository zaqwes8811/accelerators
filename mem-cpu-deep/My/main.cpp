#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace llvm;

template<class T>
using Ptr = unique_ptr<T>;

namespace helper
{
// Cloning make_unique here until it's standard in C++14.
// Using a namespace to avoid conflicting with MSVC's make_unique (which
// ADL can sometimes find in unqualified calls).
template<class T, class ... Args>
static typename enable_if<!is_array<T>::value, unique_ptr<T>>::type make_unique(
        Args &&... args )
{
	return unique_ptr<T>(new T(forward<Args>(args)...));
}
} // end namespace helper

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token
{
	tok_eof = -1,

	// commands
	tok_def = -2,
	tok_extern = -3,

	// primary
	tok_identifier = -4,
	tok_number = -5,
};

static string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

static int gettok()
{
	static int LastChar = ' ';

	while( isspace(LastChar) )
		LastChar = getchar();

	if( isalpha((LastChar)) ){
		IdentifierStr = LastChar;
		while( isalnum((LastChar = getchar())) ){
			IdentifierStr += LastChar;
		}

		if( IdentifierStr == "def" ){
			return tok_def;
		}
		if( IdentifierStr == "extern" ){
			return tok_extern;
		}
		return tok_identifier;
	}

	if( isdigit(LastChar) || LastChar == '.' ){
		string NumStr;
		do{
			NumStr += LastChar;
			LastChar = getchar();
		}
		while( isdigit(LastChar) || LastChar == '.' );

		NumVal = strtod(NumStr.c_str(), 0);
		return tok_number;
	}
	if( LastChar == '#' ){
		do
			LastChar = getchar();
		while( LastChar != EOF && LastChar != '\n' && LastChar != '\r' );

		if( LastChar != EOF )
			return gettok();
	}

	if( LastChar == EOF )
		return tok_eof;

	int ThisChar = LastChar;
	LastChar = getchar();
	return ThisChar;
}

//===========================================================
// Parser
class ExprAST
{
public:
	virtual ~ExprAST()
	{
	}

	virtual llvm::Value *codegen() = 0;
};

class NumberExprAST: public ExprAST
{
	double Val;
public:
	NumberExprAST( double Val ) :
			Val(Val)
	{
	}
};

class VariableExprAST: public ExprAST
{
	string Name;

public:
	VariableExprAST( const string &Name ) :
			Name(Name)
	{
	}
};

class BinaryExprAST: public ExprAST
{
	char Op;
	Ptr<ExprAST> LHS, RHS;

public:
	BinaryExprAST( char op, Ptr<ExprAST> LHS, Ptr<ExprAST> RHS ) :
			Op(op), LHS(move(LHS)), RHS(move(RHS))
	{
	}
};

// CallExprAST - Expression class for function !!calls.
// не прототипы
class CallExprAST: public ExprAST
{
	string Callee;
	vector<Ptr<ExprAST>> Args;

public:
	CallExprAST( const string &Callee, vector<Ptr<ExprAST>> Args ) :
			Callee(Callee), Args(move(Args))
	{
	}
};

// PrototypeAST - This class represents the "prototype" for a function,
// which captures its name, and its argument names (thus implicitly the number
// of arguments the function takes).
class PrototypeAST
{
	string Name;
	vector<string> Args;

public:
	PrototypeAST( const string &name, vector<string> Args ) :
			Name(name), Args(move(Args))
	{
	}
};

// FunctionAST - This class represents a function definition itself.
class FunctionAST
{
	Ptr<PrototypeAST> Proto;
	Ptr<ExprAST> Body;

public:
	FunctionAST( Ptr<PrototypeAST> Proto, Ptr<ExprAST> Body ) :
			Proto(move(Proto)), Body(move(Body))
	{
	}
};

//===========================================================

// "This allows us to look one token ahead at what the lexer is returning. "
// fixme: почему вперед?
static int CurTok;
static int getNextToken()
{
	return CurTok = gettok();
}

Ptr<ExprAST> LogError( const char* Str )
{
	fprintf(stderr, "LogError: %s\n", Str);
	return nullptr;
}

Ptr<PrototypeAST> LogErrorP( const char* Str )
{
	LogError(Str);
	return nullptr;
}

//============================================================
//================== Primary =================================
//============================================================

// numberexpr ::= number
static Ptr<ExprAST> ParseNumberExpr()
{
	auto Result = helper::make_unique<NumberExprAST>(NumVal);
	getNextToken();
	return move(Result);
}

static Ptr<ExprAST> ParseExpression();

// parenexpr ::= '(' expression ')'
// Группировка
static Ptr<ExprAST> ParseParenExpr()
{
	getNextToken();
	// !!!! recursive !!!!
	auto V = ParseExpression();  // выражение в скобках
	if( !V )
		return nullptr;
	if( CurTok != ')' )
		return LogError("expected ')'");
	getNextToken();
	return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
// var or func call
static Ptr<ExprAST> ParseIdentifierExpr()
{
	string IdName = IdentifierStr;
	getNextToken();
	if( CurTok != '(' ){
		//!!!!!!! var
		return helper::make_unique<VariableExprAST>(IdName);
	}

	// Call.
	getNextToken();
	vector<Ptr<ExprAST>> Args;
	if( CurTok != ')' ){
		while( 1 ){
			if( auto Arg = ParseExpression() )
				Args.push_back(move(Arg));
			else{
				// fixme: why?
				return nullptr;
			}

			if( CurTok == ')' )
				break;
			if( CurTok != ',' )
				return LogError("Exp ')' ");
			getNextToken();
		}
	}

	getNextToken();
	return helper::make_unique<CallExprAST>(IdName, move(Args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
// name / name() / ()
static Ptr<ExprAST> ParsePrimary()
{
//	cout << "primary" << endl;
	// потом будет понятно...
	switch (CurTok) {
	default:
		return LogError("unk tok");
	case tok_identifier:
		return ParseIdentifierExpr();
	case tok_number:
		return ParseNumberExpr();
	case '(':
		return ParseParenExpr();
	}
}
//=====================================================

// binary - hard - Operator-Precedence Parsing.
// precedence - преоритет
static map<char, int> BinopPrecedence;

static int GetTokPrecedence()
{
	if( !isascii(CurTok) )
		return -1;
	int TokPrec = BinopPrecedence[CurTok];
	if( TokPrec <= 0 )
		return -1;
	return TokPrec;
}

// binoprhs
static Ptr<ExprAST> ParseBinOpRHS( int ExprPrec,  // fixme:???
        Ptr<ExprAST> LHS )
{
	while( 1 ){
		int TokPrec = GetTokPrecedence();
		if( TokPrec < ExprPrec )
			return LHS;
		int BinOp = CurTok;
		getNextToken();

		auto RHS = ParsePrimary();
		if( !RHS )
			return nullptr;

		int NextPrec = GetTokPrecedence();
		if( TokPrec < NextPrec ){
			RHS = ParseBinOpRHS(TokPrec + 1, move(RHS));
			if( !RHS )
				return nullptr;
		}

		// merge lhs/rhs
		LHS = helper::make_unique<BinaryExprAST>(BinOp, move(LHS), move(RHS));
	}
	return nullptr;
}

// !!!!!!!!! CORE !!!!!!!!!!!!
/// expression
///   ::= primary binoprhs
static Ptr<ExprAST> ParseExpression()
{
	auto LHS = ParsePrimary();
	if( !LHS )
		return nullptr;

	return ParseBinOpRHS(0, move(LHS));
}

//==================================================

/// prototype
///   ::= id '(' id* ')'
static Ptr<PrototypeAST> ParsePrototype()
{
	if( CurTok != tok_identifier )
		return LogErrorP("Expected function name in prototype");

	string FnName = IdentifierStr;
	getNextToken();

	if( CurTok != '(' )
		return LogErrorP("Expected '(' in prototype");

	vector<string> ArgNames;
	while( getNextToken() == tok_identifier )
		ArgNames.push_back(IdentifierStr);
	if( CurTok != ')' )
		return LogErrorP("Expected ')' in prototype");

	// success.
	getNextToken(); // eat ')'.

	return helper::make_unique<PrototypeAST>(FnName, move(ArgNames));
}

/// definition ::= 'def' prototype expression
static Ptr<FunctionAST> ParseDefinition()
{
	getNextToken(); // eat def.
	auto Proto = ParsePrototype();
	if( !Proto )
		return nullptr;

	if( auto E = ParseExpression() )
		return helper::make_unique<FunctionAST>(move(Proto), move(E));
	return nullptr;
}

/// toplevelexpr ::= expression
static Ptr<FunctionAST> ParseTopLevelExpr()
{
	if( auto E = ParseExpression() ){
		// Make an anonymous proto.
		auto Proto = helper::make_unique<PrototypeAST>("__anon_expr",
		        vector<string>());
		return helper::make_unique<FunctionAST>(move(Proto), move(E));
	}
	return nullptr;
}

/// external ::= 'extern' prototype
static Ptr<PrototypeAST> ParseExtern()
{
	getNextToken(); // eat extern.
	return ParsePrototype();
}

//==============================================
// Top level parsing

static void HandleDefinition()
{
	if( ParseDefinition() ){
		fprintf(stderr, "Parsed a function definition.\n");
	}
	else{
		// Skip token for error recovery.
		getNextToken();
	}
}

static void HandleExtern()
{
	if( ParseExtern() ){
		fprintf(stderr, "Parsed an extern\n");
	}
	else{
		// Skip token for error recovery.
		getNextToken();
	}
}

static void HandleTopLevelExpression()
{
	// Evaluate a top-level expression into an anonymous function.
	if( ParseTopLevelExpr() ){
		fprintf(stderr, "Parsed a top-level expr\n");
	}
	else{
		// Skip token for error recovery.
		getNextToken();
	}
}

// top ::= def | external | expr | ';'
static void MainLoop()
{
	while( true ){
		fprintf(stderr, "ready >");
		switch (CurTok) {
		case tok_eof:
			return;
		case ';':
			getNextToken();
			break;
		case tok_def:
			HandleDefinition();
			break;
		case tok_extern:
			HandleExtern();
			break;
		default:
			HandleTopLevelExpression();
		}
	}
}

int main()
{
	// Install standard binary operators.
	// 1 is lowest precedence.
	BinopPrecedence['<'] = 10;
	BinopPrecedence['+'] = 20;
	BinopPrecedence['-'] = 20;
	BinopPrecedence['*'] = 40; // highest.

	// Prime the first token.
	fprintf(stderr, "ready> ");
	getNextToken();

	// Run the main "interpreter loop" now.
	MainLoop();
	return 0;
}
