
#ifndef __PIPOCA_SEMANTICS_PFFUNCTION_H__
#define __PIPOCA_SEMANTICS_PFFUNCTION_H__

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <cdk/SymbolTable.h>
#include <cdk/generators/Postfix.h>
#include "semantics/SemanticProcessor.h"
#include "semantics/Symbol.h"

#include "semantics/ExpressionType.h"
#include "semantics/PFerror.h"
#include "semantics/Function.h"


namespace pipoca {
  namespace semantics {
	
	
    class PFfunction {
	  cdk::SymbolTable<Symbol> &_symtab;
	  cdk::generator::Postfix &_pf;
	  map<std::string,pipoca::semantics::Function*>	_externFunctions;
	  list<std::string>							_externCalled;
	  list<std::string>							_internFunctions;
	  list<std::string>							_internCalled;
	  list<std::string>							_progCalled;
	  int _lbl;
	  void init();
	  void addExternFunction(Function* f,bool progCanCall = false);
	  
	  bool find ( list<std::string> l, std::string value ){
		list<std::string>::iterator it;
		for ( it=l.begin() ; it != l.end(); it++ ){
			if ( (*it)==value ) return true;
		}
		return false;
      }
	  std::string mklbl(int lbl) {
			std::ostringstream oss;
			if (lbl < 0)
				oss << "..L" << -lbl;
			else
				oss << "__L" << lbl;
			return oss.str();
		}
	 public:
	  
	  PFfunction( cdk::SymbolTable<Symbol> &symtab,cdk::generator::Postfix &pf) :
         _symtab(symtab),_pf(pf),_lbl(0) {
		 init();
      }
	  ~PFfunction() {
      }
	 
	std::string mkname(std::string name);
	std::string mknameret(std::string name);
	  	
	void addProgExternFunction(Function* f);
	void CALL(std::string f,bool prog = false);
	void EXTERN();
	void INTERN();

    }; // class

  } // namespace semantics
} // namespace pipoca

#endif
