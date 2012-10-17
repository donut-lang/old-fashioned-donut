/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <deque>
#include <string>
#include <memory>

namespace chisa {
namespace tk {

using std::deque;
using std::shared_ptr;
using std::shared_ptr;

template <typename T>
class Stack
{
private:
	deque<T> stack;
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
	typedef typename deque<T >::const_iterator Iterator;
	typedef typename deque<T >::reverse_iterator ReverseIterator;
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
	virtual ~Stack(){}
};

}}
