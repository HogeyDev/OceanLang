#include "ast.h"

namespace AST {

std::string Scope::codegen(DefinedScope *scope) {
  std::string ret        = "";
  DefinedScope *subScope = scope->newSubScope();
  for (AST::Statement *stmt : this->statements) {
    ret += stmt->codegen(subScope);
  }
  scope->inheritStrings(subScope);
  return ret;
}

std::string VariableDeclaration::codegen(DefinedScope *scope) {
  std::string ret = "";
  scope->createVariable(this->variableName, this->variableType);
  ret += this->value->codegen(scope);
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
    functionScope->createVariable(p->parameterName, p->parameterType);
  } // parameters are pushed to stack before call
  ret += "global _" + this->functionName + '\n';
  ret += "_" + this->functionName + ":\n";
  ret += functionScope->push("rbp") + "\tmov rbp, rsp\n";
  functionScope->stackSize++;
  ret += this->functionBody->codegen(functionScope);
  ret += "\tmov rsp, rbp\n" + functionScope->pop("rbp") + "\tret\n";

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
    ret += scope->push("rax");
  }
  return ret;
}

std::string BinaryExpression::codegen(DefinedScope *scope) {
  std::string ret = "";

  ret += lhs->codegen(scope);
  ret += rhs->codegen(scope);
  ret += scope->pop("rbx"); // rhs -> rbx
  ret += scope->pop("rax"); // lhs -> rax
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
  // case OP_EQ:
  // case OP_NEQ:
  case OP_LT:
    ret += "\tcmp rax, rbx\n\tsetl al\n\tmovzx rax, al\n";
    break;
  case OP_GT:
    ret += "\tcmp rax, rbx\n\tsetg al\n\tmovzx rax, al\n";
    break;
  case OP_LTEQ:
    ret += "\tcmp rax, rbx\n\tsetle al\n\tmovzx rax, al\n";
    break;
  case OP_GTEQ:
    ret += "\tcmp rax, rbx\n\tsetge al\n\tmovzx rax, al\n";
    break;
  // case OP_NEG:
  // case OP_INV:
  default:
    std::cerr << "Unimplemented op: " << this->op << std::endl;
    exit(1);
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

std::string If::codegen(DefinedScope *scope) {
  std::string ret = "";

  ret += this->condition->codegen(scope);
  std::string labelId = std::to_string(newGlobalLabel());
  ret += scope->pop("rax");
  ret += "\tcmp rax, 0\n\tje lbl" + labelId + '\n';
  ret += this->body->codegen(scope);
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
  ret += "jmp lbl" + startLabel + '\n';
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
}; // namespace AST

std::string compileToAsm(AST::Scope *root, DefinedScope *scope) {
  DefinedScope *globalScope = scope->newSubScope();
  std::string assembly = "section .text\n" + root->codegen(globalScope) + '\n';
  return assembly;
}
