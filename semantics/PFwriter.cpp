// $Id: PFwriter.cpp,v 1.30 2010-05-31 06:16:58 ist162510 Exp $ -*- c++ -*-
/*
 * $Log: PFwriter.cpp,v $
 * Revision 1.30  2010-05-31 06:16:58  ist162510
 * ci
 *
 * Revision 1.29  2010/05/30 22:39:16  ist162510
 * Pointers
 *
 * Revision 1.28  2010/05/30 00:30:51  ist162510
 * pointers in progress
 *
 * Revision 1.27  2010/05/29 01:46:01  ist162510
 * vector implemented in functions
 *
 * Revision 1.26  2010/05/29 01:31:22  ist162462
 * processMemoryAllocation in PFwriter and TypeValidator.
 *
 * Revision 1.25  2010-05-28 21:35:41  ist162510
 * vectors in progress
 * du until, continues, fixed
 *
 * Revision 1.24  2010/05/27 23:21:53  ist162510
 * Continue,return,break,inc,dec
 *
 * Revision 1.23  2010/05/27 05:58:37  ist162510
 * return,continue,break, vectors by value
 *
 * Revision 1.22  2010/05/25 22:15:28  ist162510
 * vectors implemented
 *
 * Revision 1.21  2010/05/24 22:35:29  ist162510
 * New nodes Brackets, condition, some bugs fixed.
 *
 * Revision 1.20  2010/05/24 11:08:22  uid162462
 * Implementations of IfElse and WhileElse nodes now allow null Else blocks.
 *
 * Revision 1.19  2010-05-24 02:11:16  ist162510
 * PFfunction done, it loads every extern or intern resource only when it is used.
 *
 * Revision 1.18  2010/05/23 14:24:55  ist162510
 * implemented run-time addition for strings, with doubles and integers
 * Init variables implemented.
 *
 * Revision 1.17  2010/05/22 11:35:28  ist162462
 * PF implementation of WhileElseNode and DoElseNode.
 *
 * Revision 1.16  2010-05-21 03:16:56  ist162510
 * Function call, return, args fully functional.
 * Const variables implemented.
 *
 * Revision 1.15  2010/05/20 02:32:35  ist162510
 * New exception class PFerror
 * makes easier to append several types of values to the error message
 *
 * Revision 1.14  2010/05/18 23:36:29  ist162510
 * FunctionDef/Call upgraded
 *
 * Revision 1.13  2010/05/18 19:51:44  ist162510
 * Removed WhileNode IfNode DoNode
 * Function return value, arguments fully functional
 * Print and Read is now working
 *
 * Revision 1.12  2010/05/17 23:48:50  ist162462
 * PFwriter.cpp: total or partial implementation of several node processors:
 * - Double;
 * - LT, LE, GE, GT, NE, EQ;
 * - Attribution (very partial);
 * - POW.
 *
 * Revision 1.11  2010-05-17 22:15:29  ist162510
 * all nodes names chaged to shorter version
 * New ContextValidator to manage all declarations
 * New InstructionNode to manage expression as instruction
 *
 * Revision 1.10  2010/05/15 18:11:28  ist162510
 * Lots of stuff
 *
 * Revision 1.9  2010/05/14 02:51:13  uid162462
 * PFwriter: NEG, ADD, SUB, MUL, DIV, MOD (partial)
 *
 * Revision 1.8  2010-05-13 23:31:42  ist162510
 * TypeValidator files added
 * .y confilct solved
 * MemoryAllocationNode in semantic processor
 *
 * Revision 1.7  2010/05/13 23:19:15  ist162510
 * TypeVisitor is now TypeValidator
 * FunctionNode is now FunctionDefNode , FunctionDeclNode
 * Cycle instructions removed from .y
 *
 * Revision 1.6  2010/05/13 00:08:15  ist162510
 * YEAAH!!!
 *
 * Revision 1.4  2010/05/12 20:50:13  ist162510
 * TypeValidator
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
 * Revision 1.8  2009/03/15 19:21:15  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.7  2009/03/02 17:40:21  david
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

#include <string>
#include <sstream>
#include <iostream>
#include "semantics/PFwriter.h"
#include "nodes/all.h"  // automatically generated

using namespace pipoca::semantics;

//---------------------------------------------------------------------------
//     THIS IS THE EVALUATOR'S DEFINITION
//---------------------------------------------------------------------------


//==========================HELPERS===============================
std::string PFwriter::mklbl(int lbl) {
	std::ostringstream oss;
	if (lbl < 0)
		oss << ".L" << -lbl;
	else
		oss << "_L" << lbl;
	return oss.str();
}
void PFwriter::storeSymbol(Symbol* sym,bool array){
	if(!sym) 
		throw PFerror()<<"PFwriter::storeSymbol: Loading NULL symbol";
	int  size = sym->type()->totalSize();

	if (sym->offset()==0) 	{
		_pf.ADDR(sym->name());
	}else{
		_pf.LOCAL(sym->offset());
	}

	if (size == 4) 			_pf.STORE(); 	// store the value at address
	else if ( size == 8 )	_pf.STORE2();
	else throw PFerror()<<"PFwriter::storeSymbol: Symbol '"<<sym->name()<<"' of unhandled size "<<size;	
}
void PFwriter::loadSymbol(Symbol* sym,bool array){
	if(!sym) 
		throw PFerror()<<"PFwriter::loadSymbol: Loading NULL symbol";
	int  size = sym->type()->totalSize();
	
	if (sym->offset()==0) 	{
		_pf.ADDR(sym->name());
	}else{
		_pf.LOCAL(sym->offset());
	}
	
	if( size == 4)			_pf.LOAD();
	else if (size == 8)		_pf.LOAD2();
	else throw PFerror()<<"PFwriter::loadSymbol: Symbol '"<<sym->name()<<"' of unhandled size "<<size;
	
}
void PFwriter::loadBinaryOperands(cdk::node::expression::BinaryExpression *node,int lvl){
	type tnode	= node->type()->name();
	type tleft 	= node->left()->type()->name();
	type tright = node->right()->type()->name();
	
	node->left()->accept(this, 0);
	if 		(tnode == TYPE_REAL && tleft == TYPE_INT)  _pf.I2D();
	else if (tnode == TYPE_INT && tleft == TYPE_REAL)  _pf.D2I();
	node->right()->accept(this, 0);
	if 		(tnode == TYPE_REAL && tright == TYPE_INT) _pf.I2D();
	else if (tnode == TYPE_INT && tright == TYPE_REAL) _pf.D2I();
}
void PFwriter::loadLogicOperands(cdk::node::expression::BinaryExpression *node,int lvl){
	type tleft 	= node->left()->type()->name();
	type tright = node->right()->type()->name();
	bool real = (tleft | tright) & TYPE_REAL;
	
	node->left()->accept(this, 0);
	if 	(real && tleft == TYPE_INT)  _pf.I2D();
	node->right()->accept(this, 0);
	if 	(real && tright == TYPE_INT)  _pf.I2D();
	if 	(real){
		_pf.DCMP();   // puts i on top of the stack, where i (op) 0 if d1 (op) d2
		_pf.INT(0);
    }
}
void PFwriter::loadBool(cdk::node::expression::Expression *node,int lvl){
	node->accept(this, 0);
	if(node->type()->name() == TYPE_REAL){
		_pf.INT(0);
		_pf.I2D();
		_pf.DCMP();
	}
	_pf.INT(0);
	_pf.NE();
}
//================================================================

//================================================================

void PFwriter::processProgramNode(pipoca::node::ProgramNode * const node, int lvl) {	
	try{
			
		node->statements()->accept(this, 0);
	
		_pf.TEXT();
		_pf.ALIGN();
		_pf.GLOBL("_main", _pf.FUNC());
		_pf.LABEL("_main");
		_pf.ENTER(0);
		
		_pfFunction.CALL("start",true);
	
		_pf.LEAVE();
		_pf.RET(); //return
		
		_pfFunction.INTERN();
		_pfFunction.EXTERN();
	} catch (PFerror e) { _error=1;e.out();}
}

//================================================================

void PFwriter::processSequence(cdk::node::Sequence * const node, int lvl) {
  try {
	  for (size_t i = 0; i < node->size(); i++) {
		node->node(i)->accept(this, 0);
	  }
  } catch (PFerror e) { throw e;}
}

void PFwriter::processBlockNode(pipoca::node::BlockNode * const node, int lvl){
	try {
		_symtab.push();
			if(node->decls() != NULL)
				node->decls()->accept(this, 0);
			node->instrs()->accept(this, 0);
		_symtab.pop();
	} catch (PFerror e) { _error=1;e.out();}
}

//================================================================
void PFwriter::processInstructionNode(pipoca::node::InstructionNode * const node, int lvl) {
	try {
		pipoca::semantics::TypeValidator tv(os(), _symtab);
		node->accept(&tv, lvl);
	} catch (PFerror e) { throw e;}
	
	node->expression()->accept(this, 0);
	_pf.TRASH(node->expression()->type()->totalSize());
	
}
void PFwriter::processCondition(pipoca::node::expression::Condition * const node, int lvl) {
	try {
		pipoca::semantics::TypeValidator tv(os(), _symtab);
		node->accept(&tv, lvl);
	} catch (PFerror e) {_error=1;e.out();}  
	node->expression()->accept(this, 0);
	if(node->type()->name() == TYPE_REAL){
		_pf.INT(0);
		_pf.I2D();
		_pf.DCMP();
	}
}
//================================================================

void PFwriter::processInteger(cdk::node::expression::Integer * const node,int lvl) {
  _pf.INT(node->value()); // push an integer
}

void PFwriter::processDouble(cdk::node::expression::Double * const node,int lvl) {
  int lbl1;

  _pf.RODATA();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  _pf.DOUBLE(node->value());

  _pf.TEXT();
  _pf.ADDR(mklbl(lbl1));
  _pf.LOAD2();
}
void PFwriter::processString(cdk::node::expression::String * const node, int lvl) {
   int lbl1;
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  
  _pf.STR(node->value()); // output string characters
  
  if(_cv.getFunction()){
	  _pf.TEXT(); 				// return to the TEXT segment
	  _pf.ADDR(mklbl(lbl1)); 	// the string to be printed
  }
}

void PFwriter::processVector(pipoca::node::expression::Vector * const node, int lvl){
	type t = node->type()->name();
	
	int size = node->type()->size();
	if(_cv.getFunction()){
		for (int i = node->literals()->size()-1; i >=0 ; i--) {
			cdk::node::expression::Expression * expr = (cdk::node::expression::Expression *)node->literals()->node(i);

			expr->accept(this, 0);
			if(t==TYPE_INT && expr->type()->name() == TYPE_REAL){
				_pf.I2D();
			}
			if(lvl){
				_pf.LOCAL(lvl+i*size);
				if( size == 4)   _pf.STORE();
				else if (size == 8)  _pf.STORE2();
			}
		}
	} else {
		for (size_t i = 0; i < node->literals()->size(); i++) {
			cdk::node::expression::Expression * expr = (cdk::node::expression::Expression *)node->literals()->node(i);
			if(t==TYPE_INT){
				_pf.CONST(((cdk::node::expression::Simple<int>*)expr)->value());
			} else if(t==TYPE_REAL){
				if(expr->type()->name() == TYPE_INT)
					_pf.DOUBLE((double)((cdk::node::expression::Simple<int>*)expr)->value());
				else if(expr->type()->name() == TYPE_REAL)
					_pf.DOUBLE(((cdk::node::expression::Simple<double>*)expr)->value());
			}		
		}
	}
	
}



//================================================================

void PFwriter::processIdentifier(cdk::node::expression::Identifier * const node,int lvl) {
	
	std::string id = node->value();
	Symbol* sym = _symtab.find(id.c_str());
	

	if(sym->type()->all()&TYPE_CONST && sym->type()->all()&TYPE_INT){
		_pf.INT(sym->offset());
		return;
	}
	
	if(sym->type()->all()&TYPE_VECTOR){
		int size = sym->type()->size();
		for(int i = sym->type()->length(); 0<=i ;i--){
			if (sym->offset()==0) 	_pf.ADDR(sym->name());
			else 					_pf.LOCAL(sym->offset());
			_pf.INT(i*size);
			_pf.ADD();
			if( size == 4)			_pf.LOAD();
			else if (size == 8)		_pf.LOAD2();
		}
		return;
	}
	
	loadSymbol(sym);

}
void PFwriter::processAttribution(pipoca::node::expression::Attribution * const node, int lvl) {

	char const*id =  node->lvalue()->value().c_str();
	Symbol* sym = _symtab.find(id);

	node->rvalue()->accept(this, 0); // determine the new value
	if(node->type()->name() == TYPE_INT && node->rvalue()->type()->name() == TYPE_REAL)
		_pf.D2I();
	if(node->type()->name() == TYPE_REAL && node->rvalue()->type()->name() == TYPE_INT)
		_pf.I2D();
	
	storeSymbol(sym);
	node->lvalue()->accept(this, 0); // loads the new value
}
void PFwriter::processAttributionIndex(pipoca::node::expression::AttributionIndex * const node, int lvl){}
void PFwriter::processIndex(pipoca::node::expression::Index * const node, int lvl){
	const char* id = node->identifier()->value().c_str();
	Symbol* sym = _symtab.find(id);
	
	if (sym->offset()==0) 	_pf.ADDR(sym->name());
	else 					_pf.LOCAL(sym->offset());
	
	if(sym->type()->all() & TYPE_POINTER)
		_pf.LOAD();

	int size = node->type()->size();
	_pf.INT(size);
	node->value()->accept(this, 0);
	_pf.MUL();
	_pf.ADD();
	if( size == 4)			_pf.LOAD();
	else if (size == 8)		_pf.LOAD2();
	
}


//================================================================
void PFwriter::processBrackets(pipoca::node::expression::Brackets * const node, int lvl){
	node->expr()->accept(this, 0);
}
//================================================================
void PFwriter::processNEG(cdk::node::expression::NEG * const node, int lvl) {
  node->argument()->accept(this, 0); // determine the value
  if (node->type()->name() == TYPE_INT)
    _pf.NEG(); // 2-complement
  else if (node->type()->name() == TYPE_REAL)
    _pf.DNEG();
}
//================================================================
void PFwriter::processADD(cdk::node::expression::ADD * const node, int lvl) {
	if(node->type()->name() & TYPE_STRING){	
		type tleft 	= node->left()->type()->name();
		type tright = node->right()->type()->name();
		int newStr= ++_lbl;
		
		_pf.BSS(); 
		_pf.LABEL(mklbl(newStr));	
		_pf.ALIGN();
		_pf.TEXT();
		
		_pf.ADDR(mklbl(newStr));
		if(tleft==TYPE_REAL){
			_pf.INT(5);
			node->left()->accept(this, 0);
			_pfFunction.CALL("dtoa");_pf.TRASH(12);_pf.PUSH();
		}else{
			node->left()->accept(this, 0);
			if(tleft == TYPE_INT){_pfFunction.CALL("itoa");_pf.TRASH(4);_pf.PUSH();}
		}		
		_pfFunction.CALL("_copyStr");_pf.PUSH();
		
		if(tright==TYPE_REAL){
			_pf.INT(5);
			node->right()->accept(this, 0);
			_pfFunction.CALL("dtoa");_pf.TRASH(12);_pf.PUSH();
		}else{
			node->right()->accept(this, 0);
			if(tright == TYPE_INT){_pfFunction.CALL("itoa");_pf.TRASH(4);_pf.PUSH();}	
		}	
		_pfFunction.CALL("_copyStr");
		
		_pf.BSS();
		_pf.CHAR(0);
		_pf.TEXT();
		_pf.ADDR(mklbl(newStr));
	} else {
		loadBinaryOperands(node,lvl);
		if (node->type()->name() == TYPE_INT) 		_pf.ADD();
		else if (node->type()->name() == TYPE_REAL) _pf.DADD();
	}
}
void PFwriter::processSUB(cdk::node::expression::SUB * const node, int lvl) {
  	loadBinaryOperands(node,lvl);
	if (node->type()->name() == TYPE_INT) 		_pf.SUB();
	else if (node->type()->name() == TYPE_REAL) _pf.DSUB();
}
void PFwriter::processMUL(cdk::node::expression::MUL * const node, int lvl) {
  	loadBinaryOperands(node,lvl);
	if (node->type()->name() == TYPE_INT) 		_pf.MUL();
	else if (node->type()->name() == TYPE_REAL) _pf.DMUL();
}
void PFwriter::processDIV(cdk::node::expression::DIV * const node, int lvl) {
	loadBinaryOperands(node,lvl);
	if (node->type()->name() == TYPE_INT) 		_pf.DIV();
	else if (node->type()->name() == TYPE_REAL) _pf.DDIV();
}
void PFwriter::processMOD(cdk::node::expression::MOD * const node, int lvl) {
  	loadBinaryOperands(node,lvl);
	_pf.MOD();
}
//================================================================
void PFwriter::processLT(cdk::node::expression::LT * const node, int lvl) {
    loadLogicOperands(node,lvl);
    _pf.LT();     
}
void PFwriter::processLE(cdk::node::expression::LE * const node, int lvl) {
  loadLogicOperands(node,lvl);
  _pf.LE();
}
void PFwriter::processGE(cdk::node::expression::GE * const node, int lvl) {
  loadLogicOperands(node,lvl);
  _pf.GE();  
}
void PFwriter::processGT(cdk::node::expression::GT * const node, int lvl) {
  loadLogicOperands(node,lvl);
  _pf.GT();
}
void PFwriter::processNE(cdk::node::expression::NE * const node, int lvl) {
  loadLogicOperands(node,lvl);
  _pf.NE();
}
void PFwriter::processEQ(cdk::node::expression::EQ * const node, int lvl) {
  loadLogicOperands(node,lvl);
  _pf.EQ();     
}
//================================================================

void PFwriter::processPrint(pipoca::node::expression::Print * const node, int lvl) {
  node->expr()->accept(this, 0); // determine the value to print
  type t = node->expr()->type()->name();
  if(t == TYPE_INT)			_pfFunction.CALL("printi"); 
  else if (t== TYPE_REAL) 	_pfFunction.CALL("printd");
  else if (t== TYPE_STRING) _pfFunction.CALL("prints");
  
  if(node->newline()) _pfFunction.CALL("println"); // print a newline	
  _pf.TRASH(node->expr()->type()->size());
  _pf.INT(0);
}
void PFwriter::processRead(pipoca::node::expression::Read * const node, int lvl) {
  char const*id = node->identifier()->value().c_str();
  Symbol* sym = _symtab.find(id);
  type t = sym->type()->name(); 
  size_t size = sym->type()->totalSize();
  if(t == TYPE_INT)			_pfFunction.CALL("readi"); 
  else if (t== TYPE_REAL) 	_pfFunction.CALL("readd");
  else throw;

  if (size == 4) 		_pf.PUSH(); 	// store the value at address
  else if ( size == 8 ) _pf.DPUSH();
  else throw;
  
  storeSymbol(sym);	
  loadSymbol(sym);
}

//================================================================

void PFwriter::processIfElseNode(pipoca::node::IfElseNode * const node,  int lvl) {
	
	int  ifelse = ++_lbl,ifend = ++_lbl;
	node->condition()->accept(this, 0);
	_pf.JZ(mklbl(ifelse));
	node->thenblock()->accept(this, 0);
	if(node->elseblock()){
		_pf.JMP(mklbl(ifend));
		_pf.LABEL(mklbl(ifelse));
		node->elseblock()->accept(this, 0);
		_pf.LABEL(mklbl(ifend));
	} else
		_pf.LABEL(mklbl(ifelse));
	
}

void PFwriter::processWhileElseNode(pipoca::node::WhileElseNode * const node, int lvl) {
  int whilethen = ++_lbl, whileend = ++_lbl, whilecond = ++_lbl;
  
  _cycleBegin.push_back(whilecond);
  _cycleEnd.push_back(whileend);
  
  if(node->elseblock()){
	  node->condition()->accept(this, 0);
	  _pf.JNZ(mklbl(whilethen));			//first time if cond true we want to go to then block
	  node->elseblock()->accept(this, 0); //first time if cond false we want to go to else block
	  _pf.JMP(mklbl(whileend));
  }
  
  _pf.LABEL(mklbl(whilethen));	
  node->thenblock()->accept(this, 0);	

  _cycleBegin.pop_back();
  _cycleEnd.pop_back();
  _pf.LABEL(mklbl(whilecond));
  node->condition()->accept(this, 0);

  _pf.JNZ(mklbl(whilethen));			//once  in then block we don't want to go to else block

  _pf.LABEL(mklbl(whileend));
  
}
void PFwriter::processDoElseNode(pipoca::node::DoElseNode * const node, int lvl) {
  int dothen = ++_lbl, doend = ++_lbl;

  if(node->elseblock()){
      _cycleBegin.push_back(dothen);
	  _cycleEnd.push_back(doend);
		  node->thenblock()->accept(this, 0);
		  node->condition()->accept(this, 0);
	  _cycleBegin.pop_back();
	  _cycleEnd.pop_back();
	  _pf.JNZ(mklbl(dothen));			//first time if cond true we want to go to then block
	  node->elseblock()->accept(this, 0); //first time if cond false we want to go to else block
	  _pf.JMP(mklbl(doend));
  }
  _cycleBegin.push_back(dothen);
  _cycleEnd.push_back(doend);
	  _pf.LABEL(mklbl(dothen));	
	   node->thenblock()->accept(this, 0);  
	  node->condition()->accept(this, 0);
	  _pf.JZ(mklbl(dothen));	

	  _pf.LABEL(mklbl(doend));  
  _cycleBegin.pop_back();
  _cycleEnd.pop_back();

}

//================================================================

void PFwriter::processPOW(pipoca::node::expression::POW * const node, int lvl) {
	// FIXME: how should negative integers be dealt with?
	node->left()->accept(this, 0);	//base
	node->right()->accept(this, 0);	//exp
	_pfFunction.CALL("_intpow");
	_pf.PUSH();
}

void PFwriter::processOR(pipoca::node::expression::OR * const node, int lvl){
	int end = ++_lbl,endtrue = ++_lbl;
	loadBool(node->left(),lvl);
	_pf.JNZ(mklbl(endtrue));
	loadBool(node->right(),lvl);
	_pf.JNZ(mklbl(endtrue));
	_pf.INT(0);
	_pf.JMP(mklbl(end));
	_pf.LABEL(mklbl(endtrue));
	_pf.INT(1);
	_pf.LABEL(mklbl(end));
}
void PFwriter::processAND(pipoca::node::expression::AND * const node, int lvl){
	int end = ++_lbl,endfalse = ++_lbl;
	loadBool(node->left(),lvl);
	_pf.JZ(mklbl(endfalse));
	loadBool(node->right(),lvl);
	_pf.JZ(mklbl(endfalse));
	_pf.INT(1);
	_pf.JMP(mklbl(end));
	_pf.LABEL(mklbl(endfalse));
	_pf.INT(0);
	_pf.LABEL(mklbl(end));
}
void PFwriter::processNOT(pipoca::node::expression::NOT * const node, int lvl){
	loadBool(node->argument(),lvl);
	_pf.INT(1);
	_pf.XOR();
}

//================================================================


void PFwriter::processINC(pipoca::node::expression::INC * const node, int lvl){
	cdk::node::expression::Identifier * identifier = (cdk::node::expression::Identifier *)node->argument();
	const char* id = identifier->value().c_str();
	Symbol* sym = _symtab.find(id);
	int value = (sym->type()->all()&TYPE_POINTER)? sym->type()->size() : 1;
	loadSymbol(sym);
	if(node->suffix()) loadSymbol(sym);
	_pf.SP();
	_pf.INCR(value);
	storeSymbol(sym);
	if(!node->suffix()) loadSymbol(sym);
}
void PFwriter::processDEC(pipoca::node::expression::DEC * const node, int lvl){
	cdk::node::expression::Identifier * identifier = (cdk::node::expression::Identifier *)node->argument();
	const char* id = identifier->value().c_str();
	Symbol* sym = _symtab.find(id);
	int value = (sym->type()->all()&TYPE_POINTER)? sym->type()->size() : 1;
	loadSymbol(sym);
	if(node->suffix()) loadSymbol(sym);
	_pf.SP();
	_pf.DECR(value);
	storeSymbol(sym);
	if(!node->suffix()) loadSymbol(sym);
}
	    
void PFwriter::processAddress(pipoca::node::expression::Address * const node, int lvl){
	
	const char* id = node->identifier()->value().c_str();
	Symbol* sym = _symtab.find(id);
	if (sym->offset()==0) 	{
		_pf.ADDR(sym->name());
	}else{
		_pf.LOCAL(sym->offset());
	}
}

//================================================================


void PFwriter::processVarConstNode(pipoca::node::VarConstNode * const node, int lvl){
	try{
		node->accept(&_cv, lvl);	
	} catch (PFerror e) { throw e;}	

	cdk::node::Sequence *identifiers = node->identifiers();
	if(identifiers){	
			
		for( size_t i = 0; i < identifiers->size(); i++){
			cdk::node::expression::Identifier *n = (cdk::node::expression::Identifier *)identifiers->node(i);
			if(n==NULL) break;
			if((n->type()->name() & TYPE_INT) && !(n->type()->all() & (TYPE_VECTOR|TYPE_POINTER))) continue;
			const char* id = n->value().c_str();
			Symbol* sym = _symtab.find(id);
			
			if (sym->offset()==0){				
				_pf.RODATA();
				_pf.ALIGN();
				_pf.GLOBL(sym->name().c_str(), _pf.OBJ());
				_pf.LABEL(sym->name().c_str());
				
				if( sym->type()->all() & TYPE_VECTOR){
					node->literals()->node(i)->accept(this, sym->offset());
				}else if( sym->type()->name() & TYPE_REAL){
					cdk::node::expression::Simple<double>* literal  = (cdk::node::expression::Simple<double>*) node->literals()->node(i);	
					_pf.DOUBLE(literal->value());					
				}else if( sym->type()->name() & TYPE_STRING){
					cdk::node::expression::Simple<std::string>* literal  = (cdk::node::expression::Simple<std::string>*) node->literals()->node(i);
					int lbl1= ++_lbl;
					_pf.ID(mklbl(lbl1));
					_pf.ALIGN();
					_pf.LABEL(mklbl(lbl1));    // give the string a name
					_pf.STR(literal->value()); // output string characters	
				}
				_pf.TEXT();
			} else {
			
				node->literals()->node(i)->accept(this,lvl);
				storeSymbol(sym);
			}			
		}
	}	
}
void PFwriter::processVarNode(pipoca::node::VarNode * const node, int lvl){
	try{
		node->accept(&_cv, lvl);	
	} catch (PFerror e) { throw e;}	
	
	if(_cv.offset() == 0){				
		_pf.BSS();
		_pf.ALIGN();
		cdk::node::Sequence *indexLst = node->indexLst();
		for( size_t i = 0; i < indexLst->size(); i++){
			pipoca::node::expression::Index *index = ((pipoca::node::expression::Index *) indexLst->node(i));
			cdk::node::expression::Identifier *n = (cdk::node::expression::Identifier *) index->identifier();
			if(n==NULL) break;
			const char* id = n->value().c_str();
			_pf.GLOBL(id, _pf.OBJ());
			_pf.LABEL(id);
			_pf.BYTE(node->type()->totalSize());
		}
	} 		
}
void PFwriter::processVarInitNode(pipoca::node::VarInitNode * const node, int lvl){
	try{
		node->accept(&_cv, lvl);	
	} catch (PFerror e) { throw e;}	
	
	cdk::node::expression::Identifier* identifier = node->index()->identifier();
	cdk::node::expression::Expression* expr       = node->expr();
	const char* id = identifier->value().c_str();
	Symbol* sym = _symtab.find(id);
	if(_cv.offset() == 0){		
		_pf.DATA();
		_pf.GLOBL(id, _pf.OBJ());
		_pf.ALIGN();
		_pf.LABEL(id);

		if( sym->type()->all() & TYPE_VECTOR){	
			expr->accept(this, sym->offset());			
		} else if( sym->type()->name() & TYPE_INT){
			_pf.CONST(((cdk::node::expression::Simple<int>*)expr)->value());
		} else if( sym->type()->name() & TYPE_REAL){
			_pf.DOUBLE(((cdk::node::expression::Simple<double>*)expr)->value());
		} else if( sym->type()->name() & TYPE_STRING){
			int lbl1= ++_lbl;
			_pf.ID(mklbl(_lbl));
			_pf.ALIGN();
			_pf.LABEL(mklbl(lbl1));    
			_pf.STR(((cdk::node::expression::Simple<std::string>*)expr)->value());
		} 
		_pf.TEXT();
	} else {	
		if( sym->type()->all() & TYPE_VECTOR){
		    expr->accept(this,sym->offset());
		}else{	
			expr->accept(this,lvl);
			storeSymbol(sym);
		}
	} 	
}
void PFwriter::processVarExtNode(pipoca::node::VarExtNode * const node, int lvl){}

//================================================================

void PFwriter::processReturnNode(pipoca::node::ReturnNode * const node, int lvl){
	Function* f = _cv.getFunction();
	_pf.JMP(_pfFunction.mknameret(f->name()));
}


void PFwriter::processFunctionDeclNode(pipoca::node::FunctionDeclNode * const node, int lvl){
	try{
		node->accept(&_cv, lvl);	
	} catch (PFerror e) { throw e;}	
	_pfFunction.addProgExternFunction(_cv.getFunction());
	_cv.reset();
}


void PFwriter::processFunctionDefNode(pipoca::node::FunctionDefNode * const node, int lvl){
	Symbol* funcSym;
	try{
		pipoca::semantics::TypeValidator tv(os(), _symtab);
		node->accept(&tv, lvl);	
		_cv.init(node);		//tells to ContextValidator that we are IN a function context
		funcSym = _symtab.find(node->identifier()->value().c_str());
	} catch (PFerror e) { throw e;}	

	_symtab.push();
		try{			
			node->accept(&_cv, lvl);	
		} catch (PFerror e) { throw e;}
		
		std::string retId = node->identifier()->value();
		Symbol* retSym= _symtab.find(retId.c_str());
			
		_pf.TEXT();
		_pf.ALIGN();
		_pf.GLOBL(_pfFunction.mkname(funcSym->name()), _pf.FUNC());
		_pf.LABEL(_pfFunction.mkname(funcSym->name()));
		_pf.ENTER(_cv.offset());
		
		if(funcSym->type()->name() & TYPE_INT){
			_pf.INT(0);
			_pf.LOCA(retSym->offset());
		}
		
		node->block()->accept(this, 0);
		
		if(funcSym->name() == "start" && _error != 0){
			_pf.INT(_error);
			_pf.LOCA(retSym->offset());
		}
		
		_pf.LABEL(_pfFunction.mknameret(funcSym->name()));
		
		node->identifier()->accept(this, 0);		
		if (funcSym->type()->totalSize() == 4) {	 
			_pf.POP(); 	
		} else if ( funcSym->type()->totalSize() == 8 ) {
			_pf.DPOP();
		} else throw PFerror(node)<<"PFwriter::processFunctionDefNode: Symbol '"<<funcSym->name()<<"' of unhandled size"<<funcSym->type()->totalSize();	
	_symtab.pop();
	
	_pf.LEAVE();
	_pf.RET();

	_cv.reset();//tells to ContextValidator that we are OUT of a function context		
}

void PFwriter::processFunctionCall(pipoca::node::expression::FunctionCall * const node, int lvl){
	
	std::string functionId = node->identifier()->value();
	Symbol* funcSym= _symtab.find(functionId.c_str()); 
	
	cdk::node::Sequence *arguments = node->args();	
	for (int i = funcSym->function()->argNum()-1; i >= 0 ; i--) {
		pipoca::node::expression::Argument *n = (pipoca::node::expression::Argument *)arguments->node(i);
		if (n == NULL) break;
		n->accept(this, 0);
	}
		
	_pfFunction.CALL(funcSym->name(),true);	
	_pf.TRASH(funcSym->function()->argSize());
	if (funcSym->type()->totalSize() == 4) {	 
		_pf.PUSH(); 	
	} else if ( funcSym->type()->totalSize() == 8 ) {
		_pf.DPUSH();
	} else PFerror(node)<<"PFwriter::processFunctionCall: Symbol '"<<funcSym->name()<<"' of unhandled size"<<funcSym->type()->totalSize();	
}	


void PFwriter::processContinueNode(pipoca::node::ContinueNode * const node, int lvl){
	int index = ((cdk::node::expression::Simple<int>*)node->value())->value();
	if(index>(int)_cycleBegin.size())
		throw PFerror(node)<<"PFwriter::processContinueNode: continue to inexistent cycle";
	
	_pf.JMP(mklbl(_cycleBegin[_cycleBegin.size()-index]));
}
void PFwriter::processBreakNode(pipoca::node::BreakNode * const node, int lvl){
	int index = ((cdk::node::expression::Simple<int>*)node->value())->value();
	if(index>(int)_cycleEnd.size())
		throw PFerror(node)<<"PFwriter::processBreakNode: break to inexistent cycle"<<index;
	
	_pf.JMP(mklbl(_cycleEnd[_cycleEnd.size()-index]));
}


void PFwriter::processMemoryAllocationNode(pipoca::node::MemoryAllocationNode * const node, int lvl) {
	try{
		pipoca::semantics::TypeValidator tv(os(), _symtab);
		node->accept(&tv, lvl);
	} catch (PFerror e) { throw e;}	
	  
	char const *id = node->identifier()->value().c_str();
	Symbol *sym = _symtab.find(id);

	if(sym->offset()){
		node->expression()->accept(this, lvl);
		_pf.SP();
		_pf.ALLOC();
	}else{
		int init = ++_lbl, end = ++_lbl;
		node->expression()->accept(this, lvl);
		_pf.LABEL(mklbl(init));
		_pf.JZ(mklbl(end));
		_pf.BSS();
		_pf.BYTE(1);
		_pf.TEXT();
		_pf.INT(1);
		_pf.SUB();
		_pf.JMP(mklbl(init));
		_pf.LABEL(mklbl(end));
	}
	storeSymbol(sym);
	
}

//---------------------------------------------------------------------------
//     T H E    E N D
//---------------------------------------------------------------------------
//
// vim:ts=2:sw=2:et:
