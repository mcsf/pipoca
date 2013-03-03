// $Id: SemanticProcessor.h,v 1.9 2010-05-24 22:35:29 ist162510 Exp $ -*- c++ -*-
/*
 * $Log: SemanticProcessor.h,v $
 * Revision 1.9  2010-05-24 22:35:29  ist162510
 * New nodes Brackets, condition, some bugs fixed.
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
 * Revision 1.7  2009/03/15 19:21:15  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.6  2009/03/02 17:40:21  david
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
 */

#ifndef __PIPOCA_SEMANTICS_SEMANTICPROCESSOR_H__
#define __PIPOCA_SEMANTICS_SEMANTICPROCESSOR_H__

#include <string>
#include <iostream>

/* include node forward declarations */
#define __NODE_DECLARATIONS_ONLY__
#include "nodes/all.h"  // automatically generated
#undef __NODE_DECLARATIONS_ONLY__

//!
//! Print nodes as XML elements to the output stream.
//!
class SemanticProcessor {
  //! The output stream
  std::ostream &_os;

protected:
  SemanticProcessor(std::ostream &os = std::cout) :
    _os(os) {
  }
  inline std::ostream &os() {
    return _os;
  }

public:
  virtual ~SemanticProcessor() {
  }

public:
  virtual void processNode(cdk::node::Node * const node, int lvl) = 0;
  virtual void processNil(cdk::node::Nil * const node, int lvl) = 0;
  virtual void processData(cdk::node::Data * const node, int lvl) = 0;
  virtual void processComposite(cdk::node::Composite * const node, int lvl) = 0;
  virtual void processSequence(cdk::node::Sequence * const node, int lvl) = 0;

public:
  template<typename T> void processSimple(cdk::node::expression::Simple<T> * const node, int lvl) {
    throw 42;
  }
  virtual void processInteger(cdk::node::expression::Integer * const node, int lvl) = 0;
  virtual void processDouble(cdk::node::expression::Double * const node, int lvl) = 0;
  virtual void processString(cdk::node::expression::String * const node, int lvl) = 0;
  virtual void processIdentifier(cdk::node::expression::Identifier * const node, int lvl) = 0;

public:
  virtual void
      processUnaryExpression(cdk::node::expression::UnaryExpression * const node, int lvl) = 0;
  virtual void processNEG(cdk::node::expression::NEG * const node, int lvl) = 0;

public:
  virtual void processBinaryExpression(cdk::node::expression::BinaryExpression * const node,
                                       int lvl) = 0;
  virtual void processADD(cdk::node::expression::ADD * const node, int lvl) = 0;
  virtual void processSUB(cdk::node::expression::SUB * const node, int lvl) = 0;
  virtual void processMUL(cdk::node::expression::MUL * const node, int lvl) = 0;
  virtual void processDIV(cdk::node::expression::DIV * const node, int lvl) = 0;
  virtual void processMOD(cdk::node::expression::MOD * const node, int lvl) = 0;
  virtual void processLT(cdk::node::expression::LT * const node, int lvl) = 0;
  virtual void processLE(cdk::node::expression::LE * const node, int lvl) = 0;
  virtual void processGE(cdk::node::expression::GE * const node, int lvl) = 0;
  virtual void processGT(cdk::node::expression::GT * const node, int lvl) = 0;
  virtual void processNE(cdk::node::expression::NE * const node, int lvl) = 0;
  virtual void processEQ(cdk::node::expression::EQ * const node, int lvl) = 0;
  
  virtual void processBrackets(pipoca::node::expression::Brackets * const node, int lvl) = 0;
  virtual void processAttribution(pipoca::node::expression::Attribution * const node, int lvl) = 0;
  virtual void processAttributionIndex(pipoca::node::expression::AttributionIndex * const node, int lvl) = 0;
  virtual void processVector(pipoca::node::expression::Vector * const node, int lvl) = 0;
  virtual void processRead(pipoca::node::expression::Read * const node, int lvl) = 0;	  
  virtual void processPrint(pipoca::node::expression::Print * const node, int lvl) = 0;
  virtual void processINC(pipoca::node::expression::INC * const node, int lvl) = 0;
  virtual void processDEC(pipoca::node::expression::DEC * const node, int lvl) = 0;
  virtual void processPOW(pipoca::node::expression::POW * const node, int lvl) = 0;
  virtual void processAND(pipoca::node::expression::AND * const node, int lvl) = 0;
  virtual void processOR(pipoca::node::expression::OR * const node, int lvl) = 0;
  virtual void processNOT(pipoca::node::expression::NOT * const node, int lvl) = 0;
  virtual void processIndex(pipoca::node::expression::Index * const node, int lvl) = 0;	  
  virtual void processFunctionCall(pipoca::node::expression::FunctionCall * const node, int lvl) = 0;	  
  virtual void processArgument(pipoca::node::expression::Argument * const node, int lvl) = 0;
  virtual void processAddress(pipoca::node::expression::Address * const node, int lvl) = 0;
  virtual void processCondition(pipoca::node::expression::Condition * const node, int lvl) = 0;
  
  virtual void processWhileElseNode(pipoca::node::WhileElseNode * const node, int lvl) = 0;
  virtual void processVarNode(pipoca::node::VarNode * const node, int lvl) = 0;
  virtual void processVarInitNode(pipoca::node::VarInitNode * const node, int lvl) = 0;
  virtual void processVarExtNode(pipoca::node::VarExtNode * const node, int lvl) = 0;
  virtual void processVarConstNode(pipoca::node::VarConstNode * const node, int lvl) = 0;
  virtual void processReturnNode(pipoca::node::ReturnNode * const node, int lvl) = 0;	  
  virtual void processFunctionDeclNode(pipoca::node::FunctionDeclNode * const node, int lvl) = 0;
  virtual void processFunctionDefNode(pipoca::node::FunctionDefNode * const node, int lvl) = 0;
  virtual void processDoElseNode(pipoca::node::DoElseNode * const node, int lvl) = 0;
  virtual void processContinueNode(pipoca::node::ContinueNode * const node, int lvl) = 0;
  virtual void processBreakNode(pipoca::node::BreakNode * const node, int lvl) = 0;
  virtual void processBlockNode(pipoca::node::BlockNode * const node, int lvl) = 0;
  virtual void processMemoryAllocationNode(pipoca::node::MemoryAllocationNode * const node, int lvl) = 0;
  virtual void processInstructionNode(pipoca::node::InstructionNode * const node, int lvl) = 0;
  
public:
  virtual void processProgramNode(pipoca::node::ProgramNode * const node, int lvl) = 0;
  virtual void processIfElseNode(pipoca::node::IfElseNode * const node, int lvl) = 0;

}; // class

#endif
