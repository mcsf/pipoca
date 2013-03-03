
#include <string>
#include "semantics/ContextValidator.h"
#include "nodes/all.h"  // automatically generated

pipoca::semantics::Function * pipoca::semantics::ContextValidator::getFunction(){return _function;}
int pipoca::semantics::ContextValidator::offset(){return _offsetTotal;}
void pipoca::semantics::ContextValidator::reset(){_function=NULL;_offsetTotal=_offset=0;}
void pipoca::semantics::ContextValidator::init(pipoca::node::FunctionDefNode * node){
	std::string id = node->identifier()->value();
	_function = new Function(id,new ExpressionType(node->type()->name()));
	if ( !_symtab.find(id.c_str())){						
			_symtab.insert(id.c_str(), new Symbol(_function,-node->type()->totalSize()));
	} else if(_symtab.find(_function->name().c_str())->offset()==0){
			_symtab.replace(id.c_str(), new Symbol(_function,-node->type()->totalSize()));
	} else
		throw PFerror(node)<<"ContextValidator::init received NULL FunctionDefNode";
}

//================================================================

void pipoca::semantics::ContextValidator::processSequence(cdk::node::Sequence * const node, int lvl){
   if(_countMode){
	   for (size_t i = 0; i < node->size(); i++) {
		cdk::node::Node *n = node->node(i);
		if (n == NULL) break;
		n->accept(this, lvl+2);
	  }
	}
}
	  
void pipoca::semantics::ContextValidator::processBlockNode(pipoca::node::BlockNode * const node, int lvl){
	if(_countMode){
		_symtab.push();
			if(node->decls())
				node->decls()->accept(this, lvl+2);
			node->instrs()->accept(this, lvl+2);
		_symtab.pop();
	}
}

void pipoca::semantics::ContextValidator::processWhileElseNode(pipoca::node::WhileElseNode * const node, int lvl) {
	node->thenblock()->accept(this, lvl+2);
	if(node->elseblock())
		node->elseblock()->accept(this, lvl+2);
}
void pipoca::semantics::ContextValidator::processIfElseNode(pipoca::node::IfElseNode * const node, int lvl) {
	node->thenblock()->accept(this, lvl+2);
	if(node->elseblock())
		node->elseblock()->accept(this, lvl+2);
} 
void pipoca::semantics::ContextValidator::processDoElseNode(pipoca::node::DoElseNode * const node, int lvl) {
	node->thenblock()->accept(this, lvl+2);
	if(node->elseblock())
		node->elseblock()->accept(this, lvl+2);
}

//================================================================

void pipoca::semantics::ContextValidator::processVarNode(pipoca::node::VarNode * const node, int lvl){
	try{
		pipoca::semantics::TypeValidator tv(os(), _symtab);
		node->accept(&tv, lvl);	
	} catch (PFerror e) { throw e;}		
	
	cdk::node::Sequence *indexLst = node->indexLst();
	if(indexLst == NULL) return;
	for (size_t i = 0; i < indexLst->size(); i++) {
		pipoca::node::expression::Index *index = ((pipoca::node::expression::Index *) indexLst->node(i));
		cdk::node::expression::Identifier *n = (cdk::node::expression::Identifier *)index->identifier();
		if (n == NULL) break;
		if(_countMode){
			_offsetTotal += index->type()->totalSize();
			continue;
		}
		char const* id = n->value().c_str();
		if(_function)_offset += index->type()->totalSize();
		_symtab.insert(id, new Symbol(index->type(), id, -_offset)); // put in the symbol table
	}
}
void pipoca::semantics::ContextValidator::processVarInitNode(pipoca::node::VarInitNode * const node, int lvl){
	try{
		pipoca::semantics::TypeValidator tv(os(), _symtab);
		node->accept(&tv, lvl);	
	} catch (PFerror e) { throw e;}		
	
	cdk::node::expression::Identifier* identifier = node->index()->identifier();
	if(_countMode){
		_offsetTotal += identifier->type()->totalSize();
		return;
	}	
	char const* id = identifier->value().c_str();
	if(_function) _offset += identifier->type()->totalSize();
	_symtab.insert(id, new Symbol(identifier->type(), id, -_offset)); // put in the symbol table

}
void pipoca::semantics::ContextValidator::processVarExtNode(pipoca::node::VarExtNode * const node, int lvl){}
void pipoca::semantics::ContextValidator::processVarConstNode(pipoca::node::VarConstNode * const node, int lvl){
	try{
		pipoca::semantics::TypeValidator tv(os(), _symtab);
		node->accept(&tv, lvl);	
	} catch (PFerror e) { throw e;}	
	
	cdk::node::Sequence *identifiers = node->identifiers();
	for (size_t i = 0; i < identifiers->size(); i++) {
		cdk::node::expression::Identifier* identifier = (cdk::node::expression::Identifier*) identifiers->node(i);
		if (identifier == NULL) break;
		const char* id = identifier->value().c_str();
		
		if(identifier->type()->name() & TYPE_INT && !(identifier->type()->all() & (TYPE_VECTOR|TYPE_POINTER))){
			cdk::node::expression::Simple<int>* literal  = (cdk::node::expression::Simple<int>*) node->literals()->node(i);
			_symtab.insert(id, new Symbol(identifier->type(), id, literal->value()));			
		}else{	
			if(_countMode){
				_offsetTotal += identifier->type()->totalSize();
				continue;
			}
			if(_function)_offset += identifier->type()->totalSize();
			_symtab.insert(id, new Symbol(identifier->type(), id, -_offset));
		}
	}
}

//================================================================

void pipoca::semantics::ContextValidator::processFunctionDeclNode(pipoca::node::FunctionDeclNode * const node, int lvl){
	
	const char* id = node->identifier()->value().c_str();
	_function = new Function(id,new ExpressionType(node->type()->name()));
	
	if(node->args()!=NULL)
		for (size_t i = 0; i < node->args()->size(); i++) {
			pipoca::node::expression::Argument * argument = (pipoca::node::expression::Argument *)node->args()->node(i);
			pipoca::node::expression::Index * index = (pipoca::node::expression::Index *) argument->index();
			std::string argId = index->identifier()->value();
			Function::Argument* arg = _function->addArgument(argId,argument->type());
			_symtab.insert(argId.c_str(), new Symbol(arg));
		}
}

void pipoca::semantics::ContextValidator::processFunctionDefNode(pipoca::node::FunctionDefNode * const node, int lvl){

	if(_function == NULL)
		throw PFerror(node)<<"FunctionDef context not initiated";
		
	_offset = 0;
	
	const char* id = node->identifier()->value().c_str();
	_offset += node->type()->totalSize();
	_symtab.insert(id, new Symbol(_function, -_offset)); //return value.
	
	_countMode = true;_offsetTotal = _offset;
		node->block()->accept(this, lvl);
	_countMode = false;

	if(node->args()!=NULL)
		for (size_t i = 0; i < node->args()->size(); i++) {
			pipoca::node::expression::Argument * argument = (pipoca::node::expression::Argument *)node->args()->node(i);
			pipoca::node::expression::Index * index = (pipoca::node::expression::Index *) argument->index();
			std::string argId = index->identifier()->value();

			Function::Argument* arg = _function->addArgument(argId,argument->type());
			_symtab.insert(argId.c_str(), new Symbol(arg));
		}		
}


//---------------------------------------------------------------------------
//     T H E    E N D
//---------------------------------------------------------------------------
