#ifndef __PIPOCA_FUNCTIONDECLNODE_H__
#define __PIPOCA_FUNCTIONDECLNODE_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/Sequence.h>

namespace pipoca {
  namespace node {

    //!
    //! Class for describing function nodes.
    //!
    class FunctionDeclNode: public cdk::node::Node {
      ExpressionType      *_type;
      cdk::node::expression::Identifier     *_identifier;
      cdk::node::Sequence *_args;

    public:
      inline FunctionDeclNode(int lineno, ExpressionType* type, cdk::node::expression::Identifier *identifier, cdk::node::Sequence *args) :
        cdk::node::Node(lineno), _type(type), _identifier(identifier), _args(args){
      }

    public:
      inline ExpressionType      * type() { return _type; }
      inline cdk::node::expression::Identifier *identifier() { return _identifier; }
      inline cdk::node::Sequence *args() { return _args; }


    public:
      const char *name() {
        return "FunctionDeclNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processFunctionDeclNode(this, level);
      }

    };

  } // namespace node
} // namespace pipoca

#endif
