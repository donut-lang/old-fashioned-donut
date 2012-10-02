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
#include <memory>
#include <string>

#include "Task.h"
#include "Geom.h"
#include "Stack.h"

namespace chisa {
namespace tk {
class Layout;
class Widget;

using std::shared_ptr;
using std::weak_ptr;
using std::string;

class World {
private:
	logging::Logger& log;
	TaskHandler taskHandler;
	Stack<Layout> layoutStack;
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
private:
	void popLayout();
	void pushLayout();
};

}}
#endif /* INCLUDE_GUARD */
