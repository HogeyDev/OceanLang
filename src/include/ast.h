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
  std::string codegen(DefinedScope *scope) {
    std::string ret = "";
    for (AST::Statement *stmt : this->statements) {
      ret += stmt->codegen(scope);
    }
    return ret;
  }
};

class VariableDeclaration : public AST::Statement {
public:
  std::string variableType;
  std::string variableName;
  AST::Expression *value;

  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";
    scope->createVariable(this->variableName, this->variableType);
    ret += this->value->codegen(scope);
    return ret;
  }
};

class Parameter {
public:
  std::string parameterType;
  std::string parameterName;

  std::string codegen() {
    std::string ret = "";

    return ret;
  }
};

class FunctionDeclaration : public AST::Statement {
public:
  std::string functionType;
  std::string functionName;
  std::vector<AST::Parameter *> parameters;
  AST::Scope *functionBody;

  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";
    for (auto &p : this->parameters) {
      scope->createVariable(p->parameterType, p->parameterName);
    }

    ret += "global " + this->functionName + '\n';
    ret += this->functionName + ":\n";
    ret += "\tpush rbp\n\tmov rbp, rsp\n";
    ret += this->functionBody->codegen(scope);
    ret += "\tmov rsp, rbp\n\tpop rbp\n\tret\n";

    return ret;
  }
};

class Argument {
public:
  AST::Expression *argumentValue;
};

class FunctionCall : public AST::Statement {
public:
  std::string functionName;
  std::vector<AST::Argument *> arguments;

  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";
    for (auto &a : this->arguments) {
      ret += a->argumentValue->codegen(scope);
      scope->stackSize++;
    }
    ret += "\tcall " + this->functionName + '\n';
    return ret;
  }
};

class BinaryExpression : public Expression {
public:
  AST::Expression *lhs;
  AST::Expression *rhs;
  BinOp op;
  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";

    ret += lhs->codegen(scope);
    ret += rhs->codegen(scope);
    ret += "\tpop rax\n\tpop rbx\n";
    switch (this->op) {
    case OP_ADD:
      ret += "\tadd rax, rbx\n";
      break;
    case OP_SUB:
      ret += "\tsub rax, rbx\n";
      break;
    case OP_MUL:
      ret += "\timul rax, rbx\n";
      break;
    case OP_DIV:
      ret += "\tdiv rbx\n";
      break;
    default:
      std::cerr << "Unimplemented op: " << this->op << std::endl;
      exit(1);
    }
    ret += "\tpush rax\n";

    return ret;
  }
};

class Return : public AST::Statement {
public:
  AST::Expression *returnValue;

  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";
    (void)scope;

    return ret;
  }
};

class Import : public AST::Statement {
public:
  std::string filePath;

  std::string codegen(DefinedScope *scope) override {
    (void)scope;
    return compileFile(getMainFilePrefixDirectory() + this->filePath);
  }
};

class Extern : public AST::Statement {
public:
  std::string language;
  std::string source;

  std::string codegen(DefinedScope *scope) override {
    (void)scope;

    if (this->language == "asm") {
      return this->source;
    }

    std::cerr << "Unimplemented external language: " << this->language
              << std::endl;
    exit(1);
  }
};

class If : public AST::Statement {
public:
  AST::Expression *condition;
  AST::Scope *body;

  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";

    ret += this->condition->codegen(scope);
    std::string labelId = std::to_string(newGlobalLabel());
    ret += "\tpop rax\n\tcmp rax, 0\n\tje lbl" + labelId + '\n';
    ret += this->body->codegen(scope);
    ret += "lbl" + labelId + ":\n";

    return ret;
  }
};

class IntegerLiteral : public Expression {
public:
  int value;

  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";
    (void)scope;
    ret += "\tpush " + std::to_string(this->value) + '\n';
    return ret;
  }
};

class StringLiteral : public Expression {
public:
  std::string value;

  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";
    (void)scope;
    std::cerr << "STRLIT: " << this->value << std::endl;
    exit(1);
    return ret;
  }
};

class VariableRecall : public Expression {
public:
  std::string variableName;

  std::string codegen(DefinedScope *scope) override {
    std::string ret = "";
    (void)scope;
    return ret;
  }
};

}; // namespace AST

inline std::string compileToAsm(AST::Scope *root) {
  DefinedScope *globalScope = new DefinedScope();
  std::string assembly      = root->codegen(globalScope);
  std::cout << "----PRODUCED START----" << std::endl;
  std::cout << assembly << std::endl;
  std::cout << "-----PRODUCED END-----" << std::endl;
  return assembly;
}
