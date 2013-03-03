#ifndef __PIPOCA_CONTINUE_H__
#define __PIPOCA_CONTINUE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    //!
    //! Class for describing if-then nodes.
    //!
    class ContinueNode: public cdk::node::Node {
      cdk::node::expression::Expression *_value;

    public:
      inline ContinueNode(int lineno, cdk::node::expression::Expression *value) :
        cdk::node::Node(lineno), _value(value){
      }

      inline cdk::node::expression::Expression *value() {
        return _value;
      }

      const char *name() {
        return "ContinueNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processContinueNode(this, level);
      }

    }; // class

  } // namespace node
} // namespace pipoca

#endif
