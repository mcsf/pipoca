#ifndef __PIPOCA_FUNCTIONCALL_H__
#define __PIPOCA_FUNCTIONCALL_H__

#include <cdk/nodes/Sequence.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression{

    
		class FunctionCall: public cdk::node::expression::Expression {
			cdk::node::expression::Identifier *_identifier;		  
			cdk::node::Sequence *_args;

			public:
			  inline FunctionCall(int lineno, cdk::node::expression::Identifier
					  *identifier, cdk::node::Sequence *args):
				cdk::node::expression::Expression(lineno),
				_identifier(identifier), _args(args) {
			  }

			  inline cdk::node::expression::Identifier *identifier() {return _identifier;}			  
			  inline cdk::node::Sequence *args() {return _args;}			  
			  
			  const char *name() {
				return "FunctionCall";
			  }

			  void accept(SemanticProcessor *sp, int level) {
				sp->processFunctionCall(this, level);
			  }

		};
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
