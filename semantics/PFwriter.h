// $Id: PFwriter.h,v 1.17 2010-05-27 23:21:53 ist162510 Exp $ -*- c++ -*-
/*
 * $Log: PFwriter.h,v $
 * Revision 1.17  2010-05-27 23:21:53  ist162510
 * Continue,return,break,inc,dec
 *
 * Revision 1.16  2010/05/27 05:58:37  ist162510
 * return,continue,break, vectors by value
 *
 * Revision 1.15  2010/05/25 22:15:28  ist162510
 * vectors implemented
 *
 * Revision 1.14  2010/05/24 22:35:29  ist162510
 * New nodes Brackets, condition, some bugs fixed.
 *
 * Revision 1.13  2010/05/24 02:11:16  ist162510
 * PFfunction done, it loads every extern or intern resource only when it is used.
 *
 * Revision 1.12  2010/05/23 14:24:55  ist162510
 * implemented run-time addition for strings, with doubles and integers
 * Init variables implemented.
 *
 * Revision 1.11  2010/05/22 11:35:28  ist162462
 * PF implementation of WhileElseNode and DoElseNode.
 *
 * Revision 1.10  2010-05-21 03:16:56  ist162510
 * Function call, return, args fully functional.
 * Const variables implemented.
 *
 * Revision 1.9  2010/05/20 02:32:35  ist162510
 * New exception class PFerror
 * makes easier to append several types of values to the error message
 *
 * Revision 1.8  2010/05/18 19:51:45  ist162510
 * Removed WhileNode IfNode DoNode
 * Function return value, arguments fully functional
 * Print and Read is now working
 *
 * Revision 1.7  2010/05/17 22:15:29  ist162510
 * all nodes names chaged to shorter version
 * New ContextValidator to manage all declarations
 * New InstructionNode to manage expression as instruction
 *
 * Revision 1.6  2010/05/15 18:11:28  ist162510
 * Lots of stuff
 *
 * Revision 1.5  2010/05/13 23:31:42  ist162510
 * TypeValidator files added
 * .y confilct solved
 * MemoryAllocationNode in semantic processor
 *
 * Revision 1.4  2010/05/13 23:19:15  ist162510
 * TypeVisitor is now TypeValidator
 * FunctionNode is now FunctionDefNode , FunctionDeclNode
 * Cycle instructions removed from .y
 *
 * Revision 1.3  2010/05/03 11:45:18  ist162510
 * Nodes added to semantics.
 *
 * Revision 1.2  2010/05/02 23:39:38  ist162510
 * *** empty log message ***
 *
 * Revision 1.1  2010/03/27 17:00:15  ist162462
 * Initial commit.
 *
 * Revision 1.8  2009/03/15 19:21:14  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.7  2009/03/02 17:40:20  david
 * Major changes: in addition to compiling with CDK4, Pipoca now has
 * its own namespace (pipoca). All classes are defined in it or in its
 * child namespaces. Added automatic node processing: the "nodes"
 * directory is scanned and nodes/all.h is built (contains both forward
 * declarations and include directives for all nodes -- in the appropriate
 * namespaces).
 *
 * Revision 1.5  2008/03/01 18:32:52  david
 * Pipoca for CDK3 (first version).
 *
 * Revision 1.3  2007/03/13 00:26:34  david
 * This version of the Pipoca compiler uses the new CDK2 library.
 * All nodes and visitors now use non-overloaded visitor hooks. The Pipoca
 * compiler now implements the abstract factory for scanners and compilers.
 *
 * Revision 1.2  2007/02/22 17:37:34  david
 * Fixed default parameter initialization error.
 */

#ifndef __PIPOCA_SEMANTICS_PFWRITER_H__
#define __PIPOCA_SEMANTICS_PFWRITER_H__

#include <vector>
#include <string>
#include <iostream>
#include <cdk/SymbolTable.h>
#include <cdk/generators/Postfix.h>
#include "semantics/SemanticProcessor.h"
#include "semantics/Symbol.h"
#include "semantics/ContextValidator.h"
#include "semantics/TypeValidator.h"

#include "semantics/PFerror.h"
#include "semantics/PFfunction.h"

namespace pipoca {
  namespace semantics {

    //!
    //! Traverse syntax tree and generate the corresponding nasm code.
    //!
    class PFwriter: public virtual SemanticProcessor {
      cdk::SymbolTable<Symbol> &_symtab;
      cdk::generator::Postfix &_pf;
      int _lbl;
	  ContextValidator _cv;
	  PFfunction _pfFunction;
	  int _error;
	  std::vector<int> _cycleBegin;
	  std::vector<int> _cycleEnd;
	  
    public:
      PFwriter(std::ostream &os, cdk::SymbolTable<Symbol> &symtab, cdk::generator::Postfix &pf) :
        SemanticProcessor(os), _symtab(symtab), _pf(pf), _lbl(0),_cv(os, symtab),_pfFunction(symtab,pf), _error(0), _cycleBegin(),_cycleEnd() {

      }

    public:
      ~PFwriter() {
        os().flush();
      }

