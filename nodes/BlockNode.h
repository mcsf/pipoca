#ifndef __PIPOCA_BLOCKNODE_H__
#define __PIPOCA_BLOCKNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/Sequence.h>

namespace pipoca {
  namespace node {

    //!
    //! Class for describing function nodes.
    //!
    class BlockNode: public cdk::node::Node {
      cdk::node::Sequence *_decls;
      cdk::node::Node *_instrs;
	  
    public:
      inline BlockNode(int lineno, cdk::node::Sequence *decls, cdk::node::Node *instrs) :
        cdk::node::Node(lineno), _decls(decls), _instrs(instrs) {
      }

    public:
      inline cdk::node::Sequence *decls() { return _decls; }
      inline cdk::node::Node *instrs() { return _instrs; }

    public:
      const char *name() {
        return "BlockNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processBlockNode(this, level);
      }

    };

  } // namespace node
} // namespace pipoca

#endif
