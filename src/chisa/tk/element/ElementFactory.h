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
std::shared_ptr<T> elementConstructor(logging::Logger& log, std::weak_ptr<World> world, std::weak_ptr<Element> root, std::weak_ptr<Element> parent)
{
	return Element::create<T>(log, world, root, parent);
}

class ElementFactory {
	DISABLE_COPY_AND_ASSIGN(ElementFactory);
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
	std::weak_ptr<World> world_;
	typedef std::function<std::shared_ptr<Element>(logging::Logger& log, std::weak_ptr<World> world, std::weak_ptr<Element> root, std::weak_ptr<Element> parent)> ConstructorType;
	util::VectorMap<std::string, ConstructorType> elementMap_;
public:
	inline logging::Logger& log() const { return log_; }
	inline std::weak_ptr<World> world() const { return world_; }
private:
	std::string filename_;
	tinyxml2::XMLDocument* doc_;
	const bool doc_free_by_me_;
	tinyxml2::XMLElement* root_;
public:
	ElementFactory(logging::Logger& log, std::weak_ptr<World> world, std::string const& filename);
	ElementFactory(logging::Logger& log, std::weak_ptr<World> world, std::string const& filename, tinyxml2::XMLDocument* document, bool doc_free_by_me);
	ElementFactory(logging::Logger& log, std::weak_ptr<World> world, std::string const& filename, const char* buffer, std::size_t lenb);
	virtual ~ElementFactory();
private:
	void init();
public:
	std::shared_ptr<Element> parseTree(std::string const& layoutId);
	std::shared_ptr<Element> parseTree(std::weak_ptr<Element> root, std::weak_ptr<Element> parent, tinyxml2::XMLElement* top);
	void registerElement(std::string const& elementName, std::function<std::shared_ptr<Element>(logging::Logger& log, std::weak_ptr<World> world, std::weak_ptr<Element> root, std::weak_ptr<Element> parent)> constructor);
	template <typename T>
	void registerElement(std::string const& elementName)
	{
		this->registerElement(elementName, elementConstructor<T>);
	}
};

}}}
