#ifndef __PIPOCA_READ_H__
#define __PIPOCA_READ_H__

#include <cdk/nodes/Node.h>
#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression{

    
		class Read: public cdk::node::expression::Expression {
			cdk::node::expression::Identifier *_identifier;		  

			public:
			  inline Read(int lineno, cdk::node::expression::Identifier *identifier):
				cdk::node::expression::Expression(lineno), _identifier(identifier)  {
			  }

			  inline cdk::node::expression::Identifier *identifier() {
				return _identifier;
			  }
			  
			  const char *name() {
				return "Read";
			  }

			  void accept(SemanticProcessor *sp, int level) {
				sp->processRead(this, level);
			  }

		};
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
