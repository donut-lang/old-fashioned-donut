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
,enter_(false)
,discardFutureRequested_(false)
,discardHistoryRequested_(false)
,tickRequested_(false)
{
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
	this->enter_=true;
	while(time != this->now()){
		if( time < this->now()){
			this->backStep();
		}else{
			this->forwardStep();
		}
	}
	this->processRequest();
	this->enter_=false;
}

void Clock::processRequest()
{
	int cnt = 0;
	while( this->discardFutureRequested_ || this->discardHistoryRequested_ || this->tickRequested_){
		if( this->discardFutureRequested_ ){
			this->discardFutureStep();
			this->discardFutureRequested_=false;
		}
		if(this->discardHistoryRequested_ ){
			this->discardHistoryStep();
			this->discardHistoryRequested_=false;
		}
		if(this->tickRequested_) {
			this->tickStep();
			this->tickRequested_ = false;
		}
		++cnt;
		if(cnt > 1000){
			throw DonutException(__FILE__, __LINE__, "[BUG] Endless discard/tick loop.");
		}
	}
}

void Clock::back()
{
	if(this->enter_){
		throw DonutException(__FILE__, __LINE__, "[BUG] You cannt seek back while discarding/seeking/ticking clock, homura!");
	}
	this->enter_=true;
	this->backStep();
	this->processRequest();
	this->enter_=false;
}
void Clock::forward()
{
	if(this->enter_){
		throw DonutException(__FILE__, __LINE__, "[BUG] You cannt seek forward while discarding/seeking/ticking clock, homura!");
	}
	this->enter_=true;
	this->forwardStep();
	this->processRequest();
	this->enter_=false;
}

void Clock::tick()
{
	if(this->enter_){
		this->log().i(TAG, "Ticking reserved.");
		this->tickRequested_=true;
		return;
	}
	this->enter_=true;
	this->tickStep();
	this->processRequest();
	this->enter_=false;
}
void Clock::discardFuture()
{
	if(this->enter_){
		this->log().i(TAG, "Discarding future reserved.");
		this->discardFutureRequested_=true;
		return;
	}
	this->enter_=true;
	this->discardFutureStep();
	this->processRequest();
	this->enter_=false;
}
void Clock::discardHistory()
{
	if(this->enter_){
		this->log().i(TAG, "Discarding history reserved.");
		this->discardHistoryRequested_=true;
		return;
	}
	this->enter_=true;
	this->discardHistoryStep();
	this->processRequest();
	this->enter_=false;
}

/**********************************************************************************
 * time impl
 **********************************************************************************/

void Clock::tickStep()
{
	if( this->last_ != this->now_ ){
		this->discardFutureStep();
	}
	this->last_ = ++this->now_;
	if(Handler<Donut> dnt = donut_.lock()) {
		dnt->onTickNotify();
	}
}

void Clock::backStep()
{
	--this->now_;
	if(Handler<Donut> donut = this->donut_.lock()){
		donut->onBackNotify();
	}else{
		log().e(TAG, "Seeked to %d, but donut was already dead.", time);
	}
}
void Clock::forwardStep()
{
	++this->now_;
	if(Handler<Donut> donut = this->donut_.lock()){
		donut->onForwardNotify();
	}else{
		log().e(TAG, "Seeked to %d, but donut was already dead.", time);
	}
}

void Clock::discardFutureStep()
{
	if( this->last_ <= this->now_ ){
		log().w(TAG, "Tried to discard future, but vm state is already latest.");
		return;
	}
	if(Handler<Donut> donut = this->donut_.lock()){
		donut->onDiscardFutureNotify();
	}else{
		log().e(TAG, "Tried to discard future, but donut was already dead.");
	}
	this->last_ = this->now_;
}
void Clock::discardHistoryStep()
{
	if( this->first_ >= this->now_ ){
		log().w(TAG, "Tried to discard history, but vm state is already oldest.");
		return;
	}
	if(Handler<Donut> donut = this->donut_.lock()){
		donut->onDiscardHistoryNotify();
	}else{
		log().e(TAG, "Tried to discard future, but donut was already dead.");
	}
	this->first_ = this->now_;
}


}}
