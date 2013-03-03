#ifndef __PIPOCA_CONDITION_H__
#define __PIPOCA_CONDITION_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression{
    class Condition: public cdk::node::expression::Expression {
		cdk::node::expression::Expression* _expr;

    public:
      inline Condition(int lineno, cdk::node::expression::Expression *expr) :
        cdk::node::expression::Expression(lineno), _expr(expr) {
      }

	  inline cdk::node::expression::Expression* expression(){ return _expr;}
      
	  const char *name() {
        return "ConditionNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processCondition(this, level);
      }

    }; // class
	}
  } // namespace node
} // namespace pipoca

#endif
