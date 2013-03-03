#ifndef __PIPOCA_MEMORYALLOCATIONNODE_H__
#define __PIPOCA_MEMORYALLOCATIONNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    class MemoryAllocationNode: public cdk::node::Node {
      cdk::node::expression::Identifier *_identifier;
      cdk::node::expression::Expression *_expression;

    public:
	  inline MemoryAllocationNode(int lineno, cdk::node::expression::Identifier
			  *identifier, cdk::node::expression::Expression *expression) :
        cdk::node::Node(lineno), _identifier(identifier), _expression(expression) {
      }

    public:
      inline cdk::node::expression::Identifier *identifier() {
        return _identifier;
      }
      inline cdk::node::expression::Expression *expression() {
        return _expression;
      }

    public:
      const char *name() {
        return "MemoryAllocationNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processMemoryAllocationNode(this, level);
      }

    }; // class

  } // namespace node
} // namespace pipoca

#endif
