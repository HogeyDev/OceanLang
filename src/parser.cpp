#include "parser.h"
#include "include/ast.h"
#include "token.h"
#include <iostream>
#include <string>
#include <vector>

Token Parser::eat(TokenType type) {
  if (this->currTok.type != type) {
    std::cerr << "Expected token of type: " << printableTokenType(type)
              << ", but instead got token of type: "
              << printableTokenType(this->currTok.type) << std::endl;
    exit(1);
  }
  return this->advance();
}

Token Parser::advance() {
  this->index++;
  this->currTok = this->tokenList[this->index];
  return this->currTok;
}

Token Parser::get(int index) {
  if (index < 0 || index >= (int)this->tokenList.size())
    return {TOKEN_EOF, ""};
  return this->tokenList[index];
}

Token Parser::peek(int offset) { return this->get(this->index + offset); }

Parser::Parser(std::vector<Token> tokens) {
  this->tokenList = tokens;
  this->index     = 0;
  this->currTok   = this->tokenList[this->index];
}

AST::Scope *Parser::parse() { return this->scope(); }
AST::Scope *Parser::scope() {
  AST::Scope *ast = new AST::Scope();

  bool braceDelim = false;
  if (this->currTok.type == TOKEN_LBRACE) {
    braceDelim = true;
    this->eat(TOKEN_LBRACE);
  }

  while (this->currTok.type != TOKEN_EOF &&
         this->currTok.type != TOKEN_RBRACE) {
    AST::Statement *stmt = nullptr;
    if (this->currTok.type == TOKEN_IDENTIFIER) {
      // function declaration, function call, variable declaration, variable
      // assignment, return
      if (this->currTok.value == "return") {
        // return statement
        AST::Return *ret = new AST::Return();

        this->eat(TOKEN_IDENTIFIER);

        ret->returnValue = this->expression();

        this->eat(TOKEN_SEMI);

        stmt = ret;
      } else if (this->peek(1).type == TOKEN_IDENTIFIER) {
        // function declaration, variable declaration, return
        if (this->currTok.value == "extern") {
          // extern statement
          AST::Extern *ext = new AST::Extern();

          this->eat(TOKEN_IDENTIFIER);

          ext->language = this->currTok.value;
          this->eat(TOKEN_IDENTIFIER);

          ext->source = this->currTok.value;
          this->eat(TOKEN_STRING);

          this->eat(TOKEN_SEMI);

          stmt = ext;
        } else if (this->peek(2).type == TOKEN_LPAREN) {
          // function declaration
          AST::FunctionDeclaration *funcDecl = new AST::FunctionDeclaration();

          funcDecl->functionType             = this->currTok.value;
          this->eat(TOKEN_IDENTIFIER);

          funcDecl->functionName = this->currTok.value;
          this->eat(TOKEN_IDENTIFIER);

          this->eat(TOKEN_LPAREN);
          while (this->currTok.type != TOKEN_RPAREN) {
            AST::Parameter *param = new AST::Parameter();

            param->parameterType  = this->currTok.value;
            this->eat(TOKEN_IDENTIFIER);

            param->parameterName = this->currTok.value;
            this->eat(TOKEN_IDENTIFIER);

            funcDecl->parameters.push_back(param);
            if (this->currTok.type != TOKEN_RPAREN)
              this->eat(TOKEN_COMMA);
          }
          this->eat(TOKEN_RPAREN);

          funcDecl->functionBody = this->scope();

          stmt                   = funcDecl;
        } else if (this->peek(2).type == TOKEN_EQUALS) {
          // variable declaration
          AST::VariableDeclaration *varDecl = new AST::VariableDeclaration();

          varDecl->variableType             = this->currTok.value;
          this->eat(TOKEN_IDENTIFIER);

          varDecl->variableName = this->currTok.value;
          this->eat(TOKEN_IDENTIFIER);

          this->eat(TOKEN_EQUALS);

          varDecl->value = this->expression();

          this->eat(TOKEN_SEMI);

          stmt = varDecl;
        }
      } else if (this->currTok.value == "import") {
        // import statement
        AST::Import *imp = new AST::Import();

        this->eat(TOKEN_IDENTIFIER);

        imp->filePath = this->currTok.value;
        this->eat(TOKEN_STRING);

        this->eat(TOKEN_SEMI);

        stmt = imp;
      } else if (this->currTok.value == "if") {
        // if statement
        AST::If *ifStmt = new AST::If();

        this->eat(TOKEN_IDENTIFIER);

        this->eat(TOKEN_LPAREN);
        ifStmt->condition = this->expression();
        this->eat(TOKEN_RPAREN);

        ifStmt->body = this->scope();

        stmt         = ifStmt;
      } else if (this->currTok.value == "while") {
        AST::While *whileStmt = new AST::While();

        this->eat(TOKEN_IDENTIFIER);
        this->eat(TOKEN_LPAREN);
        whileStmt->condition = this->expression();
        this->eat(TOKEN_RPAREN);

        whileStmt->body = this->scope();

        stmt            = whileStmt;
      } else {
        // function call, variable assignment
        if (this->peek(1).type == TOKEN_LPAREN) {
          // function call
          AST::FunctionCall *funcCall = new AST::FunctionCall();

          funcCall->functionName      = this->currTok.value;
          this->eat(TOKEN_IDENTIFIER);

          this->eat(TOKEN_LPAREN);

          while (this->currTok.type != TOKEN_RPAREN) {
            AST::Argument *arg = new AST::Argument();

            arg->argumentValue = this->expression();

            funcCall->arguments.push_back(arg);
            if (this->currTok.type != TOKEN_RPAREN)
              this->eat(TOKEN_COMMA);
          }
          this->eat(TOKEN_RPAREN);
          this->eat(TOKEN_SEMI);

          stmt = funcCall;
        } else if (this->peek(1).type == TOKEN_EQUALS) {
          // AST::VariableDeclaration // TODO!!!
        }
      }
    }
    if (stmt == nullptr) {
      std::cerr << "No matching parse method found" << std::endl;

      const int range = 2;
      for (int offset = -range; offset <= range; offset++) {
        Token tok = this->peek(offset);
        if (tok.type == TOKEN_EOF)
          continue;
        std::cerr << (offset == 0 ? "> " : "  ") << printableToken(tok)
                  << std::endl;
      }

      exit(1);
    }

    ast->statements.push_back(stmt);
  }

  if (braceDelim) {
    this->eat(TOKEN_RBRACE);
  }
  return ast;
}

