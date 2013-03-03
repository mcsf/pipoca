#ifndef __PIPOCA_VARCONSTNODE_H__
#define __PIPOCA_VARCONSTNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {

    
    class VarConstNode: public cdk::node::Node {
	  cdk::node::Sequence* _identifiers;
	  cdk::node::Sequence* _literals;

    public:
      inline VarConstNode(int lineno, cdk::node::Sequence* identifiers, cdk::node::Sequence* literals) :
        cdk::node::Node(lineno), _identifiers(identifiers), _literals(literals) {
      }

    public:
      inline cdk::node::Sequence* identifiers() {
        return _identifiers;
      }
	  
	  inline cdk::node::Sequence* literals() {
        return _literals;
      }

    public:
      const char *name() {
        return "VarConstNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processVarConstNode(this, level);
      }

    };

  } // namespace node
} // namespace pipoca

#endif
