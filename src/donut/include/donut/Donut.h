/**
 * Donut
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

#include <tarte/Logger.h>
#include <tarte/Handler.h>
#include <tarte/XVal.h>
#include <tarte/VectorMap.h>

#include "Decl.h"
#include "Clock.h"
#include "source/Source.h"
#include "object/Heap.h"
#include "provider/Provider.h"
#include "vm/Machine.h"
#include "Patron.h"

namespace donut {
using namespace tarte;

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

