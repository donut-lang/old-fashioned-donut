/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../chisa/Hexe.hpp"
#include "donut/NesGeistObject.hpp"

namespace nes {

class Hexe: public chisa::Hexe {
private:
	::cinamo::Handler<NesGeistProvider> nesGeistProvider_;
public:
	Hexe(chisa::Logger& log, std::string const& basepath);
	virtual ~Hexe() noexcept;
public:
	::cinamo::Handler<NesGeistProvider> nesGeistProvider() const noexcept { return this->nesGeistProvider_; };
public:
	Handler<Hexe> self() { return ::cinamo::Handler<Hexe>::__internal__fromRawPointerWithoutCheck(static_cast<Hexe*>(chisa::Hexe::self().get())); };
public:
	virtual void registerGeistProvider( ::cinamo::Handler< ::donut::Heap> const& heap ) override;
	virtual void registerWidgets(chisa::tk::WidgetFactory& factory) override;
	virtual void registerElements(chisa::tk::ElementFactory& factory) override;
public:
	virtual chisa::Handler<chisa::WorldGeist> invokeWorldGeist(chisa::HandlerW<chisa::tk::World> world, std::string const& nameOfGeist) override;
};

}
