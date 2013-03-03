#ifndef __PIPOCA_BRACKETS_H__
#define __PIPOCA_BRACKETS_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression{

    
		class Brackets: public cdk::node::expression::Expression {
			cdk::node::expression::Expression * _expr;

			public:
			  inline Brackets(int lineno, cdk::node::expression::Expression * expr):
				cdk::node::expression::Expression(lineno), _expr(expr) {
			  }

			  inline cdk::node::expression::Expression * expr() {
				return _expr;
			  }
			  
			  const char *name() {
				return "Print";
			  }

			  void accept(SemanticProcessor *sp, int level) {
				sp->processBrackets(this, level);
			  }

		};
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
