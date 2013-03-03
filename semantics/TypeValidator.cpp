
#include <string>
#include "semantics/TypeValidator.h"
#include "nodes/all.h"  // automatically generated

using namespace pipoca::semantics;
void TypeValidator::binaryExpressionType (cdk::node::expression::BinaryExpression *node, int lvl, int input, type output){
	
	node->left()-> accept(this, lvl+1);
	node->right()->accept(this, lvl+1);
	
	if(node->left()->type() == NULL)
		throw  PFerror(node)<<"BinaryExpression: Left type undefined";
	if(node->right()->type() == NULL)
		throw  PFerror(node)<<"BinaryExpression: Right type undefined";
	
	type lType = node->left()->type()->name();
	type rType = node->right()->type()->name();
   
	int t = lType | rType;
	type nodeType;
	if ( input & TYPE_INT && !( t & ~(TYPE_INT))){
		nodeType = TYPE_INT;
	} else if ( input & TYPE_REAL && !( t & ~(TYPE_INT|TYPE_REAL))) {
		nodeType = TYPE_REAL;
	} else if ( input & TYPE_STRING && !( t & ~(TYPE_INT|TYPE_REAL|TYPE_STRING)) ) {
		nodeType = TYPE_STRING;
	} else 
		throw PFerror(node) << node->name() <<": Invalid types: "<< node->left()->type()->str() <<" "<<node->right()->type()->str();

	if(output) 
		nodeType = output;
	node->type(new ExpressionType(nodeType));
}

void TypeValidator::unaryExpressionType (cdk::node::expression::UnaryExpression *node, int lvl, int input, type output){

	node->argument()-> accept(this, lvl+1);
  
	int t = node->argument()->type()->name();
	type nodeType;
	if ( input & TYPE_INT &&  t == TYPE_INT){
		nodeType = TYPE_INT;
	} else if ( input & TYPE_REAL &&  t == TYPE_REAL) {
		nodeType = TYPE_REAL;
	} else if ( input & TYPE_STRING && t == TYPE_STRING) {
		nodeType = TYPE_STRING;
	} else 
		throw PFerror(node) << node->name() <<": Invalid type: "<< node->argument()->type()->str();

	if(output) 
		nodeType = output;
	node->type(new ExpressionType(nodeType));
}

void TypeValidator::variableValidator(cdk::node::expression::Identifier *node, const char * str){

	if(node==NULL)
		throw PFerror(node) <<"variableValidator: Identifier is NULL";
	char const*id = node->value().c_str();
	Symbol* sym = _symtab.find(id);
	if(sym != NULL){
		if(sym->function()!= NULL) 
			throw PFerror(node) << str<<": Symbol '"<< id <<"' already defined as function";

		if(_symtab.findLocal(id,sym))
			throw PFerror(node) <<str<<": Symbol '"<< id <<"' already defined in this context";

		if(sym->function() && sym->offset()>0) 
			throw PFerror(node) <<str<<": Symbol '"<< id <<"' already defined as argument";					
	}		
}

ExpressionType* TypeValidator::typeIndex(pipoca::node::expression::Index *index){
	if(index->value()){
		cdk::node::expression::Simple<int>* value = dynamic_cast<cdk::node::expression::Simple<int>*>(index->value());
		if(value<=0)
			throw PFerror(index)<<"Invalid argument index";
		if(value->value()>0)
			return	new ExpressionType(TYPE_VECTOR,value->value());
	} else {
		return new ExpressionType(TYPE_POINTER);
	}
	return NULL;
}

//================================================================

void TypeValidator::processSequence(cdk::node::Sequence *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    cdk::node::Node *n = node->node(i);
    if (n == NULL) break;
    n->accept(this, lvl+2);
  } 
}

//================================================================

void TypeValidator::processInstructionNode(pipoca::node::InstructionNode * const node, int lvl) {
  node->expression()->accept(this, lvl+2);
  if(node->expression()->type() == NULL)
	throw PFerror(node) <<"Instruction: Expression untyped";
  if(node->expression()->type()->totalSize() == 0)
	throw PFerror(node) <<"Instruction: Type Error";
}

//================================================================