AST::Expression *Parser::expression() { return this->comparison(); }

AST::Expression *Parser::comparison() {
  AST::Expression *lhs = this->term();

  while (this->currTok.type == TOKEN_EQUALS_EQUALS ||
         this->currTok.type == TOKEN_BANG_EQUALS ||
         this->currTok.type == TOKEN_LESS_THAN ||
         this->currTok.type == TOKEN_MORE_THAN ||
         this->currTok.type == TOKEN_LESS_THAN_EQUALS ||
         this->currTok.type == TOKEN_MORE_THAN_EQUALS) {
    this->advance();
    AST::BinaryExpression *c = new AST::BinaryExpression();
    if (this->peek(-1).type == TOKEN_EQUALS_EQUALS) {
      c->op = OP_EQ;
    } else if (this->peek(-1).type == TOKEN_BANG_EQUALS) {
      c->op = OP_NEQ;
    } else if (this->peek(-1).type == TOKEN_LESS_THAN) {
      c->op = OP_LT;
    } else if (this->peek(-1).type == TOKEN_MORE_THAN) {
      c->op = OP_GT;
    } else if (this->peek(-1).type == TOKEN_LESS_THAN_EQUALS) {
      c->op = OP_LTEQ;
    } else if (this->peek(-1).type == TOKEN_MORE_THAN_EQUALS) {
      c->op = OP_GTEQ;
    } else {
      exit(51);
    }
    c->lhs = lhs;
    c->rhs = this->term();

    lhs    = c;
  }

  return lhs;
}

