
#include <string>
#include "semantics/Function.h"
#include "nodes/all.h"  

using namespace pipoca::semantics;


Function::Argument* Function::addArgument(std::string name,ExpressionType* type){
	Argument* arg = new Argument(name,type,_initialArgOffset+argSize());
	_args.push_back(arg);
	return arg;
}

Function::Argument* Function::argument(int n){
	if(n<0 || n>(int)_args.size())
		throw PFerror()<<"Argument out of range";
	return _args[n];
}
		
int Function::argNum(){
	return (int)_args.size();	
}  

int Function::argSize(){
	int size =  0;
		for (size_t i = 0; i < _args.size(); i++) {
			size += _args[i]->type()->totalSize();
		}	
	return size;
}

//---------------------------------------------------------------------------
//     T H E    E N D
//---------------------------------------------------------------------------
