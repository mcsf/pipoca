#ifndef __PIPOCA_EXPRESSION_INC_H__
#define __PIPOCA_EXPRESSION_INC_H__

#include <cdk/nodes/expressions/UnaryExpression.h>

namespace pipoca {
  namespace node {
    namespace expression {
	//!
    //! Class for describing increment operators.
    //!
    class INC : public cdk::node::expression::UnaryExpression {
			bool _suffix;		
		public:

			inline INC(int lineno, cdk::node::expression::Expression *arg, bool suffix):
				cdk::node::expression::UnaryExpression(lineno,arg),  _suffix(suffix)  {}

			inline bool suffix() {
				return _suffix;
			}

			const char *name() const { return "INC"; }


			virtual void accept(SemanticProcessor *sp, int level) {
			  sp->processINC(this, level);
			}
		
      }; // class

    } // expression
  } // node
} // pipoca

#endif
