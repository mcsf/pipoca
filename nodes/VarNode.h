#ifndef __PIPOCA_VARNODE_H__
#define __PIPOCA_VARNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    
    class VarNode: public cdk::node::Node {
		  cdk::node::Sequence * _indexLst;
		  ExpressionType      * _type;

		public:
		  inline VarNode(int lineno, ExpressionType* type, cdk::node::Sequence * indexLst):
			cdk::node::Node(lineno), _indexLst(indexLst), _type(type) {
		  }

		  inline cdk::node::Sequence* indexLst() {return _indexLst;}		  
		  inline ExpressionType* type() {return _type;}

		  const char *name() {
			return "VarNode";
		  }

		  void accept(SemanticProcessor *sp, int level) {
			sp->processVarNode(this, level);
		  }
		  
    }; // class
	
  } // namespace node
} // namespace pipoca

#endif
