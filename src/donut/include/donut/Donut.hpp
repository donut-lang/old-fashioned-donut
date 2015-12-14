/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include <cinamo/Logger.h>
#include <cinamo/Handler.h>
#include <cinamo/XVal.h>
#include <cinamo/VectorMap.h>

#include "Decl.hpp"
#include "Clock.hpp"
#include "source/Source.hpp"
#include "object/Heap.hpp"
#include "provider/Provider.hpp"
#include "vm/Machine.hpp"
#include "Patron.hpp"

namespace donut {
using namespace cinamo;

class Donut final : public HandlerBody<Donut> {
	DEFINE_MEMBER_REF(public, Logger, log);
private:
	Handler<Clock> clock_;
	Handler<Heap> heap_;
	Handler<Patron> patron_;
	VectorMap<std::string, Handler<Machine> > machines_;
public: /* 生成・破棄 */
	Donut();
	Donut(Logger& log);
	Donut(Handler<Patron> patron);
	Donut(Logger& log, Handler<Patron> patron);
	virtual ~Donut() noexcept = default;
	inline bool onFree() noexcept { return false; };
public:
	Handler<Machine> queryMachine( std::string const& name = "" );
	void sendMessage( std::string const& name, Handler<Object> const& obj );
public: /* Getter */
	Handler<Clock> clock() const noexcept{ return this->clock_; };
	Handler<Heap> heap() const noexcept { return this->heap_; };
	Handler<Source> parse(std::string const& src, std::string const& filename="<DEFAULT>", int const& lineno=0);
public: /* 処理系の保存・復帰をします。 */
	void bootstrap();
	XValue save();
	void load( XValue const& data);
public: /* seekとかをします。外部から呼びます */
	unsigned int nowTime() const noexcept;
	void seek( unsigned int const& time );
	void discardFuture();
	void discardHistory();
	unsigned int lastTime() const noexcept;
	unsigned int firstTime() const noexcept;
	bool canBack() const noexcept;
	bool canAdvance() const noexcept;
public: /* clockから呼ばれます */
	void onTickNotify();
	void onBackNotify();
	void onForwardNotify();
	void onDiscardFutureNotify();
	void onDiscardHistoryNotify();
};

}