    private:
	  void CALL(std::string f);
      std::string mklbl(int lbl);
	  void storeSymbol(Symbol* sym,bool array = false);
	  void loadSymbol(Symbol* sym,bool array = false);
	  void loadBinaryOperands(cdk::node::expression::BinaryExpression *node,int lvl);
	  void loadLogicOperands(cdk::node::expression::BinaryExpression *node,int lvl);	
      void loadBool(cdk::node::expression::Expression *node,int lvl);
	public:
      void processNode(cdk::node::Node * const node, int lvl) {
      }
      void processNil(cdk::node::Nil * const node, int lvl) {
      }
      void processData(cdk::node::Data * const node, int lvl) {
      }
      void processComposite(cdk::node::Composite * const node, int lvl) {
      }
      void processSequence(cdk::node::Sequence * const node, int lvl);

    public:
      template<typename T> void processSimple(cdk::node::expression::Simple<T> * const node,
                                              int lvl) {
      }
      void processInteger(cdk::node::expression::Integer * const node, int lvl);
	  void processDouble(cdk::node::expression::Double * const node, int lvl);
      void processString(cdk::node::expression::String * const node, int lvl);
	  void processVector(pipoca::node::expression::Vector * const node, int lvl);
	  void processVector(pipoca::node::expression::Vector * const node,int lvl, std::string label);
      void processIdentifier(cdk::node::expression::Identifier * const node, int lvl);

    public:
      void processUnaryExpression(cdk::node::expression::UnaryExpression * const node, int lvl) {
      }
      void processNEG(cdk::node::expression::NEG * const node, int lvl);

    public:
      void processBinaryExpression(cdk::node::expression::BinaryExpression * const node, int lvl) {
      }
      void processADD(cdk::node::expression::ADD * const node, int lvl);
      void processSUB(cdk::node::expression::SUB * const node, int lvl);
      void processMUL(cdk::node::expression::MUL * const node, int lvl);
      void processDIV(cdk::node::expression::DIV * const node, int lvl);
      void processMOD(cdk::node::expression::MOD * const node, int lvl);
      void processLT(cdk::node::expression::LT * const node, int lvl);
      void processLE(cdk::node::expression::LE * const node, int lvl);
      void processGE(cdk::node::expression::GE * const node, int lvl);
      void processGT(cdk::node::expression::GT * const node, int lvl);
      void processNE(cdk::node::expression::NE * const node, int lvl);
      void processEQ(cdk::node::expression::EQ * const node, int lvl);
	  
	  void processBrackets(pipoca::node::expression::Brackets * const node, int lvl);
	  void processAttribution(pipoca::node::expression::Attribution * const node, int lvl);
	  void processAttributionIndex(pipoca::node::expression::AttributionIndex * const node, int lvl);
	  void processRead(pipoca::node::expression::Read * const node, int lvl);	  
	  void processPrint(pipoca::node::expression::Print * const node, int lvl);
	  void processINC(pipoca::node::expression::INC * const node, int lvl);
	  void processDEC(pipoca::node::expression::DEC * const node, int lvl);
	  void processPOW(pipoca::node::expression::POW * const node, int lvl);
	  void processOR(pipoca::node::expression::OR * const node, int lvl);
	  void processAND(pipoca::node::expression::AND * const node, int lvl);
	  void processNOT(pipoca::node::expression::NOT * const node, int lvl);
	  void processIndex(pipoca::node::expression::Index * const node, int lvl);	  
	  void processFunctionCall(pipoca::node::expression::FunctionCall * const node, int lvl);	  
	  void processArgument(pipoca::node::expression::Argument * const node, int lvl){}
	  void processAddress(pipoca::node::expression::Address * const node, int lvl);
	  void processCondition(pipoca::node::expression::Condition * const node, int lvl);
	  
	  void processWhileElseNode(pipoca::node::WhileElseNode * const node, int lvl);
	  void processVarNode(pipoca::node::VarNode * const node, int lvl);
	  void processVarInitNode(pipoca::node::VarInitNode * const node, int lvl);
	  void processVarExtNode(pipoca::node::VarExtNode * const node, int lvl);
	  void processVarConstNode(pipoca::node::VarConstNode * const node, int lvl);
	  void processReturnNode(pipoca::node::ReturnNode * const node, int lvl);	  
	  void processFunctionDeclNode(pipoca::node::FunctionDeclNode * const node, int lvl);
	  void processFunctionDefNode(pipoca::node::FunctionDefNode * const node, int lvl);
	  void processDoElseNode(pipoca::node::DoElseNode * const node, int lvl);
	  void processContinueNode(pipoca::node::ContinueNode * const node, int lvl);
	  void processBreakNode(pipoca::node::BreakNode * const node, int lvl);
	  void processBlockNode(pipoca::node::BlockNode * const node, int lvl);
	  void processMemoryAllocationNode(pipoca::node::MemoryAllocationNode * const node, int lvl);
	  void processInstructionNode(pipoca::node::InstructionNode * const node, int lvl);
	  
	  
    public:
      void processProgramNode(pipoca::node::ProgramNode * const node, int lvl);
      void processIfElseNode(pipoca::node::IfElseNode * const node, int lvl);

    }; // class

  } // namespace semantics
} // namespace pipoca

#endif
