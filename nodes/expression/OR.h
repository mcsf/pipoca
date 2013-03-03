#ifndef __PIPOCA_EXPRESSION_OR_H__
#define __PIPOCA_EXPRESSION_OR_H__

#include <cdk/nodes/expressions/BinaryExpression.h>

namespace pipoca {
  namespace node {
    namespace expression {
	//!
    //! Class for describing or operator
    //!
    class OR : public cdk::node::expression::BinaryExpression {
		public:
		/**
		 * @param lineno source code line number for this nodes
		 * @param left first operand
		 * @param right second operand
		 */
		inline OR(int lineno, cdk::node::expression::Expression *left, cdk::node::expression::Expression *right):
			cdk::node::expression::BinaryExpression(lineno, left, right) {}

		/**
		 * @return the name of the node's class
		 */
		const char *name() const { return "OR"; }

		/**
		 * @param sp semantic processor visitor
		 * @param level syntactic tree level
		 */
		virtual void accept(SemanticProcessor *sp, int level) {
		  sp->processOR(this, level);
		}
		
      }; // class

    } // expression
  } // node
} // pipoca

#endif