void TypeValidator::processInteger(cdk::node::expression::Integer *const node, int lvl) {
	node->type(new ExpressionType(TYPE_INT));
}
void TypeValidator::processDouble(cdk::node::expression::Double *const node, int lvl) {
	node->type(new ExpressionType(TYPE_REAL));
}
void TypeValidator::processString(cdk::node::expression::String *const node, int lvl) {
	node->type(new ExpressionType(TYPE_STRING));
}
void TypeValidator::processVector(pipoca::node::expression::Vector * const node, int lvl){
	type t = TYPE_ERROR;
	if(!node->literals())
		throw PFerror(node) <<"Vector: Empty";
	for (size_t i = 0; i < node->literals()->size(); i++) {
		cdk::node::expression::Expression * expr = (cdk::node::expression::Expression *)node->literals()->node(i);
		expr->accept(this, lvl+1);
		type exprType = expr->type()->name();
		if(t==TYPE_ERROR || (t== TYPE_INT && exprType == TYPE_REAL))
			t = exprType;
		if ( exprType == TYPE_STRING )
			throw PFerror(node) <<"Vector: Can not contain a string";
	}		
	node->type(new ExpressionType(TYPE_VECTOR|t,node->literals()->size()));
}
void TypeValidator::processIdentifier(cdk::node::expression::Identifier *const node, int lvl) {

        char const*id =  node->value().c_str();
		Symbol *sym = _symtab.find(id);
       
        if (!sym)
			throw PFerror(node) <<"Identifier: Symbol '"<< id <<"' is undeclared";

		if (!sym->type() || !sym->type()->name())
			throw PFerror(node) << "Identifier: Symbol '"<< id <<"' is untyped";
		
        node->type(sym->type());
}
void TypeValidator::processArgument(pipoca::node::expression::Argument * const node, int lvl){
	if(node->index()->value()){
		cdk::node::expression::Simple<int>* value = dynamic_cast<cdk::node::expression::Simple<int>*>(node->index()->value());
		if(!value)
			throw PFerror(node)<<"Invalid argument index";
		if(value->value()>0)
			node->type(new ExpressionType(TYPE_VECTOR|node->type()->name(),value->value()));
	} else {
		node->type(new ExpressionType(TYPE_POINTER|node->type()->name()));
	}
}
void TypeValidator::processIndex(pipoca::node::expression::Index * const node, int lvl){
	node->identifier()->accept(this, lvl+1);
	node->type(node->identifier()->type());
}  
//================================================================
void TypeValidator::processBrackets(pipoca::node::expression::Brackets * const node, int lvl){
	node->expr()->accept(this, lvl+1);
	node->type(node->expr()->type());
}
//================================================================
void TypeValidator::processADD(cdk::node::expression::ADD *const node, int lvl) 		{
	if(!node->type())
		binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL|TYPE_STRING);
	if(node->type()->name()==TYPE_STRING){
		if(std::string(node->right()->name()) == std::string(node->name()) && node->right()->type()->name() != TYPE_STRING){
			node->right()->type(node->type());
			node->right()->accept(this, lvl+1);
		}
		if(std::string(node->left()->name()) == std::string(node->name())&& node->left()->type()->name() != TYPE_STRING){
			node->left()->type(node->type());
			node->left()->accept(this, lvl+1);
		}
	}	
}
void TypeValidator::processSUB(cdk::node::expression::SUB *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL);}
void TypeValidator::processMUL(cdk::node::expression::MUL *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL);}
void TypeValidator::processDIV(cdk::node::expression::DIV *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL);}
void TypeValidator::processMOD(cdk::node::expression::MOD *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL);}
void TypeValidator::processPOW(pipoca::node::expression::POW * const node, int lvl)	{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL);}

void TypeValidator::processLT(cdk::node::expression::LT  *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}
void TypeValidator::processLE(cdk::node::expression::LE  *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}
void TypeValidator::processGE(cdk::node::expression::GE  *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}
void TypeValidator::processGT(cdk::node::expression::GT  *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}
void TypeValidator::processNE(cdk::node::expression::NE  *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}
void TypeValidator::processEQ(cdk::node::expression::EQ  *const node, int lvl) 		{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}
void TypeValidator::processOR(pipoca::node::expression::OR * const node, int lvl)	{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}
void TypeValidator::processAND(pipoca::node::expression::AND * const node, int lvl)	{binaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}

void TypeValidator::processNOT(pipoca::node::expression::NOT * const node, int lvl)	{unaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL,TYPE_INT);}

