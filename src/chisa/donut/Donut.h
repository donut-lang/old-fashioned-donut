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

#include "source/Source.h"
#include "object/Heap.h"
#include "vm/Machine.h"

#include "Clock.h"

#include "../logging/Logger.h"
#include "../Handler.h"
#include "../util/XVal.h"
#include "../util/VectorMap.h"

namespace chisa {
namespace donut {
class Provider;

class Donut final : public HandlerBody<Donut> {
	DEFINE_MEMBER_REF(public, logging::Logger, log);
private:
	Handler<Clock> clock_;
	Handler<Heap> heap_;
	util::VectorMap<std::string, Handler<Machine> > machines_;
public: /* 生成・破棄 */
	Donut(logging::Logger& log);
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
	util::XValue save();
	void load( util::XValue const& data);
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

}}

