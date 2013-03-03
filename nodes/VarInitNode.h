#ifndef __PIPOCA_VARINITNODE_H__
#define __PIPOCA_VARINITNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    
    class VarInitNode: public cdk::node::Node {
		  pipoca::node::expression::Index* _index;
		  cdk::node::expression::Expression* _expr;

		public:
		  inline VarInitNode(int lineno, pipoca::node::expression::Index* index, cdk::node::expression::Expression * expr) :
			cdk::node::Node(lineno), _index(index), _expr(expr) {
		  }

		  inline pipoca::node::expression::Index* index() {
			return _index;
		  }
		  
		  inline cdk::node::expression::Expression* expr() {
			return _expr;
		  }

		  const char *name() {
			return "VarInitNode";
		  }

		  void accept(SemanticProcessor *sp, int level) {
			sp->processVarInitNode(this, level);
		  }
		  
    }; // class
	
  } // namespace node
} // namespace pipoca

#endif
