// $Id: Symbol.h,v 1.7 2010-05-24 22:35:29 ist162510 Exp $ -*- c++ -*-
/*
 * $Log: Symbol.h,v $
 * Revision 1.7  2010-05-24 22:35:29  ist162510
 * New nodes Brackets, condition, some bugs fixed.
 *
 * Revision 1.6  2010/05/24 02:11:16  ist162510
 * PFfunction done, it loads every extern or intern resource only when it is used.
 *
 * Revision 1.5  2010/05/23 14:24:55  ist162510
 * implemented run-time addition for strings, with doubles and integers
 * Init variables implemented.
 *
 * Revision 1.4  2010/05/18 19:51:45  ist162510
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
 * Revision 1.1  2010/03/27 17:00:16  ist162462
 * Initial commit.
 *
 * Revision 1.1  2009/03/02 17:40:21  david
 * Major changes: in addition to compiling with CDK4, Pipoca now has
 * its own namespace (pipoca). All classes are defined in it or in its
 * child namespaces. Added automatic node processing: the "nodes"
 * directory is scanned and nodes/all.h is built (contains both forward
 * declarations and include directives for all nodes -- in the appropriate
 * namespaces).
 *
 * Revision 1.1  2009/03/01 20:40:34  david
 * Class Symbol is now a part of Pipoca.
 *
 *
 */
#ifndef __PIPOCA_SEMANTICS_SYMBOL_H__
#define __PIPOCA_SEMANTICS_SYMBOL_H__

#include <string>
#include "semantics/ExpressionType.h"
#include "semantics/Function.h"

class Symbol {
  ExpressionType *_type;
  std::string _name;
  int _offset;
  pipoca::semantics::Function* _function;
  
public:
  inline Symbol(ExpressionType *type, std::string name,int offset = 0) :
    _type(type), _name(name), _offset(offset), _function(NULL) {
	}

  inline Symbol(pipoca::semantics::Function* function,int offset = 0) :
     _offset(offset), _function(function) 
	{
		_type = function->type();
		_name = function->name();
	}
  inline Symbol(pipoca::semantics::Function::Argument* arg) 
	{
		_type = arg->type();
		_name = arg->name();
		_offset = arg->offset();
	}

  virtual ~Symbol() {
  }
  
  inline ExpressionType *type() const {
    return _type;
  }
  inline const std::string &name() const {
	return _name;
  }
  inline int offset() const {
    return _offset;
  }
  inline pipoca::semantics::Function* function() const {
    return _function;
  }
  
};



#endif
