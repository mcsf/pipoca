#ifndef __PIPOCA_VAREXTNODE_H__
#define __PIPOCA_VAREXTNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    
    class VarExtNode: public cdk::node::Node {
      cdk::node::Node *_var;

    public:
      inline VarExtNode(int lineno, cdk::node::Node *var) :
        cdk::node::Node(lineno), _var(var) {
      }

    public:
      inline cdk::node::Node *var() {
        return _var;
      }

    public:
      const char *name() {
        return "VarExtNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processVarExtNode(this, level);
      }

    };

  } // namespace node
} // namespace pipoca

#endif
