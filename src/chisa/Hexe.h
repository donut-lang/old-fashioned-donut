/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <memory>
#include <cinamo/Logger.h>
#include <cinamo/FileSystem.h>
#include <cinamo/Handler.h>

#include "tk/element/ElementFactory.h"
#include "tk/widget/WidgetFactory.h"

namespace chisa {
using namespace cinamo;
class WorldGeist;

class Hexe : public HandlerBody<Hexe> {
	DEFINE_MEMBER_REF(protected, Logger, log);
	DEFINE_MEMBER_CONST(protected, std::string, basepath);
public:
	Hexe(Logger& log, std::string const& basepath);
	virtual ~Hexe() noexcept;
	inline bool onFree() noexcept { return false; };
public:
	virtual std::string toString() const;
	template <typename... Args>
	constexpr std::string resolveFilepath(Args const&... path) const noexcept
	{
		return file::join(this->basepath(), path...);
	}

public:
	virtual void registerGeistProvider( ::cinamo::Handler< ::donut::Heap> const& heap ){};
	virtual void registerWidgets(tk::WidgetFactory& factory){};
	virtual void registerElements(tk::ElementFactory& factory){};
public:
	virtual Handler<WorldGeist> invokeWorldGeist(HandlerW<chisa::tk::World> world, std::string const& nameOfGeist) = 0;
};

class WorldGeist : public HandlerBody<WorldGeist> {
	DISABLE_COPY_AND_ASSIGN(WorldGeist);
	DEFINE_MEMBER_REF(protected, Logger, log);
	HandlerW<Hexe> hexe_;
	HandlerW<chisa::tk::World> world_;
	HandlerW< ::donut::Object> donutObject_;
public:
	Handler< ::donut::Object> donutObject(Handler< ::donut::Heap> const& heap);
public:
	virtual std::string toString() const;
public:
	WorldGeist(Logger& log, Handler<Hexe> const& hexe, HandlerW<chisa::tk::World> world);
	virtual ~WorldGeist() noexcept;
	inline bool onFree() noexcept { return false; };
public:
	Handler<chisa::tk::World> world();
	Handler<Hexe> hexe();
protected:
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) = 0;
};

}
