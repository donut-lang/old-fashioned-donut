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

#include "Clock.h"
#include "Donut.h"

namespace chisa {
namespace donut {

static const std::string TAG("Clock");

Clock::Clock( Donut* const donut)
:log_(donut->log())
,last_(1)
,first_(1)
,now_(1)
,donut_(donut)
{
}

void Clock::tick()
{
	if( this->last_ != this->now_ ){
		this->discardFuture();
	}
	this->last_ = ++this->now_;
}

/**********************************************************************************
 * save/restore function
 **********************************************************************************/
void Clock::bootstrap()
{
	this->last_ = 1;
	this->first_ = 1;
	this->now_ = 1;
}
Handler<util::XObject> Clock::save()
{
	Handler<util::XObject> val( new util::XObject );
	val->set("now", this->now_);
	val->set("first", this->first_);
	val->set("last", this->last_);
	return val;
}
void Clock::load(Handler<util::XObject> const& data)
{
	this->now_ = data->get<util::XUInt>("now");
	this->first_ = data->get<util::XUInt>("first");
	this->last_ = data->get<util::XUInt>("last");
}

/**********************************************************************************
 * time function
 **********************************************************************************/

void Clock::seek( unsigned int const& time )
{
	if( time < this->first_ ) {
		throw DonutException(__FILE__, __LINE__, "[BUG] Failed to seek. time: %d < %d", time, this->first_);
	}
	if( time > this->last_ ) {
		throw DonutException(__FILE__, __LINE__, "[BUG] Failed to seek. time: %d > %d", time, this->last_);
	}
	this->now_ = time;
	Handler<Donut> donut = this->donut_.lock();
	if(!donut){
		log().e(TAG, "Tried to discard future, but donut was already dead.");
		return;
	}
	donut->onSeekNotify();
	// これだけではまだ未来・過去は削除しない
}

void Clock::discardFuture()
{
	if( this->last_ == this->now_ ){
		log().w(TAG, "Tried to discard future, but vm state is already latest.");
		return;
	}
	Handler<Donut> donut = this->donut_.lock();
	if(!donut){
		log().e(TAG, "Tried to discard future, but donut was already dead.");
		return;
	}
	donut->onDiscardFutureNotify();
	this->last_ = this->now_;
}
void Clock::discardHistory()
{
	if( this->first_ == this->now_ ){
		log().w(TAG, "Tried to discard history, but vm state is already oldest.");
		return;
	}
	Handler<Donut> donut = this->donut_.lock();
	if(!donut){
		log().e(TAG, "Tried to discard history, but donut was already dead.");
		return;
	}
	donut->onDiscardHistoryNotify();
	this->first_ = now_;
}


}}
