
#ifndef __PIPOCA_SEMANTICS_FUNCTION_H__
#define __PIPOCA_SEMANTICS_FUNCTION_H__

#include <string>
#include <vector>

#include "semantics/ExpressionType.h"
#include "semantics/PFerror.h"


namespace pipoca {
  namespace semantics {
	
	class Function{
	  public:
		class Argument{
			std::string _name;
			ExpressionType* _type;
			int	_offset;
		  public:
			Argument(std::string name,ExpressionType* type, int offset) : _name(name), _type(type), _offset(offset){}
			~Argument(){ delete _type;}
			
			ExpressionType* type(){ return _type;}
			std::string name(){ return _name;}
			int offset(){ return _offset;}
		};
	  private:
		std::string _name;
		ExpressionType* _type;
		vector<Argument*> _args;
		int _initialArgOffset;
	  public:
		
		
		Function(std::string name,ExpressionType* type) : _name(name), _type(type),_args(), _initialArgOffset(8){}
		~Function(){
			_args.clear();
			delete _type;
		}
		
		Argument* addArgument(std::string name,ExpressionType* type);
		Argument* argument(int n);
		
		int argNum();
		int argSize();
		
		std::string name(){return _name;}
		ExpressionType* type(){return _type;}
	};
  } // namespace semantics
} // namespace pipoca

#endif