AST::Expression *Parser::term() {
  AST::Expression *lhs = this->factor();

  while (this->currTok.type == TOKEN_PLUS ||
         this->currTok.type == TOKEN_MINUS) {
    this->advance();
    AST::BinaryExpression *t = new AST::BinaryExpression();
    if (this->peek(-1).type == TOKEN_PLUS) {
      t->op = OP_ADD;
    } else if (this->peek(-1).type == TOKEN_MINUS) {
      t->op = OP_SUB;
    } else {
      exit(51);
    }
    t->lhs = lhs;
    t->rhs = this->factor();

    lhs    = t;
  }

  return lhs;
}

AST::Expression *Parser::factor() {
  AST::Expression *lhs = this->unary();

  while (this->currTok.type == TOKEN_STAR ||
         this->currTok.type == TOKEN_SLASH) {
    this->advance();
    AST::BinaryExpression *f = new AST::BinaryExpression();
    if (this->peek(-1).type == TOKEN_STAR) {
      f->op = OP_MUL;
    } else if (this->peek(-1).type == TOKEN_SLASH) {
      f->op = OP_DIV;
    } else {
      exit(51);
    }
    f->lhs = lhs;
    f->rhs = this->unary();

    lhs    = f;
  }

  return lhs;
}

AST::Expression *Parser::unary() {

  if (this->currTok.type == TOKEN_BANG || this->currTok.type == TOKEN_MINUS) {
    this->advance();
    AST::BinaryExpression *u = new AST::BinaryExpression();
    if (this->peek(-1).type == TOKEN_MINUS) {
      u->op = OP_NEG;
    } else if (this->peek(-1).type == TOKEN_BANG) {
      u->op = OP_INV;
    } else {
      exit(51);
    }
    u->lhs = this->primary();

    return u;
  }
  AST::Expression *lhs = this->primary();

  return lhs;
}

AST::Expression *Parser::primary() {
  if (this->currTok.type == TOKEN_INTEGER) {
    AST::IntegerLiteral *intLit = new AST::IntegerLiteral();
    intLit->value               = std::stoi(this->currTok.value);
    this->eat(TOKEN_INTEGER);
    return intLit;
  } else if (this->currTok.type == TOKEN_STRING) {
    AST::StringLiteral *strLit = new AST::StringLiteral();
    strLit->value              = this->currTok.value;
    this->eat(TOKEN_STRING);
    return strLit;
  } else if (this->currTok.type == TOKEN_IDENTIFIER) {
    if (this->peek(1).type == TOKEN_LPAREN) {
      AST::FunctionCall *funcCall = new AST::FunctionCall();
      funcCall->functionName      = this->currTok.value;
      this->eat(TOKEN_IDENTIFIER);
      this->eat(TOKEN_LPAREN);
      while (this->currTok.type != TOKEN_RPAREN) {
        AST::Argument *arg = new AST::Argument();

        arg->argumentValue = this->expression();

        funcCall->arguments.push_back(arg);
        if (this->currTok.type != TOKEN_RPAREN)
          this->eat(TOKEN_COMMA);
      }
      this->eat(TOKEN_RPAREN);
      return funcCall;
    }
    AST::VariableRecall *varRec = new AST::VariableRecall();
    varRec->variableName        = this->currTok.value;
    this->eat(TOKEN_IDENTIFIER);
    return varRec;
  } else if (this->currTok.type == TOKEN_LPAREN) {
    this->eat(TOKEN_LPAREN);
    AST::Expression *group = this->expression();
    this->eat(TOKEN_RPAREN);
    return group;
  }
  std::cerr << "Cannot parse expression: " << printableToken(this->currTok)
            << std::endl;
  exit(1);
}
