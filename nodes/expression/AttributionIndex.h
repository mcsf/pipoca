#ifndef __PIPOCA_EXPRESSION_ATTRIBUTIONINDEX_H__
#define __PIPOCA_EXPRESSION_ATTRIBUTIONINDEX_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression {

		//!
		//! Class for describing attribution index nodes.
		//!
		class AttributionIndex: public cdk::node::expression::Expression {
		  cdk::node::expression::Expression *_lvalue, *_rvalue;
		  int _index;

		public:
		  inline AttributionIndex(int lineno, cdk::node::expression::Expression *lvalue, int index,
								cdk::node::expression::Expression *rvalue) :
			cdk::node::expression::Expression(lineno), _lvalue(lvalue), _rvalue(rvalue), _index(index) {
		  }

		  inline cdk::node::expression::Expression *lvalue() {
			return _lvalue;
		  }
		  inline int index() {
			return _index;
		  }
		  inline cdk::node::expression::Expression *rvalue() {
			return _rvalue;
		  }

		  const char *name() {
			return "AttributionIndex";
		  }

		  void accept(SemanticProcessor *sp, int level) {
			 sp->processAttributionIndex(this, level);
		  }

		}; // class
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
