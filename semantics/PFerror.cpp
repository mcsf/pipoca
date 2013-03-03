#include "semantics/PFerror.h"

using namespace pipoca::semantics; 
 


void PFerror::out() throw(){
	 std::cerr<<what();
	 exit(1);
} 

const char* PFerror::what() const throw(){
	std::stringstream ss;
	if(_node)
		ss<<"  "<<_node->lineno()<<": "<<_message<<std::endl;  
	else
		ss<<"  "<<_message<<std::endl;
	return ss.str().c_str();
}

		 
PFerror& PFerror::operator<<(const char* str) throw(){
	_message.append(str);
	return *this;
}   

PFerror& PFerror::operator<<(std::string str) throw(){
	_message.append(str);
	return *this;
}

PFerror& PFerror::operator<<(int i) throw(){
	std::stringstream ss;
    ss << i;
    _message.append(ss.str());
	return *this;
}  

