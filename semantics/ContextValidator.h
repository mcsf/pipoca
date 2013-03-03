
#ifndef __PIPOCA_SEMANTICS_CONTEXTVALIDATOR_H__
#define __PIPOCA_SEMANTICS_CONTEXTVALIDATOR_H__

#define INITOFFSET 8

#include <exception>
#include <string>
#include <iostream>
#include <vector>
#include <cdk/SymbolTable.h>
#include <cdk/nodes/expressions/Expression.h>
#include <cdk/nodes/expressions/BinaryExpression.h>
#include "semantics/SemanticProcessor.h"
#include "semantics/Symbol.h"

#include "semantics/ExpressionType.h"
#include "semantics/TypeValidator.h"
#include "semantics/PFerror.h"
#include "semantics/PFfunction.h"
#include "semantics/Function.h"

namespace pipoca {
  namespace semantics {

    //!
    //! Runs the program by interpreting the syntactic tree directly
    //!
    class ContextValidator: public virtual SemanticProcessor {
	    Function *_function;
	    cdk::SymbolTable<Symbol> &_symtab;	
	    bool _countMode;
	    int _offset;
		int _offsetTotal;
	
      public: 
		Function * getFunction();
		int  offset();
		void reset();
		void init(pipoca::node::FunctionDefNode * node);
		
      ContextValidator(std::ostream &os, cdk::SymbolTable<Symbol> &symtab) :
        SemanticProcessor(os), _function(NULL),  _symtab(symtab),_countMode(false),_offset(0),_offsetTotal(0) {
      }

    public:
      ~ContextValidator() {
        os().flush();
      }

		
    public:
      void processNode(cdk::node::Node * const node, int lvl) { throw 42; }
      void processNil(cdk::node::Nil * const node, int lvl) {}
      void processData(cdk::node::Data * const node, int lvl) {}
      void processComposite(cdk::node::Composite * const node, int lvl) {}
      

    public:
      template<typename T> void processSimple(cdk::node::expression::Simple<T> * const node,int lvl) { throw 42; }
	  
      void processInteger(cdk::node::expression::Integer * const node, int lvl) {}
	  void processDouble(cdk::node::expression::Double * const node, int lvl) {}
      void processString(cdk::node::expression::String * const node, int lvl) {}
      void processIdentifier(cdk::node::expression::Identifier * const node, int lvl){}

    public:
      void processUnaryExpression(cdk::node::expression::UnaryExpression * const node, int lvl) { throw 42; }
      void processNEG(cdk::node::expression::NEG * const node, int lvl) {}

    public:
      void processBinaryExpression(cdk::node::expression::BinaryExpression * const node, int lvl) { throw 42; }
      void processADD(cdk::node::expression::ADD * const node, int lvl) {}
      void processSUB(cdk::node::expression::SUB * const node, int lvl) {}
      void processMUL(cdk::node::expression::MUL * const node, int lvl) {}
      void processDIV(cdk::node::expression::DIV * const node, int lvl) {}
      void processMOD(cdk::node::expression::MOD * const node, int lvl) {}
      void processLT(cdk::node::expression::LT * const node, int lvl) {}
      void processLE(cdk::node::expression::LE * const node, int lvl) {}
      void processGE(cdk::node::expression::GE * const node, int lvl) {}
      void processGT(cdk::node::expression::GT * const node, int lvl) {}
      void processNE(cdk::node::expression::NE * const node, int lvl) {}
      void processEQ(cdk::node::expression::EQ * const node, int lvl) {}
	  
	  void processBrackets(pipoca::node::expression::Brackets * const node, int lvl){}
	  void processAttribution(pipoca::node::expression::Attribution * const node, int lvl) {}
	  void processAttributionIndex(pipoca::node::expression::AttributionIndex * const node, int lvl) {}
	  void processVector(pipoca::node::expression::Vector * const node, int lvl) {}
	  void processRead(pipoca::node::expression::Read * const node, int lvl) {}	  
	  void processPrint(pipoca::node::expression::Print * const node, int lvl) {}
	  void processINC(pipoca::node::expression::INC * const node, int lvl) {}
	  void processDEC(pipoca::node::expression::DEC * const node, int lvl) {}
	  void processPOW(pipoca::node::expression::POW * const node, int lvl) {}
	  void processOR(pipoca::node::expression::OR * const node, int lvl) {}
	  void processAND(pipoca::node::expression::AND * const node, int lvl) {}
	  void processNOT(pipoca::node::expression::NOT * const node, int lvl) {}
	  void processIndex(pipoca::node::expression::Index * const node, int lvl) {}	   
	  void processArgument(pipoca::node::expression::Argument * const node, int lvl){};
	  void processAddress(pipoca::node::expression::Address * const node, int lvl) {}
	  void processCondition(pipoca::node::expression::Condition * const node, int lvl){}
	  
	  void processWhileElseNode(pipoca::node::WhileElseNode * const node, int lvl);
	  void processIfElseNode(pipoca::node::IfElseNode * const node, int lvl); 
	  void processDoElseNode(pipoca::node::DoElseNode * const node, int lvl);
	  void processReturnNode(pipoca::node::ReturnNode * const node, int lvl) {}	 
	  void processContinueNode(pipoca::node::ContinueNode * const node, int lvl) {}
	  void processBreakNode(pipoca::node::BreakNode * const node, int lvl) {}
	  void processMemoryAllocationNode(pipoca::node::MemoryAllocationNode * const node, int lvl) {}
	 
    public:
      void processProgramNode(pipoca::node::ProgramNode * const node, int lvl) {};
	  void processInstructionNode(pipoca::node::InstructionNode * const node, int lvl) {}
	  void processFunctionCall(pipoca::node::expression::FunctionCall * const node, int lvl){}
	  
	  void processSequence(cdk::node::Sequence * const node, int lvl);
	  void processBlockNode(pipoca::node::BlockNode * const node, int lvl);
	  void processVarNode(pipoca::node::VarNode * const node, int lvl);
	  void processVarInitNode(pipoca::node::VarInitNode * const node, int lvl);
	  void processVarExtNode(pipoca::node::VarExtNode * const node, int lvl);
	  void processVarConstNode(pipoca::node::VarConstNode * const node, int lvl);
	  
	  void processFunctionDeclNode(pipoca::node::FunctionDeclNode * const node, int lvl);
	  void processFunctionDefNode(pipoca::node::FunctionDefNode * const node, int lvl);
    }; // class

  } // namespace semantics
} // namespace pipoca

#endif
