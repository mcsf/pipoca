#ifndef __PIPOCA_INSTRUCTIONNODE_H__
#define __PIPOCA_INSTRUCTIONNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    class InstructionNode: public cdk::node::Node {
		cdk::node::expression::Expression* _expr;

    public:
      inline InstructionNode(int lineno, cdk::node::expression::Expression *expr) :
        cdk::node::Node(lineno), _expr(expr) {
      }

	  inline cdk::node::expression::Expression* expression(){ return _expr;}
      
	  const char *name() {
        return "InstructionNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processInstructionNode(this, level);
      }

    }; // class

  } // namespace node
} // namespace pipoca

#endif