void TypeValidator::processNEG(cdk::node::expression::NEG *const node, int lvl) 		{unaryExpressionType(node,lvl,TYPE_INT|TYPE_REAL);}
void TypeValidator::processINC(pipoca::node::expression::INC * const node, int lvl)	{unaryExpressionType(node,lvl,TYPE_INT);}
void TypeValidator::processDEC(pipoca::node::expression::DEC * const node, int lvl)	{unaryExpressionType(node,lvl,TYPE_INT);}

//================================================================


void TypeValidator::processPrint(pipoca::node::expression::Print * const node, int lvl){

	node->expr()->accept(this, lvl);
	ExpressionType* type = node->expr()->type();
	if(type->name() & ~(TYPE_INT|TYPE_STRING|TYPE_REAL))
		throw PFerror(node) << "Print: Invalid type "<< type->str();

	node->type(new ExpressionType(TYPE_INT));		
}
void TypeValidator::processRead(pipoca::node::expression::Read * const node, int lvl){

	node->identifier()->accept(this, lvl);
	ExpressionType* type = node->identifier()->type();
	if(type->name() & ~(TYPE_INT|TYPE_REAL))
		throw PFerror(node) << "Read: Invalid type "<< type->str();
		
	node->type(node->identifier()->type());		
}

//================================================================

void TypeValidator::processCondition(pipoca::node::expression::Condition * const node, int lvl) {
	node->expression()->accept(this, lvl); 
	ExpressionType * t = node->expression()->type();
	if (t->name() & ~(TYPE_INT|TYPE_REAL))
		throw PFerror(node) <<"INT or REAL expected in condition";
    node->type(t);		
}

//================================================================

void TypeValidator::processReturnNode(pipoca::node::ReturnNode * const node, int lvl){}	 
void TypeValidator::processContinueNode(pipoca::node::ContinueNode * const node, int lvl){}
void TypeValidator::processBreakNode(pipoca::node::BreakNode * const node, int lvl){}


void TypeValidator::processAddress(pipoca::node::expression::Address * const node, int lvl){
	node->identifier()->accept(this, lvl+2);
	node->type(new ExpressionType(TYPE_POINTER|node->identifier()->type()->all(),node->identifier()->type()->length()));
	node->identifier()->type(node->type());
}

void TypeValidator::processMemoryAllocationNode(pipoca::node::MemoryAllocationNode * const node, int lvl) {
	node->identifier()->accept(this, lvl+2);
	if (!(node->identifier()->type()->all() & TYPE_POINTER))
		throw PFerror(node) << "MemoryAllocation:"<<node->identifier()->type()->str() <<" in identifier expected POINTER";
	node->expression()->accept(this, lvl+2);
	if (!(node->expression()->type()->name() & TYPE_INT))
		throw PFerror(node) << "MemoryAllocation:"<<node->expression()->type()->str() <<" in expression expected INTEGER";
}

//================================================================

void TypeValidator::processAttribution(pipoca::node::expression::Attribution *const node, int lvl) {
    node->lvalue()->accept(this, lvl+2);
    node->rvalue()->accept(this, lvl+2);
	
	ExpressionType *ltype = node->lvalue()->type();
	ExpressionType *rtype = node->rvalue()->type();
	if(!ltype)
		throw PFerror(node) <<"Attribution: lvalue type undefined";
	if(!rtype)
		throw PFerror(node) <<"Attribution: rvalue type undefined";
	
	if(ltype->all() & TYPE_CONST)
		throw PFerror(node) <<"Attribution: lvalue must not be CONST";			
		
    if ((!((ltype->name() | rtype->name()) & (TYPE_INT|TYPE_REAL)) && ltype->name() != rtype->name()) 
		|| 	(((ltype->all()|rtype->all())&TYPE_POINTER)&&((ltype->all()&~rtype->all())&TYPE_POINTER)))
	        throw PFerror(node) << "Attribution: Invalid types: lvalue:"<< ltype->str() <<"  rvalue:"<< rtype->str();

	node->type(ltype);
}

void TypeValidator::processAttributionIndex(pipoca::node::expression::AttributionIndex * const node, int lvl){}

//================================================================

