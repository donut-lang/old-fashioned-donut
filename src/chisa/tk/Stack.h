/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include <string>

namespace chisa {
namespace tk {

template <typename T>
class Stack
{
private:
	std::vector<T> stack;
public:
	T push(const T elm)
	{
		stack.push_back(elm);
		return elm;
	}

	T pop()
	{
		if(stack.empty()){
			return T();
		}
		T val = stack.back();
		stack.pop_back();
		return val;
	}
	T top()
	{
		if(stack.empty()){
			return T();
		}
		return stack.back();
	}
	T bottom()
	{
		if(stack.empty()){
			return T();
		}
		return stack.front();
	}
	T replace(T elm)
	{
		if(stack.empty()){
			return T();
		}
		T val = stack.back();
		stack.assign(stack.size()-1, elm);
		return val;
	}
	size_t size()
	{
		return stack.size();
	}
	typedef typename std::vector<T>::const_iterator Iterator;
	typedef typename std::vector<T>::reverse_iterator ReverseIterator;
	Iterator begin(){
		return stack.begin();
	}
	Iterator end(){
		return stack.end();
	}
	ReverseIterator rbegin(){
		return stack.rbegin();
	}
	ReverseIterator rend(){
		return stack.rend();
	}
	int indexOf(T elm){
		std::size_t i = 0;
		for(T& sptr : stack){
			if(sptr == elm){
				return i;
			}
			++i;
		}
		return -1;
	}
	T at(size_t idx)
	{
		if(idx >= stack.size()){
			return T();
		}
		return stack.at(idx);
	}
	void erase(size_t idx)
	{
		if(idx >= stack.size()){
			return;
		}
		stack.erase(stack.begin()+idx);
	}
public:
	explicit Stack(){}
	virtual ~Stack() noexcept = default;
};

}}
