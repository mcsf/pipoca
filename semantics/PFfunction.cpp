
#include <string>
#include "semantics/PFfunction.h"
#include "nodes/all.h"  

using namespace pipoca::semantics;

std::string PFfunction::mkname(std::string name){
	return "PIP."+name;
	return name;
}
std::string PFfunction::mknameret(std::string name){
	return "PIP."+name+"_ret";
}

void PFfunction::init(){

	_internFunctions.push_back("_intpow");
	_internFunctions.push_back("_copyStr");
	
	Function* argc = new Function("argc",new ExpressionType(TYPE_INT));
	addExternFunction(argc,true);
	
	Function* argv = new Function("argv",new ExpressionType(TYPE_STRING));
	argv->addArgument("indice",new ExpressionType(TYPE_INT));
	addExternFunction(argv,true);
	
	Function* envp = new Function("envp",new ExpressionType(TYPE_STRING));
	envp->addArgument("indice",new ExpressionType(TYPE_INT));
	addExternFunction(envp,true);
	
	Function* atoi = new Function("atoi",new ExpressionType(TYPE_INT));
	atoi->addArgument("string",new ExpressionType(TYPE_STRING));
	addExternFunction(atoi, true);
	
	Function* itoa = new Function("itoa",new ExpressionType(TYPE_STRING));
	itoa->addArgument("int",new ExpressionType(TYPE_INT));
	addExternFunction(itoa);
	
	Function* dtoa = new Function("dtoa",new ExpressionType(TYPE_STRING));
	dtoa->addArgument("double",new ExpressionType(TYPE_REAL));
	addExternFunction(dtoa);
	
	Function* readi = new Function("readi",new ExpressionType(TYPE_INT));
	addExternFunction(readi);
	
	Function* readd = new Function("readd",new ExpressionType(TYPE_STRING));
	addExternFunction(readd);
	
	Function* printi = new Function("printi",new ExpressionType(TYPE_INT));
	printi->addArgument("int",new ExpressionType(TYPE_INT));
	addExternFunction(printi);
	
	Function* prints = new Function("prints",new ExpressionType(TYPE_STRING));
	prints->addArgument("string",new ExpressionType(TYPE_STRING));
	addExternFunction(prints);
	
	Function* printd = new Function("printd",new ExpressionType(TYPE_REAL));
	printd->addArgument("printd",new ExpressionType(TYPE_STRING));
	addExternFunction(printd);
	
	Function* println = new Function("println",new ExpressionType(TYPE_INT));
	addExternFunction(println);
}
void PFfunction::addProgExternFunction(Function* f){
	addExternFunction(f,true);
}
void PFfunction::addExternFunction(Function* f,bool progCanCall){
	if(progCanCall)
		if(!_symtab.find(f->name().c_str()))
			_symtab.insert(f->name().c_str(),new Symbol(f,0));
	_externFunctions[f->name()]=f;
}

void PFfunction::CALL(std::string f,bool prog){
	if(prog && _symtab.find(f.c_str()) /*&& _symtab.find(f.c_str())->offset() != 0*/){
		_progCalled.push_back(f);
		_pf.CALL(mkname(f));
	} else if(_externFunctions.find(f) != _externFunctions.end()){
		_externCalled.push_back(f);
		_pf.CALL(f);
	}else if(find(_internFunctions,f)){
	    _internCalled.push_back(f);
		_pf.CALL(f);
	}else{
		throw PFerror()<<"PFfunction::CALL Invalid function id:"<<f<<"\n";
	}
} 

