#ifndef __PIPOCA_BREAKNODE_H__
#define __PIPOCA_BREAKNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    //!
    //! Class for describing break nodes.
    //!
    class BreakNode: public cdk::node::Node {
      cdk::node::expression::Expression *_value;

    public:
      inline BreakNode(int lineno, cdk::node::expression::Expression *value) :
        cdk::node::Node(lineno), _value(value) {
      }

      inline cdk::node::expression::Expression *value() {
        return _value;
      }

      const char *name() {
        return "BreakNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processBreakNode(this, level);
      }

    }; // class

  } // namespace node
} // namespace pipoca

#endif
