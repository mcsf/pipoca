// $Id: PFevaluator.cpp,v 1.6 2010-05-27 23:21:53 ist162510 Exp $
/*
 * $Log: PFevaluator.cpp,v $
 * Revision 1.6  2010-05-27 23:21:53  ist162510
 * Continue,return,break,inc,dec
 *
 * Revision 1.5  2010/05/23 14:24:55  ist162510
 * implemented run-time addition for strings, with doubles and integers
 * Init variables implemented.
 *
 * Revision 1.4  2010/05/18 19:51:44  ist162510
 * Removed WhileNode IfNode DoNode
 * Function return value, arguments fully functional
 * Print and Read is now working
 *
 * Revision 1.3  2010/05/17 22:15:29  ist162510
 * all nodes names chaged to shorter version
 * New ContextValidator to manage all declarations
 * New InstructionNode to manage expression as instruction
 *
 * Revision 1.2  2010/05/15 18:11:28  ist162510
 * Lots of stuff
 *
 * Revision 1.1  2010/03/27 17:00:15  ist162462
 * Initial commit.
 *
 * Revision 1.4  2009/03/15 19:21:15  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.3  2009/03/02 17:40:21  david
 * Major changes: in addition to compiling with CDK4, Pipoca now has
 * its own namespace (pipoca). All classes are defined in it or in its
 * child namespaces. Added automatic node processing: the "nodes"
 * directory is scanned and nodes/all.h is built (contains both forward
 * declarations and include directives for all nodes -- in the appropriate
 * namespaces).
 *
 * Revision 1.1  2007/03/14 01:20:28  david
 * Changed compiler implementation to use evaluation strategy objects
 * instead of functions. These strategies are selected according to the
 * target selected from the command line options or from clues such as
 * the output file extension.
 *
 */

//---------------------------------------------------------------------------

#include <cdk/nodes/Node.h>
#include <cdk/SymbolTable.h>
#include <cdk/Compiler.h>
#include <cdk/generators/ix86.h>
#include "semantics/PFevaluator.h"
#include "semantics/PFwriter.h"
#include "semantics/Symbol.h"

//---------------------------------------------------------------------------

/**
 * Postfix for ix86.
 * @var create and register an evaluator for ASM targets.
 */
pipoca::semantics::PFevaluator pipoca::semantics::PFevaluator::_thisEvaluator("asm");

//---------------------------------------------------------------------------

bool pipoca::semantics::PFevaluator::evaluate(cdk::Compiler *compiler) {
  bool debug = false; // HACK! (should be passed as arg)

  std::ostream &os = compiler->ostream();

  // this symbol table will be used to check identifiers
  cdk::SymbolTable<Symbol> symtab;
  cdk::generator::ix86 pf(os, debug);


  // now the rest of the nasm code will be generated
  // identifiers will be checked during code generation
  //
  PFwriter pfwriter(os, symtab, pf);
  compiler->syntax()->accept(&pfwriter, 0);

  return true;
}

//---------------------------------------------------------------------------
//     T H E    E N D
//---------------------------------------------------------------------------
