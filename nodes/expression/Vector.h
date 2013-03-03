#ifndef __PIPOCA_VECTOR_H__
#define __PIPOCA_VECTOR_H__

#include <cdk/nodes/Sequence.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression {

		class Vector: public virtual cdk::node::expression::Expression {
			cdk::node::Sequence *_literals;

		public:
		  inline Vector(int lineno, cdk::node::Sequence *literals) :
			cdk::node::expression::Expression(lineno), _literals(literals) {
		  }

		  inline cdk::node::Sequence *literals() {
			  return _literals;
		  }

		  const char *name() {
			return "Vector";
		  }

		  void accept(SemanticProcessor *sp, int level) {
			sp->processVector(this, level);
		  }

		};
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