void PFfunction::EXTERN(){
	_externCalled.sort();
	_externCalled.unique();
	_progCalled.sort();
	_progCalled.unique();
	list<std::string>::iterator it;
	for ( it=_externCalled.begin() ; it != _externCalled.end(); it++ ){
		_pf.EXTRN((*it));
	}
	if(_externCalled.empty()){
		_pf.EXTRN("_start");
	}
	for ( it=_progCalled.begin() ; it != _progCalled.end(); it++ ){
		Symbol * sym = _symtab.find((*it).c_str());
		if(sym && sym->offset() == 0)
			_pf.EXTRN((*it));	
	}
	for ( it=_progCalled.begin() ; it != _progCalled.end(); it++ ){
		Symbol * sym = _symtab.find((*it).c_str());
		if(sym && sym->offset() == 0){
			_pf.LABEL(mkname((*it)));
			_pf.JMP((*it));
		}		
	}
	
}

void PFfunction::INTERN(){
	_internCalled.sort();
	_internCalled.unique();
	
	if(find(_internCalled, "_copyStr")){
		int  init= ++_lbl,end= ++_lbl,strPtr= 8,newStrPtr= 12;
		_pf.GLOBL("_copyStr", _pf.FUNC());
		_pf.LABEL("_copyStr");
		_pf.ENTER(0);
		_pf.LABEL(mklbl(init));	
		_pf.LOCV(strPtr);		//gets old char position
		_pf.LDCHR();			//load char	
		_pf.JZ(mklbl(end));		//if char = 0 stops
		
		_pf.BSS(); 
		_pf.BYTE(1);
		_pf.TEXT();

		_pf.LOCV(strPtr);		//gets old char position
		_pf.LDCHR();			//load char	
		_pf.LOCV(newStrPtr);	//gets new char position
		_pf.STCHR();			//store oldchar position value in newchar position

		_pf.LOCAL(strPtr);		//incr old char position
		_pf.INCR(1);
		_pf.LOCAL(newStrPtr);	//incr char position
		_pf.INCR(1);
		_pf.JMP(mklbl(init));	//if char = 0 stops
		_pf.LABEL(mklbl(end));
		
		_pf.LOCV(newStrPtr);	//gets old char position
		_pf.POP();
		_pf.LEAVE();
		_pf.RETN(8);
	}
	
	if(find(_internCalled, "_intpow")){
		/* QUICK for large exponents
		ipow(base,exp){  
			int res=1;
			while(exp){
				if (!exp&1) res*=base
				exp>>=1
				base*=base
			}
			return res;
		}*/
		int lbl1 = ++_lbl,lbl2 = ++_lbl,lbl3 = ++_lbl,res=-4,base=12,exp=8;
		_pf.GLOBL("_intpow", _pf.FUNC());
		_pf.LABEL("_intpow");
		_pf.ENTER(4);
		_pf.INT(1);							//result
		_pf.LOCA(res);
		
		_pf.LABEL(mklbl(lbl1));		//while(exp)
		_pf.LOCV(exp);				//while(exp)
		_pf.JZ(mklbl(lbl3));		//while(exp)
		_pf.LOCV(exp);				//	if(!exp&1)
		_pf.INT(1);					//	if(!exp&1)
		_pf.AND();					//	if(!exp&1)
		_pf.JZ(mklbl(lbl2));		//	if(!exp&1)												
		_pf.LOCV(res);				//		res*=base
		_pf.LOCV(base);				//		res*=base
		_pf.MUL();					//		res*=base
		_pf.LOCA(res);				//		res*=base
		_pf.LABEL(mklbl(lbl2));		//	endif
		_pf.LOCV(exp);				//	exp>>=1								
		_pf.INT(1);					//	exp>>=1	
		_pf.SHTRS();				//	exp>>=1	
		_pf.LOCA(exp);				//	exp>>=1	
		_pf.LOCV(base);				//	base*=base
		_pf.LOCV(base);				//	base*=base
		_pf.MUL();					//	base*=base
		_pf.LOCA(base);				//	base*=base
		_pf.JMP(mklbl(lbl1));		//endwhile
		_pf.LABEL(mklbl(lbl3));		//endwhile
		
		_pf.LOCV(res);
		_pf.POP();
		_pf.LEAVE();
		_pf.RETN(8);
	}
}