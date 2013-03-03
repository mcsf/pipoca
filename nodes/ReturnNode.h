#ifndef __PIPOCA_RETURNNODE_H__
#define __PIPOCA_RETURNNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    //!
    //! Class for describing return-then nodes.
    //!
    class ReturnNode: public cdk::node::Node {

    public:
      inline ReturnNode(int lineno) :
        cdk::node::Node(lineno){
      }

      const char *name() {
        return "ReturnNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processReturnNode(this, level);
      }

    }; // class

  } // namespace node
} // namespace pipoca

#endif
