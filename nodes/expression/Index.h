#ifndef __PIPOCA_INDEX_H__
#define __PIPOCA_INDEX_H__

#include <cdk/nodes/expressions/Expression.h>

namespace pipoca {
  namespace node {
	namespace expression{

    
		class Index: public cdk::node::expression::Expression {
			cdk::node::expression::Identifier *_identifier;
			cdk::node::expression::Expression *_value;		  

			public:
			  inline Index(int lineno, cdk::node::expression::Identifier
					  *identifier, cdk::node::expression::Expression *value):
				cdk::node::expression::Expression(lineno),
				_identifier(identifier), _value(value)  {
			  }

			  inline cdk::node::expression::Identifier *identifier() {
				return _identifier;
			  }
			  
			  inline cdk::node::expression::Expression *value() {
				return _value;
			  }
			  
			  const char *name() {
				return "Index";
			  }

			  void accept(SemanticProcessor *sp, int level) {
				sp->processIndex(this, level);
			  }

		};
	} // namespace expression
  } // namespace node
} // namespace pipoca

#endif
