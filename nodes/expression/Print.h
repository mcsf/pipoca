#ifndef __PIPOCA_PRINT_H__
#define __PIPOCA_PRINT_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression{

    
		class Print: public cdk::node::expression::Expression {
			cdk::node::expression::Expression * _expr;
			bool _newline;

			public:
			  inline Print(int lineno, cdk::node::expression::Expression * expr, bool newline):
				cdk::node::expression::Expression(lineno), _expr(expr), _newline(newline) {
			  }

			  inline cdk::node::expression::Expression * expr() {
				return _expr;
			  }
			  
			  inline bool newline() {
				return _newline;
			  }
			  
			  const char *name() {
				return "Print";
			  }

			  void accept(SemanticProcessor *sp, int level) {
				sp->processPrint(this, level);
			  }

		};
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
