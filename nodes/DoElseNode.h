#ifndef __PIPOCA_DOELSENODE_H__
#define __PIPOCA_DOELSENODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    //!
    //! Class for describing Do-then-else nodes.
    //!
    class DoElseNode: public cdk::node::Node {
      cdk::node::expression::Expression *_condition;
      cdk::node::Node *_thenblock, *_elseblock;

    public:
      inline DoElseNode(int lineno, cdk::node::expression::Expression *condition,
                        cdk::node::Node *thenblock, cdk::node::Node *elseblock) :
        cdk::node::Node(lineno), _condition(condition), _thenblock(thenblock),
            _elseblock(elseblock) {
      }

      inline cdk::node::expression::Expression *condition() {
        return _condition;
      }
      inline cdk::node::Node *thenblock() {
        return _thenblock;
      }
      inline cdk::node::Node *elseblock() {
        return _elseblock;
      }

      const char *name() {
        return "DoElseNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processDoElseNode(this, level);
      }

    }; // class

  } // namespace node
} // namespace pipoca

#endif
