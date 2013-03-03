#ifndef __PIPOCA_NODEEXCEPTION_H__
#define __PIPOCA_NODEEXCEPTION_H__

#include <stdlib.h>
#include <sstream>
#include <string>
#include <exception>

#include <cdk/nodes/Node.h>


using namespace std;


namespace pipoca {
  namespace semantics {
  
	class PFerror : public exception {

		const  cdk::node::Node* _node;
		std::string _message;
	public:
	    PFerror() throw() : exception(), _node(NULL), _message("") {}
		PFerror(const cdk::node::Node* node) throw() : exception(), _node(node), _message("") {}
		~PFerror() throw(){}

		void out() throw();
		
		virtual const char* what() const throw();
		
		PFerror& operator<<(const char* str) throw();
		PFerror& operator<<(std::string str) throw();
		PFerror& operator<<(int i) throw();
		
	};
	
 }
}

#endif