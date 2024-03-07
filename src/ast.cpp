#include "ast.h"
#include <iostream>
#include <string>

namespace AST {

std::string Scope::codegen(DefinedScope *scope) {
  std::string ret        = "";
  DefinedScope *subScope = scope->newSubScope();
  ret += subScope->push("rbp") + "\tmov rbp, rsp\n";
  for (AST::Statement *stmt : this->statements) {
    ret += stmt->codegen(subScope);
  }
  scope->inheritStrings(subScope);
  ret += "\tmov rsp, rbp\n" + subScope->pop("rbp");
  return ret;
}

std::string VariableDeclaration::codegen(DefinedScope *scope) {
  std::string ret = "";
  ret += this->value->codegen(scope);
  scope->createVariable(this->variableName, this->variableType,
                        this->isPointerType);
  return ret;
}

std::string VariableAssignment::codegen(DefinedScope *scope) {
  std::string ret = "";
  ret += this->value->codegen(scope);
  ret += scope->pop("rax");
  int variableIndex = scope->getVariableIndex(this->variableName);
  ret += "\tmov QWORD [rsp+" + std::to_string(8 * variableIndex) + "], rax\n";
  // ret += "\tadd rsp, 8\n";
  // scope->stackSize--;
  return ret;
}

std::string Parameter::codegen() {
  std::string ret = "";

  return ret;
}

std::string FunctionDeclaration::codegen(DefinedScope *scope) {
  DefinedScope *functionScope = new DefinedScope();
  functionScope->inheritStrings(scope);
  functionScope->functions          = scope->functions;
  std::string ret                   = "";

  std::vector<std::string> argTypes = {};
  for (AST::Parameter *p : this->parameters) {
    argTypes.push_back(p->parameterType);
  }
  scope->createFunction(this->functionName, this->functionType, argTypes);
  for (int i = this->parameters.size() - 1; i >= 0; i--) { // backwards
    // for (unsigned int i = 0; i < this->parameters.size(); i++) {
    AST::Parameter *p = this->parameters.at(i);
    functionScope->createVariable(p->parameterName, p->parameterType,
                                  p->isPointerType);
  } // parameters are pushed to stack before call
  functionScope->stackSize++;
  ret += "global _" + this->functionName + '\n';
  ret += "_" + this->functionName + ":\n";
  ret += this->functionBody->codegen(functionScope);
  ret += "\tret\n";

  return ret;
}

std::string FunctionCall::codegen(DefinedScope *scope) {
  std::string ret = "";
  if (!scope->hasFunction(this->functionName)) {
    std::cerr << "No function defined with name \'" << this->functionName
              << "\'\n";
    std::exit(1);
  }
  std::vector<std::string> functionData =
      scope->getFunctionData(this->functionName);
  // for (unsigned int i = 0; i < this->arguments.size(); i++) {
  for (int i = this->arguments.size() - 1; i >= 0; i--) { // backwards
    ret += this->arguments.at(i)->argumentValue->codegen(scope);
  }
  ret += scope->call("_" + this->functionName);
  int popCount = (int)this->arguments.size();
  scope->stackSize -= popCount;
  ret += "\tadd rsp, " + std::to_string(8 * popCount) + '\n';
  if (functionData.at(1) != "void") {
    // ret += "\tpush rax\n"
    ret += scope->push("rax");
  }
  return ret;
}

std::string BinaryExpression::codegen(DefinedScope *scope) {
  std::string ret    = "";

  const bool isUnary = this->rhs == nullptr;

  ret += this->lhs->codegen(scope);
  if (!isUnary) {
    ret += this->rhs->codegen(scope);
    ret += scope->pop("rbx"); // rhs -> rbx
  }
  ret += scope->pop("rax"); // lhs -> rax
  switch (this->op) {
  case OP_ADD: {
    ret += "\tadd rax, rbx\n";
    break;
  }
  case OP_SUB: {
    ret += "\tsub rax, rbx\n";
    break;
  }
  case OP_MUL: {
    ret += "\timul rax, rbx\n";
    break;
  }
  case OP_DIV: {
    ret += "\tmov rdx, 0\n\tdiv rbx\n";
    break;
  }
  case OP_MOD: {
    ret += "\tmov rdx, 0\n\tdiv rbx\n\tmov rax, rdx\n";
    break;
  }
  // case OP_EQ:
  // case OP_NEQ:
  case OP_LT: {
    ret += "\tcmp rax, rbx\n\tsetl al\n\tmovzx rax, al\n";
    break;
  }
  case OP_GT: {
    ret += "\tcmp rax, rbx\n\tsetg al\n\tmovzx rax, al\n";
    break;
  }
  case OP_LTEQ: {
    ret += "\tcmp rax, rbx\n\tsetle al\n\tmovzx rax, al\n";
    break;
  }
  case OP_GTEQ: {
    ret += "\tcmp rax, rbx\n\tsetge al\n\tmovzx rax, al\n";
    break;
  }
  case OP_NEG: {
    ret += "\tneg rax\n";
    break;
  }
  // case OP_INV: // TODO
  case OP_OR: {
    ret += "\tor rax, rbx\n";
    break;
  }
  case OP_AND: {
    ret += "\tand rax, rbx\n";
    break;
  }
  case OP_DEREF: {
    ret += "\tmov rbx, rax\n";
    ret += "\tmov rax, [rbx]\n";
    break;
  }
  default: {
    std::cerr << "Unimplemented op: " << printableOperation(this->op)
              << std::endl;
    exit(1);
  }
  }
  ret += scope->push("rax");

  return ret;
}

std::string Return::codegen(DefinedScope *scope) {
  std::string ret = "";

  ret += this->returnValue->codegen(scope);
  ret += scope->pop("rax");
  ret += "\tmov rsp, rbp\n" + scope->pop("rbp") + "\tret\n";

  return ret;
}

std::string Import::codegen(DefinedScope *scope) {
  return compileFile(getMainFilePrefixDirectory() + this->filePath, scope);
}

std::string Extern::codegen(DefinedScope *scope) {
  (void)scope;

  if (this->language == "asm") {
    return this->source;
  }

  std::cerr << "Unimplemented external language: " << this->language
            << std::endl;
  exit(1);
}

std::string Expect::codegen(DefinedScope *scope) {
  std::string ret = "";
  if (this->type == 0) {
    // function
    scope->createFunction(this->externalName, this->externalType, {});
    ret += "\textern _" + this->externalName + "\n";
  } else if (this->type == 1) {
    // variable
    std::cerr << "Unimplemented" << std::endl;
    exit(99);
  }
  return ret;
}

std::string If::codegen(DefinedScope *scope) {
  std::string ret       = "";

  DefinedScope *ifScope = new DefinedScope();
  ifScope->inheritStrings(scope);
  ifScope->functions  = scope->functions;
  ifScope->variables  = scope->variables;
  ifScope->stackSize  = scope->stackSize;
  std::string labelId = std::to_string(newGlobalLabel());
  ret += this->condition->codegen(ifScope);
  ret += ifScope->pop("rax");
  /*
   * TODO: export all { ... } into a scope that starts with "push rbp..." and
   * ends with "...pop rbp"
   * */
  ret += "\tcmp rax, 0\n\tje lbl" + labelId + '\n';
  ret += this->body->codegen(ifScope);
  ret += "lbl" + labelId + ":\n";

  return ret;
}

std::string While::codegen(DefinedScope *scope) {
  std::string ret        = "";
  std::string startLabel = std::to_string(newGlobalLabel());
  std::string endLabel   = std::to_string(newGlobalLabel());
  ret += "lbl" + startLabel + ":\n";
  ret += this->condition->codegen(scope);
  ret += scope->pop("rax");
  ret += "\tcmp rax, 0\n\tje lbl" + endLabel + '\n';
  ret += this->body->codegen(scope);
  ret += "\tjmp lbl" + startLabel + '\n';
  ret += "lbl" + endLabel + ":\n";
  return ret;
}

std::string For::codegen(DefinedScope *scope) {
  std::string ret        = "";
  std::string startLabel = std::to_string(newGlobalLabel());
  std::string endLabel   = std::to_string(newGlobalLabel());
  ret += this->initialize->codegen(scope);
  ret += "lbl" + startLabel + ":\n";
  ret += this->test->codegen(scope);
  ret += scope->pop("rax");
  ret += "\tcmp rax, 0\n\tje lbl" + endLabel + '\n';
  ret += this->body->codegen(scope);
  ret += "\tjmp lbl" + startLabel + '\n';
  ret += "lbl" + endLabel + ":\n";
  return ret;
}

std::string IntegerLiteral::codegen(DefinedScope *scope) {
  std::string ret = "";
  ret += scope->push(std::to_string(this->value).c_str());
  return ret;
}

std::string StringLiteral::codegen(DefinedScope *scope) {
  std::string ret = "";
  int id          = scope->stringId(this->value);
  ret += scope->push("STR" + std::to_string(id));
  return ret;
}

std::string VariableRecall::codegen(DefinedScope *scope) {
  std::string ret = "";
  // std::cout << "SS: " << scope->stackSize << std::endl;
  // for (auto &v : scope->variables) {
  //   std::cout << v[0] << " " << v[2] << " => "
  //             << scope->getVariableIndex(v[0]) << std::endl;
  // }
  int index       = scope->getVariableIndex(this->variableName);
  // if (index < 0) {
  //   std::cerr << "Could not find variable named \'" << this->variableName
  //             << "\'" << std::endl;
  //   exit(1);
  // }
  ret += scope->push("QWORD [rsp+" + std::to_string(8 * index) + "]");
  return ret;
}

std::string VariableReference::codegen(DefinedScope *scope) {
  std::string ret          = "";

  const int variableOffset = scope->getVariableIndex(this->variableName);
  ret += "\tmov rax, rsp\n";
  ret += "\tsub rax," + std::to_string(variableOffset) + "\n";
  ret += "\tpush rax\n";

  return ret;
}
}; // namespace AST

std::string compileToAsm(AST::Scope *root, DefinedScope *scope) {
  DefinedScope *globalScope = scope->newSubScope();
  std::string assembly = "section .text\n" + root->codegen(globalScope) + '\n';
  return assembly;
}

std::string printableOperation(BinOp op) {
  std::string values[] = {
      "OP_ADD", "OP_SUB", "OP_MUL", "OP_DIV",   "OP_MOD",  "OP_EQ",
      "OP_NEQ", "OP_LT",  "OP_GT",  "OP_LTEQ",  "OP_GTEQ", "OP_NEG",
      "OP_INV", "OP_OR",  "OP_AND", "OP_DEREF",
  };
  return values[op];
}
