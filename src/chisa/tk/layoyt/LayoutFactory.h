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

#ifndef Chisa_LAYOUTFACTORY_H__CPP_
#define Chisa_LAYOUTFACTORY_H__CPP_

#include <string>
#include <memory>
#include <tinyxml2.h>
#include "../../logging/Exception.h"
#include "../../logging/Logger.h"

namespace chisa {
namespace tk {
class Layout;
class World;

namespace layout {
using namespace std;
using namespace tinyxml2;

//TODO: 要素名とレイアウトの対応表

class LayoutFactory {
private:
	logging::Logger& log_;
	weak_ptr<World> world_;
public:
	inline logging::Logger& log() const { return log_; }
	inline weak_ptr<World> world() const { return world_; }
private:
	XMLDocument* doc_;
	const bool doc_free_by_me_;
	XMLElement* root_;
public:
	LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename);
	LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename, XMLDocument* document, bool doc_free_by_me);
	LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename, const char* buffer, std::size_t lenb);
	virtual ~LayoutFactory();
public:
	shared_ptr<Layout> parseTree();
	shared_ptr<Layout> parseTree(weak_ptr<Layout> root, weak_ptr<Layout> parent, XMLElement* top);
};

}}}
#endif /* INCLUDE_GUARD */
