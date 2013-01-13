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
#include <functional>
#include <vector>
#include <tarte/Exception.h>
#include <tarte/Logger.h>
#include <tarte/ClassUtil.h>
#include <tarte/VectorMap.h>
#include "../Element.h"
#include "../donut/ElementObject.h"

namespace chisa {
using namespace tarte;

namespace tk {
class World;

template <typename T>
Handler<T> elementConstructor(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST)
{
	return Handler<T>(new T(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_APPLY));
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
		static const std::string Label;
		static const std::string TextArea;
		static const std::string WidgetWrapper;
	};
	struct AttrName {
		static const std::string Id;
	};
	typedef std::function<Handler<Element>(CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_PARAM_LIST)> ConstructorType;
private:
	Logger& log_;
	HandlerW<World> world_;
	VectorMap<std::string, ConstructorType> tagToElementConstructorMap_;
	VectorMap<std::string, Handler<ElementProvider> > demangledElementNameToDonutProviderMap_;
public:
	inline Logger& log() const { return log_; }
	inline HandlerW<World> world() const { return world_; }
private:
	std::string filename_;
	tinyxml2::XMLDocument* doc_;
	const bool doc_free_by_me_;
	tinyxml2::XMLElement* root_;
public:
	ElementFactory(Logger& log, HandlerW<World> world, std::string const& filename);
	ElementFactory(Logger& log, HandlerW<World> world, std::string const& filename, tinyxml2::XMLDocument* document, bool doc_free_by_me);
	ElementFactory(Logger& log, HandlerW<World> world, std::string const& filename, const char* buffer, std::size_t lenb);
	virtual ~ElementFactory();
private:
	void init();
	void registerLayout(std::string const& xmlElementName, ConstructorType constructor);
	void registerProvider(std::string const& demangledElementName, Handler<ElementProvider> const& provider);
public: /* Element作成メソッド */
	Handler<Element> parseTree(std::string const& layoutId);
	Handler<Element> parseTree(HandlerW<Element> parent, tinyxml2::XMLElement* top);
public: /* donut関連 */
	Handler<ElementProvider> getProviderOf(Element* me);
	void registerDonutProvider(Handler< ::donut::Heap> const& heap);
public: /* 登録 */
	template <typename ElementKlass>
	void registerLayout(std::string const& xmlElementName)
	{
		static_assert(std::is_base_of<Element, ElementKlass>::value, "Please register layout for element class.");
		this->registerLayout(xmlElementName, elementConstructor<ElementKlass>);
	}
	//XXX: ここだめ！！！ヒープごとにプロバイダは作られるので、コンストラクタを登録しないと。
	template <typename ElementKlass>
	void registerProvider(Handler<ElementProvider> const& provider)
	{
		static_assert(std::is_base_of<Element, ElementKlass>::value, "Please register provider for element class.");
		this->registerProvider(::tarte::demangle<ElementKlass>(), provider);
	}
private:
	Handler<Heap> heap_;
	class HeapLock final{
		STACK_OBJECT(HeapLock);
		DISABLE_COPY_AND_ASSIGN(HeapLock);
	public:
		ElementFactory& self;
	public:
		HeapLock(ElementFactory& self, Handler< ::donut::Heap> const& heap):self(self) {
			self.heap_ = heap;
		}
		~HeapLock(){
			self.heap_.reset();
		}
	};
};

}}
