#ifndef __PIPOCA_EXPRESSION_DEC_H__
#define __PIPOCA_EXPRESSION_DEC_H__

#include <cdk/nodes/expressions/UnaryExpression.h>

namespace pipoca {
  namespace node {
    namespace expression {
	//!
    //! Class for describing decrement operators.
    //!
	class DEC : public cdk::node::expression::UnaryExpression {
			bool _suffix;		
		public:
			
			inline DEC(int lineno, cdk::node::expression::Expression *arg, bool suffix):
				cdk::node::expression::UnaryExpression(lineno,arg),  _suffix(suffix)  {}
			
			
			inline bool suffix() {
				return _suffix;
			}

			const char *name() const { return "DEC"; }

	
			virtual void accept(SemanticProcessor *sp, int level) {
			  sp->processDEC(this, level);
			}
			
      }; // class

    } // expression
  } // node
} // pipoca

#endif
