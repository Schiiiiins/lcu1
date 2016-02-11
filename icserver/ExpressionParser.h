#pragma once

#include <string>
#include <map>
#include <list>

class ExpressionParser
{
public:
	ExpressionParser();
	~ExpressionParser();
	void SetVariable(const std::string& name, int value);
	bool Eval( const std::string& expr, int* result );
private:
	bool EvalOp( char op, int arg1, int arg2, int* result );
	bool EvalOpWithStack( std::list<int>* stack, char op );
	bool ResolveVariable( const std::string& name, int* value );
	typedef std::map<std::string, int> t_mapVariables;
	t_mapVariables _mapVariables;
};