void TypeValidator::processVarConstNode(pipoca::node::VarConstNode * const node, int lvl){
	cdk::node::Sequence *identifiers = node->identifiers();
	cdk::node::Sequence *literals	= node->literals();
	if(identifiers->size() != literals->size())
		throw PFerror(node) <<"VarConstNode: "<<identifiers->size()<<" identifier for "<<literals->size()<<" values";
	for (size_t i = 0; i < identifiers->size(); i++) {
		cdk::node::expression::Identifier* identifier = (cdk::node::expression::Identifier*) identifiers->node(i);
		cdk::node::expression::Expression* literal    = (cdk::node::expression::Expression*) literals->node(i);
		variableValidator(identifier, "VarConstNode");		
		literal->accept(this, lvl+2);	
		
		identifier->type(new ExpressionType(literal->type()->all()|TYPE_CONST,literal->type()->length()));
	}
}
void TypeValidator::processVarNode(pipoca::node::VarNode * const node, int lvl){

	cdk::node::Sequence *indexLst = node->indexLst();
	for (size_t i = 0; i < indexLst->size(); i++) {
		pipoca::node::expression::Index *index = ((pipoca::node::expression::Index *) indexLst->node(i));
		cdk::node::expression::Identifier *identifier = (cdk::node::expression::Identifier *) index->identifier();
		
		ExpressionType* eType = node->type();
		ExpressionType* iType = typeIndex(index);
		if(iType)
			eType = new ExpressionType(iType->all()|eType->all(),eType->length());
		index->type(eType);
		identifier->type(eType);
		
		variableValidator(identifier, "VarNode");		
	}
}
void TypeValidator::processVarInitNode(pipoca::node::VarInitNode * const node, int lvl){
	cdk::node::expression::Identifier* identifier = node->index()->identifier();
	cdk::node::expression::Expression* expr       = node->expr();
	expr->accept(this, lvl+2);	
	
	ExpressionType* eType = expr->type();
	ExpressionType* iType = typeIndex(node->index());	
	if(iType)
		eType = new ExpressionType(iType->all()|eType->all(),eType->length());
	node->index()->type(eType);
	identifier->type(eType);
	
	variableValidator(identifier, "VarInitNode");	
}

void TypeValidator::processVarExtNode(pipoca::node::VarExtNode * const node, int lvl){}


//================================================================

 
void TypeValidator::processFunctionDeclNode(pipoca::node::FunctionDeclNode * const node, int lvl){}

void TypeValidator::processFunctionDefNode(pipoca::node::FunctionDefNode * const node, int lvl){

	std::string functionId = node->identifier()->value();
	Symbol* sym = _symtab.find(functionId.c_str());
	if( sym && sym->offset() != 0 )
		throw PFerror(node) << "FunctionDefNode: Symbol "<< functionId <<" already defined";
	if(node->args())
		node->args()->accept(this, lvl);
}
void TypeValidator::processFunctionCall(pipoca::node::expression::FunctionCall * const node, int lvl){

	std::string functionId = node->identifier()->value();
	Symbol* sym= _symtab.find(functionId.c_str()); 
	
	if(sym == NULL)
		throw PFerror(node) << "FunctionCall: Function '"<< functionId <<"' is undefined";
	
	cdk::node::Sequence *arguments = node->args();
	int argNum = (arguments==NULL)? 0 : arguments->size();
	if(argNum != sym->function()->argNum())
		throw PFerror(node) << "FunctionCall: Function '"<< functionId <<"' accept "<<sym->function()->argNum()<<" arguments but is called with "<<argNum;
					
	node->type(sym->type());		
	
	if(arguments!=NULL){
		arguments->accept(this, lvl);	
		for (int i = 0; i < argNum; i++) {
			cdk::node::expression::Expression *n = (cdk::node::expression::Expression*)arguments->node(i);
			if (n == NULL) break;
			ExpressionType *t = sym->function()->argument(i)->type();
			
			if(n->type()->name() != t->name())
				throw PFerror(node) << "FunctionCall: Argument "<< i+1 <<" is of type "<< n->type()->str() <<" expected "<<t->str();	
			
			if(t->all() & TYPE_VECTOR){
				if(!(n->type()->all() & TYPE_VECTOR))
					throw PFerror(node) << "FunctionCall: Argument "<< i+1 <<" is of type "<< n->type()->str() <<" expected "<<t->str();
				if(n->type()->length()!=t->length())
					throw PFerror(node) << "FunctionCall: Argument "<< i+1 <<" is of type "<< n->type()->str() <<" with length "<<n->type()->length()<<" expected "<<t->length();
			}
		}
	}
}


//---------------------------------------------------------------------------
//     T H E    E N D
//---------------------------------------------------------------------------
