#pragma once

#include "compile.h"
#include "global.h"
#include "scope.h"
#include <iostream>
#include <string>
#include <vector>

typedef enum {
  // Arithmatic
  OP_ADD, // +
  OP_SUB, // -
  OP_MUL, // *
  OP_DIV, // /
  OP_MOD, // %

  // Comparison
  OP_EQ,   // ==
  OP_NEQ,  // !=
  OP_LT,   // <
  OP_GT,   // >
  OP_LTEQ, // <=
  OP_GTEQ, // >=

  // Unary
  OP_NEG, // -
  OP_INV, // !

  OP_OR,  // ||
  OP_AND, // &&
} BinOp;

namespace AST {

class Scope;
class Statement;
class Expression;
class FunctionDeclaration;
class Parameter;
class FunctionCall;
class Expression;
class BinaryExpression;
class Argument;
class Return;
class Import;
class Extern;
class If;
class While;
class IntegerLiteral;
class StringLiteral;
class VariableRecall;

class Statement {
public:
  virtual std::string codegen(DefinedScope *scope) = 0;
};

class Expression {
public:
  virtual std::string codegen(DefinedScope *scope) = 0;
};

class Scope {
public:
  std::vector<AST::Statement *> statements;
  std::string codegen(DefinedScope *scope);
};

class VariableDeclaration : public AST::Statement {
public:
  std::string variableType;
  std::string variableName;
  AST::Expression *value;

  std::string codegen(DefinedScope *scope);
};

class VariableAssignment : public AST::Statement {
public:
  std::string variableName;
  AST::Expression *value;

  std::string codegen(DefinedScope *scope);
};

class Parameter {
public:
  std::string parameterType;
  std::string parameterName;

  std::string codegen();
};

class FunctionDeclaration : public AST::Statement {
public:
  std::string functionType;
  std::string functionName;
  std::vector<AST::Parameter *> parameters;
  AST::Scope *functionBody;

  std::string codegen(DefinedScope *scope);
};

class Argument {
public:
  AST::Expression *argumentValue;
};

class FunctionCall : public AST::Statement, public AST::Expression {
public:
  std::string functionName;
  std::vector<AST::Argument *> arguments;

  std::string codegen(DefinedScope *scope);
};

class BinaryExpression : public Expression {
public:
  AST::Expression *lhs;
  AST::Expression *rhs;
  BinOp op;
  std::string codegen(DefinedScope *scope);
};

class Return : public AST::Statement {
public:
  AST::Expression *returnValue;

  std::string codegen(DefinedScope *scope);
};

class Import : public AST::Statement {
public:
  std::string filePath;

  std::string codegen(DefinedScope *scope);
};

class Extern : public AST::Statement {
public:
  std::string language;
  std::string source;

  std::string codegen(DefinedScope *scope);
};

class If : public AST::Statement {
public:
  AST::Expression *condition;
  AST::Scope *body;

  std::string codegen(DefinedScope *scope);
};

class While : public AST::Statement {
public:
  AST::Expression *condition;
  AST::Scope *body;

  std::string codegen(DefinedScope *scope);
};

class For : public AST::Statement {
public:
  AST::VariableAssignment *initialize;
  AST::Expression *test;
  AST::Expression *update;
  AST::Scope *body;

  std::string codegen(DefinedScope *scope);
};

class IntegerLiteral : public Expression {
public:
  int value;

  std::string codegen(DefinedScope *scope);
};

class StringLiteral : public Expression {
public:
  std::string value;

  std::string codegen(DefinedScope *scope);
};

class VariableRecall : public Expression {
public:
  std::string variableName;

  std::string codegen(DefinedScope *scope);
};

}; // namespace AST

std::string compileToAsm(AST::Scope *root, DefinedScope *scope);
