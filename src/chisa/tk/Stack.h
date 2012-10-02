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

#ifndef Chisa_TK_STACK_H__CPP_
#define Chisa_TK_STACK_H__CPP_

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
	deque<shared_ptr<T> > stack;
public:
	shared_ptr<T> push(const shared_ptr<T> elm)
	{
		stack.push_back(elm);
		return elm;
	}

	shared_ptr<T> pop()
	{
		if(stack.empty()){
			return shared_ptr<T>();
		}
		shared_ptr<T> val = stack.back();
		stack.pop_back();
		return val;
	}
	shared_ptr<T> top()
	{
		if(stack.empty()){
			return shared_ptr<T>();
		}
		return stack.back();
	}
	shared_ptr<T> bottom()
	{
		if(stack.empty()){
			return shared_ptr<T>();
		}
		return stack.front();
	}
	shared_ptr<T> replace(shared_ptr<T> elm)
	{
		if(stack.empty()){
			return shared_ptr<T>();
		}
		shared_ptr<T> val = stack.back();
		stack.assign(stack.size()-1, elm);
		return val;
	}
	size_t size()
	{
		return stack.size();
	}
	typedef typename deque<shared_ptr<T> >::const_iterator Iterator;
	typedef typename deque<shared_ptr<T> >::reverse_iterator ReverseIterator;
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
	int indexOf(T& world){
		std::size_t i = 0;
		for(shared_ptr<T>& sptr : stack){
			if(sptr.get() == &world){
				return i;
			}
			++i;
		}
		return -1;
	}
	int indexOf(shared_ptr<T> world)
	{
		if(!world.get()){
			return 0;
		}
		return indexOf(*(world.get()));
	}
	shared_ptr<T> at(size_t idx)
	{
		if(idx >= stack.size()){
			return shared_ptr<T>();
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

#endif /* INCLUDE_GUARD */
