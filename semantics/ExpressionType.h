// $Id: ExpressionType.h,v 1.14 2010-05-30 22:39:16 ist162510 Exp $ -*- c++ -*-
/*
 * $Log: ExpressionType.h,v $
 * Revision 1.14  2010-05-30 22:39:16  ist162510
 * Pointers
 *
 * Revision 1.13  2010/05/29 01:46:01  ist162510
 * vector implemented in functions
 *
 * Revision 1.12  2010/05/28 21:35:41  ist162510
 * vectors in progress
 * du until, continues, fixed
 *
 * Revision 1.11  2010/05/27 05:58:37  ist162510
 * return,continue,break, vectors by value
 *
 * Revision 1.10  2010/05/25 22:15:28  ist162510
 * vectors implemented
 *
 * Revision 1.9  2010/05/21 03:16:56  ist162510
 * Function call, return, args fully functional.
 * Const variables implemented.
 *
 * Revision 1.8  2010/05/18 19:51:44  ist162510
 * Removed WhileNode IfNode DoNode
 * Function return value, arguments fully functional
 * Print and Read is now working
 *
 * Revision 1.7  2010/05/17 22:15:29  ist162510
 * all nodes names chaged to shorter version
 * New ContextValidator to manage all declarations
 * New InstructionNode to manage expression as instruction
 *
 * Revision 1.6  2010/05/14 10:14:13  ist162510
 * ExpressinType Esier to use
 *
 * Revision 1.5  2010/05/14 00:43:49  ist162510
 * *** empty log message ***
 *
 * Revision 1.4  2010/05/13 23:31:42  ist162510
 * TypeValidator files added
 * .y confilct solved
 * MemoryAllocationNode in semantic processor
 *
 * Revision 1.3  2010/05/13 00:08:15  ist162510
 * YEAAH!!!
 *
 * Revision 1.2  2010/05/12 22:08:01  ist162462
 * Oops.
 *
 * Revision 1.1  2010-05-12 22:03:58  ist162462
 * Add own ExpressionType class in semantics
 *
 * Revision 1.1  2010/03/22 14:55:04  david
 * Expression types are now in the semantics directory but are not part of
 * the semantics package. This is because we defined them to be
 * drop-in-replaceable. That is, if the compiler provides, in its semantics
 * directory, an ExpressionType.h file (this file must contain a class called
 * ExpressionType), then that class will be used instead of the one
 * provided by the CDK.
 *
 *
 */

#ifndef __PIPOCA_SEMANTICS_EXPRESSIONTYPE_H__
#define __PIPOCA_SEMANTICS_EXPRESSIONTYPE_H__

#include <cstdlib>
#include <sstream>
#include <string>

/**
 * This is a quick and very dirty approach to type information.
 * It is defined this way (even though it's not extensible at all)
 * for simplicity.
 *
 * Nevertheless, new types can be added simply by using other
 * integer values other than the ones listed.
 */
 enum type { 	TYPE_ERROR	= 0,
				TYPE_INT 	= 1UL<<0,
				TYPE_REAL	= 1UL<<1,
				TYPE_STRING	= 1UL<<2,
				TYPE_VECTOR	= 1UL<<3,
				TYPE_CONST	= 1UL<<4,
				TYPE_POINTER= 1UL<<5,
			};
 

 
class ExpressionType {
  size_t _size;
  size_t _length;
  type  _name;
  int   _more;
 public:
  static int sizeOf(type t){
	if(t & TYPE_REAL) return 8;
	else if(t & TYPE_INT) return 4;
	else if(t & TYPE_STRING) return 4; 
	return 0;
  }
 
  static std::string str(int t){
	std::stringstream err;
	if(t == TYPE_ERROR) err<<"ERROR";
	else {
		if(t & TYPE_CONST) err<<"CONST ";
		if(t & TYPE_VECTOR) err<<"VECTOR ";
		if(t & TYPE_POINTER) err<<"POINTER ";
		if(t & TYPE_REAL) err<<"REAL";
		else if(t & TYPE_INT) err<<"INTEGER";
		else if(t & TYPE_STRING) err<<"STRING";	
	}
	return err.str();
  }
 
  inline ExpressionType() :
    _size(0),_length(1), _name(TYPE_ERROR) {
  }
  inline ExpressionType(size_t size, type name) : _size(size), _length(1), _name(name), _more(0){}
  
  inline ExpressionType(int name,size_t length) : _length(length) {
	init(name);
  }
  
  inline ExpressionType(type name) :   _length(1),_name(name), _more(0){
	_size = sizeOf(_name);
  }
  
  inline ExpressionType(int name): _length(1){ 
	init(name);
  }
  
  inline void init(int name){
	_name = (type)(name & (TYPE_ERROR|TYPE_REAL|TYPE_INT|TYPE_STRING));
	
	_more = name & ~(_name);
	_size = sizeOf(_name);
  }
    
  inline int totalSize() {return _size*length();}
  inline int size() {return _size;}
  inline int length() {return (_length>0)?_length:1;}

  std::string str(){return str(_name|_more);}
  
  inline type name() {return _name;}
  inline int all()  {return _name|_more;}
};

#endif
