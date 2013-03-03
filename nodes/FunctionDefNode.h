#ifndef __PIPOCA_FUNCTIONDEFNODE_H__
#define __PIPOCA_FUNCTIONDEFNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/Sequence.h>
#include <vector>
#include "nodes/expression/Argument.h"

namespace pipoca {
  namespace node {

    //!
    //! Class for describing function nodes.
    //!
    class FunctionDefNode: public cdk::node::Node {
	  ExpressionType      *_type;
      cdk::node::expression::Identifier     *_identifier;
      cdk::node::Sequence *_args;
      cdk::node::Node	  *_block;

    public:
      inline FunctionDefNode(int lineno, ExpressionType* type, cdk::node::expression::Identifier *identifier, cdk::node::Sequence *args, cdk::node::Node *block) :
        cdk::node::Node(lineno), _type(type), _identifier(identifier), _args(args), _block(block) {
      }
	  virtual ~FunctionDefNode(){
	  }

    public:
      inline ExpressionType      * type() { return _type; }
      inline cdk::node::expression::Identifier *identifier() { return _identifier; }
      inline cdk::node::Sequence *args() { return _args; }
      inline cdk::node::Node *block() { return _block; }

    public:
      const char *name() const {
        return "FunctionDefNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processFunctionDefNode(this, level);
      }

    };

  } // namespace node
} // namespace pipoca

#endif
