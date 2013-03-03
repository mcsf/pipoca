#ifndef __PIPOCA_EXPRESSION_ATTRIBUTION_H__
#define __PIPOCA_EXPRESSION_ATTRIBUTION_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression {

		//!
		//! Class for describing attribution nodes.
		//!
		class Attribution: public cdk::node::expression::Expression {
		  cdk::node::expression::Identifier *_lvalue;
		  cdk::node::expression::Expression *_rvalue;
		  

		public:
		  inline Attribution(int lineno, cdk::node::expression::Identifier *lvalue,
								cdk::node::expression::Expression *rvalue) :
			cdk::node::expression::Expression(lineno), _lvalue(lvalue), _rvalue(rvalue) {
		  }

		public:
		  inline cdk::node::expression::Identifier *lvalue() {
			return _lvalue;
		  }
		  inline cdk::node::expression::Expression *rvalue() {
			return _rvalue;
		  }

		public:
		  const char *name() {
			return "Attribution";
		  }

		  void accept(SemanticProcessor *sp, int level) {
			 sp->processAttribution(this, level);
		  }

		}; // class
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
