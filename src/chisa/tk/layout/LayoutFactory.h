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
#include <functional>
#include <map>
#include <tinyxml2.h>
#include "../../logging/Exception.h"
#include "../../logging/Logger.h"
#include "../../util/class_utils.h"
#include "../Layout.h"

namespace chisa {
namespace tk {
class World;

namespace layout {
using namespace std;
using namespace tinyxml2;

template <typename T>
shared_ptr<T> layoutConstructor(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
{
	return Layout::create<T>(log, world, root, parent);
}

class LayoutFactory {
	DISABLE_COPY_AND_ASSIGN(LayoutFactory);
public:
	struct ElemName{
		static const std::string World;
		static const std::string Vertical;
		static const std::string Horizontal;
		static const std::string Tab;
		static const std::string Empty;
		static const std::string Scroll;
		static const std::string WidgetWrapper;
	};
	struct AttrName {
		static const std::string Id;
	};
private:
	logging::Logger& log_;
	weak_ptr<World> world_;
	std::map<std::string, std::function<shared_ptr<Layout>(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)> > layoutMap_;
public:
	inline logging::Logger& log() const { return log_; }
	inline weak_ptr<World> world() const { return world_; }
private:
	std::string filename_;
	XMLDocument* doc_;
	const bool doc_free_by_me_;
	XMLElement* root_;
public:
	LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename);
	LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename, XMLDocument* document, bool doc_free_by_me);
	LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename, const char* buffer, std::size_t lenb);
	virtual ~LayoutFactory();
private:
	void init();
public:
	shared_ptr<Layout> parseTree(const string& layoutname);
	shared_ptr<Layout> parseTree(weak_ptr<Layout> root, weak_ptr<Layout> parent, XMLElement* top);
	void registerLayout(const std::string& layoutName, std::function<shared_ptr<Layout>(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)> constructor);
	template <typename T>
	void registerLayout(const std::string& layoutName)
	{
		this->registerLayout(layoutName, layoutConstructor<T>);
	}
};

}}}
#endif /* INCLUDE_GUARD */
