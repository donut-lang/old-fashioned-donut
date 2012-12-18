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
,state(State::NORMAL)
,discardFutureRequested_(false)
,discardHistoryRequested_(false)
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
util::XValue Clock::save()
{
	Handler<util::XObject> val( new util::XObject );
	val->set("now", this->now_);
	val->set("first", this->first_);
	val->set("last", this->last_);
	return val;
}
void Clock::load(util::XValue const& data)
{
	auto obj = data.as<util::XObject>();
	this->now_ = obj->get<util::XUInt>("now");
	this->first_ = obj->get<util::XUInt>("first");
	this->last_ = obj->get<util::XUInt>("last");
}

/**********************************************************************************
 * time function
 **********************************************************************************/

void Clock::seek( unsigned int const& time )
{
	if( time < this->firstTime() ) {
		throw DonutException(__FILE__, __LINE__, "[BUG] Failed to seek. time: %d < %d", time, this->first_);
	}
	if( time > this->lastTime() ) {
		throw DonutException(__FILE__, __LINE__, "[BUG] Failed to seek. time: %d > %d", time, this->last_);
	}
	while(time != this->now()){
		if( time < this->now()){
			this->back();
		}else{
			this->forward();
		}
	}
}

void Clock::processRequestedDiscard()
{
	if( this->discardFutureRequested_ ){
		this->discardFuture();
	}
	if(this->discardHistoryRequested_ ){
		this->discardHistory();
	}
}

void Clock::back()
{
	switch(this->state){
	case State::NORMAL:
		break;
	case State::SEEK:
		throw DonutException(__FILE__, __LINE__, "[BUG] Seek function is not reentrant.");
		return;
	case State::DISCARD:
		throw DonutException(__FILE__, __LINE__, "[BUG] You cannot call seek function while discarding.");
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon clock state!");
	}
	if( this->now_ <= this->first_ ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Cannot back the clock anymore.");
	}
	this->state = State::SEEK;
	--this->now_;
	if(Handler<Donut> donut = this->donut_.lock()){
		donut->onBackNotify();
	}else{
		log().e(TAG, "Seeked to %d, but donut was already dead.", time);
	}
	this->state = State::NORMAL;
	this->processRequestedDiscard();
}
void Clock::forward()
{
	switch(this->state){
	case State::NORMAL:
		break;
	case State::SEEK:
		throw DonutException(__FILE__, __LINE__, "[BUG] Seek function is not reentrant.");
		return;
	case State::DISCARD:
		throw DonutException(__FILE__, __LINE__, "[BUG] You cannot call seek function while discarding.");
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon clock state!");
	}
	if( this->now_ >= this->last_ ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Cannot back the clock anymore.");
	}
	this->state = State::SEEK;
	++this->now_;
	if(Handler<Donut> donut = this->donut_.lock()){
		donut->onForwardNotify();
	}else{
		log().e(TAG, "Seeked to %d, but donut was already dead.", time);
	}
	this->state = State::NORMAL;
	this->processRequestedDiscard();
}

void Clock::discardFuture()
{
	switch(this->state){
	case State::NORMAL:
		break;
	case State::SEEK:
		this->log().i(TAG, "Discarding future reserved");
		this->discardFutureRequested_ = true;
		return;
	case State::DISCARD:
		throw DonutException(__FILE__, __LINE__, "[BUG] Discarding function is not reentrant");
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon clock state!");
	}
	if( this->last_ <= this->now_ ){
		log().w(TAG, "Tried to discard future, but vm state is already latest.");
		return;
	}
	this->state = State::DISCARD;
	{
		if(Handler<Donut> donut = this->donut_.lock()){
			donut->onDiscardFutureNotify();
		}else{
			log().e(TAG, "Tried to discard future, but donut was already dead.");
		}
		this->last_ = this->now_;
		this->discardFutureRequested_ = false;
	}
	this->state = State::NORMAL;
}
void Clock::discardHistory()
{
	switch(this->state){
	case State::NORMAL:
		break;
	case State::SEEK:
		this->log().i(TAG, "Discarding future reserved");
		this->discardHistoryRequested_ = true;
		return;
	case State::DISCARD:
		throw DonutException(__FILE__, __LINE__, "[BUG] Discarding function is not reentrant");
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon clock state!");
	}
	if( this->first_ >= this->now_ ){
		log().w(TAG, "Tried to discard history, but vm state is already oldest.");
		return;
	}
	this->state = State::DISCARD;
	{
		if(Handler<Donut> donut = this->donut_.lock()){
			donut->onDiscardHistoryNotify();
		}else{
			log().e(TAG, "Tried to discard future, but donut was already dead.");
		}
		this->first_ = this->now_;
		this->discardHistoryRequested_ = false;
	}
	this->state = State::NORMAL;
}


}}
