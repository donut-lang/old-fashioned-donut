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

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <tinyxml2.h>
#include "../../logging/Exception.h"
#include "../../logging/Logger.h"
#include "../../util/ClassUtil.h"
#include "../../util/VectorMap.h"
#include "../Element.h"

namespace chisa {
namespace tk {
class World;

namespace element {


template <typename T>
Handler<T> elementConstructor(logging::Logger& log, HandlerW<World> world, HandlerW<Element> root, HandlerW<Element> parent)
{
	return Handler<T>(new T(log, world, root, parent));
}

class ElementFactory {
	DISABLE_COPY_AND_ASSIGN(ElementFactory);
public:
	struct ElemName{
		static const std::string World;
		static const std::string Vertical;
		static const std::string Horizontal;
		static const std::string Frame;
		static const std::string Tab;
		static const std::string Empty;
		static const std::string Scroll;
		static const std::string Button;
		static const std::string WidgetWrapper;
	};
	struct AttrName {
		static const std::string Id;
	};
	typedef std::function<Handler<Element>(logging::Logger& log, HandlerW<World> world, HandlerW<Element> root, HandlerW<Element> parent)> ConstructorType;
private:
	logging::Logger& log_;
	HandlerW<World> world_;
	util::VectorMap<std::string, ConstructorType> elementMap_;
public:
	inline logging::Logger& log() const { return log_; }
	inline HandlerW<World> world() const { return world_; }
private:
	std::string filename_;
	tinyxml2::XMLDocument* doc_;
	const bool doc_free_by_me_;
	tinyxml2::XMLElement* root_;
public:
	ElementFactory(logging::Logger& log, HandlerW<World> world, std::string const& filename);
	ElementFactory(logging::Logger& log, HandlerW<World> world, std::string const& filename, tinyxml2::XMLDocument* document, bool doc_free_by_me);
	ElementFactory(logging::Logger& log, HandlerW<World> world, std::string const& filename, const char* buffer, std::size_t lenb);
	virtual ~ElementFactory();
private:
	void init();
public:
	Handler<Element> parseTree(std::string const& layoutId);
	Handler<Element> parseTree(HandlerW<Element> root, HandlerW<Element> parent, tinyxml2::XMLElement* top);
	void registerElement(std::string const& elementName, ConstructorType constructor);
	template <typename T>
	void registerElement(std::string const& elementName)
	{
		this->registerElement(elementName, elementConstructor<T>);
	}
};

}}}
