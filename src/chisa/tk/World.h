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

#ifndef Chisa_TK_WORLD_H__CPP_
#define Chisa_TK_WORLD_H__CPP_

#include "Task.h"
#include "Geom.h"
#include <memory>
#include <deque>
#include <string>

namespace chisa {
namespace tk {
class WidgetGroup;
class Widget;

using std::shared_ptr;
using std::weak_ptr;
using std::deque;
using std::string;

class World {
private:
	logging::Logger& log;
	TaskHandler taskHandler;
	shared_ptr<WidgetGroup> rootWidget;
	Box size;
public:
	World(logging::Logger& log);
	virtual ~World();
public:
	void loadLayout(const string& filename);
	weak_ptr<Widget> getWidgetById(const string& id);
public:
	void render();
	void idle(const float delta_ms);
	void reshape(const Box& area);
};

class WorldStack
{
private:
	deque<shared_ptr<World> > stack;
public:
	inline shared_ptr<World> push(const shared_ptr<World> elm)
	{
		stack.push_back(elm);
		return elm;
	}

	inline shared_ptr<World> pop()
	{
		if(stack.empty()){
			return shared_ptr<World>();
		}
		shared_ptr<World> val = stack.back();
		stack.pop_back();
		return val;
	}
	inline shared_ptr<World> top()
	{
		if(stack.empty()){
			return shared_ptr<World>();
		}
		return stack.back();
	}
	inline shared_ptr<World> bottom()
	{
		if(stack.empty()){
			return shared_ptr<World>();
		}
		return stack.front();
	}
	inline shared_ptr<World> replace(shared_ptr<World> elm)
	{
		if(stack.empty()){
			return shared_ptr<World>();
		}
		shared_ptr<World> val = stack.back();
		stack.assign(stack.size()-1, elm);
		return val;
	}
	inline size_t size()
	{
		return stack.size();
	}
	typedef typename deque<shared_ptr<World> >::const_iterator Iterator;
	typedef typename deque<shared_ptr<World> >::reverse_iterator ReverseIterator;
	inline Iterator begin(){
		return stack.begin();
	}
	inline Iterator end(){
		return stack.end();
	}
	inline ReverseIterator rbegin(){
		return stack.rbegin();
	}
	inline ReverseIterator rend(){
		return stack.rend();
	}
	inline int indexOf(World& world){
		std::size_t i = 0;
		for(shared_ptr<World>& sptr : stack){
			if(sptr.get() == &world){
				return i;
			}
			++i;
		}
		return -1;
	}
	inline int indexOf(shared_ptr<World> world)
	{
		if(!world.get()){
			return 0;
		}
		return indexOf(*(world.get()));
	}
	inline shared_ptr<World> at(size_t idx)
	{
		if(idx >= stack.size()){
			return shared_ptr<World>();
		}
		return stack.at(idx);
	}
	inline void erase(size_t idx)
	{
		if(idx >= stack.size()){
			return;
		}
		stack.erase(stack.begin()+idx);
	}
public:
	explicit WorldStack(){}
	virtual ~WorldStack(){}
};

}}
#endif /* INCLUDE_GUARD */
