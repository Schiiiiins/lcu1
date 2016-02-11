#include "ExpressionParser.h"
#include <stdlib.h>
#include <Logger.h>

ExpressionParser::ExpressionParser()
{
}

ExpressionParser::~ExpressionParser()
{
}

void ExpressionParser::SetVariable(const std::string& name, int value)
{
	_mapVariables[name] = value;
}

bool ExpressionParser::EvalOpWithStack( std::list<int>* stack, char op )
{
	if(stack->size()<2)return false;
	int arg2 = stack->back();
	stack->pop_back();
	int arg1 = stack->back();
	stack->pop_back();
	int result;
	if( !EvalOp( op, arg1, arg2, &result ) )return false;
	stack->push_back( result );
	return true;
}

bool ExpressionParser::EvalOp( char op, int arg1, int arg2, int* result )
{
	switch( op )
	{
	case '+':
		*result = arg1 + arg2;
		break;
	case '-':
		*result = arg1 - arg2;
		break;
	case '*':
		*result = arg1 * arg2;
		break;
	case '/':
		*result = arg1 / arg2;
		break;
	case '%':
		*result = arg1 % arg2;
		break;
	default:
		return false;
	}
	//LOG( Logger::LOG_DEBUG, "EvalOp( '%c', %d, %d, %d )", op, arg1, arg2, *result);
	return true;
}

bool ExpressionParser::ResolveVariable( const std::string& name, int* value )
{
	t_mapVariables::iterator it = _mapVariables.find(name);
	if( it==_mapVariables.end() )return false;
	*value = it->second;
	return true;
}

bool ExpressionParser::Eval( const std::string& expr, int* result )
{
	//LOG( Logger::LOG_DEBUG, "Parsing expression %s", expr.c_str());
	std::list<int> valueStack;
	std::list<unsigned char> opStack;

	std::string::size_type left = 0;
	while( left != std::string::npos )
	{
		std::string::size_type right = expr.find_first_of("+-/*%() ", left);
		if( left == right )
		{
			//LOG( Logger::LOG_DEBUG, "Operator '%c'", expr[left]);
			switch( expr[left] )
			{
			case '+':
			case '-':
				if( (!opStack.empty()) && (opStack.back() != '(' ) )
				{
					if( !EvalOpWithStack( &valueStack, opStack.back() ) )
					{
						return false;
					}
					opStack.pop_back();
				}
				opStack.push_back( expr[left] );
				break;
			case '*':
			case '/':
			case '%':
				if( (!opStack.empty()) && (opStack.back() != '(') && (opStack.back() != '+') && (opStack.back() != '-') )
				{
					if( !EvalOpWithStack( &valueStack, opStack.back() ) )
					{
						return false;
					}
					opStack.pop_back();
				}
				opStack.push_back( expr[left] );
				break;
			case '(':
				opStack.push_back( expr[left] );
				break;
			case ')':
				while( (!opStack.empty()) && (opStack.back() != '(') )
				{
					if( !EvalOpWithStack( &valueStack, opStack.back() ) )
					{
						return false;
					}
					opStack.pop_back();
				}
				if( opStack.empty() )return false;
				opStack.pop_back();
			}
			left++;
		}else{
			std::string token = expr.substr(left, right-left);
			int value;
			if( token[0] >= '0' && token[0] <= '9' )
			{
				value = strtol(token.c_str(), NULL, 0);
			}else{
				if( !ResolveVariable( token, &value ) )return false;
			}
			valueStack.push_back( value );
			//LOG( Logger::LOG_DEBUG, "Token %s, value %d", token.c_str(), value);
			left = right;
		}
	}
	while( !opStack.empty() )
	{
		if( !EvalOpWithStack( &valueStack, opStack.back() ) )
		{
			return false;
		}
		opStack.pop_back();
	}
	if( valueStack.size() != 1 )return false;
	*result = valueStack.back();
	//LOG( Logger::LOG_DEBUG, "%s = %d", expr.c_str(), *result);
	return true;
}
