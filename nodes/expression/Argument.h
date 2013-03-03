#ifndef __PIPOCA_ARGUMENT_H__
#define __PIPOCA_ARGUMENT_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression{

		class Argument: public cdk::node::expression::Expression {
			  pipoca::node::expression::Index   * _index;
			  

			public:
			  inline Argument(int lineno, ExpressionType *etype, pipoca::node::expression::Index   * index):
				cdk::node::expression::Expression(lineno), _index(index)
				{
					type(etype);
				}

			  inline pipoca::node::expression::Index   * index() {return _index;}
			  
			  const char *name() {
				return "Argument";
			  }

			  void accept(SemanticProcessor *sp, int level) {
				sp->processArgument(this, level);
			  }
			  
		}; // class
		
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
