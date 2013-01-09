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

#include <donut/Donut.h>
#include <donut/Clock.h>

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
,machineRequest_(MachineRequest::NONE)
,machineRequestedSeekTime_(0)
{
}

void Clock::tickFromMachine()
{
	if(unlikely(machineRequest_ != MachineRequest::NONE)){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Clock operation already requested.");
	}
	if(this->log().d()) {
		this->log().d(TAG, "Tick operation requested from machine.");
	}
	this->machineRequest_ = MachineRequest::TICK;
}
void Clock::seekFromMachine( unsigned int const& time )
{
	if(unlikely(machineRequest_ != MachineRequest::NONE)){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Clock operation already requested.");
	}
	this->machineRequest_ = MachineRequest::SEEK;
	this->machineRequestedSeekTime_ = time;
	if(this->log().d()) {
		this->log().d(TAG, "Seek operation to %d requested from machine.", time);
	}
}
void Clock::backFromMachine()
{
	if(unlikely(machineRequest_ != MachineRequest::NONE)){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Clock operation already requested.");
	}
	if(this->log().d()) {
		this->log().d(TAG, "Back operation requested from machine.");
	}
	this->machineRequest_ = MachineRequest::BACK;
}
void Clock::forwardFromMachine()
{
	if(unlikely(machineRequest_ != MachineRequest::NONE)){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Clock operation already requested.");
	}
	if(this->log().d()) {
		this->log().d(TAG, "Forward operation requested from machine.");
	}
	this->machineRequest_ = MachineRequest::FORWARD;
}
void Clock::discardFutureFromMachine()
{
	if(unlikely(machineRequest_ != MachineRequest::NONE)){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Clock operation already requested.");
	}
	if(this->log().d()) {
		this->log().d(TAG, "Discarding future requested from machine.");
	}
	this->machineRequest_ = MachineRequest::DISCARD_FUTURE;
}
void Clock::discardHistoryFromMachine()
{
	if(unlikely(machineRequest_ != MachineRequest::NONE)){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Clock operation already requested.");
	}
	if(this->log().d()) {
		this->log().d(TAG, "Discarding history requested from machine.");
	}
	this->machineRequest_ = MachineRequest::DISCARD_HISTORY;
}

bool Clock::invokeMahcineRequestInner()
{
	MachineRequest const req = this->machineRequest_;
	this->machineRequest_ = MachineRequest::NONE;
	switch (req) {
	case MachineRequest::TICK:
		if (this->log().d()) {
			this->log().d(TAG, "Invoking requested tick operation from machine.");
		}
		this->tick();
		return false;
	case MachineRequest::SEEK:
		if (this->log().d()) {
			this->log().d(TAG, "Invoking requested seek operation to %d from machine.", this->machineRequestedSeekTime_);
		}
		this->seek(this->machineRequestedSeekTime_);
		return true;
	case MachineRequest::BACK:
		if (this->log().d()) {
			this->log().d(TAG, "Invoking requested back operation from machine.");
		}
		this->back();
		return true;
	case MachineRequest::FORWARD:
		if (this->log().d()) {
			this->log().d(TAG, "Invoking requested forward operation from machine.");
		}
		this->forward();
		return true;
	case MachineRequest::DISCARD_FUTURE:
		if (this->log().d()) {
			this->log().d(TAG, "Invoking requested discarding future operation from machine.");
		}
		this->discardFuture();
		return false;
	case MachineRequest::DISCARD_HISTORY:
		if (this->log().d()) {
			this->log().d(TAG, "Invoking requested discarding history operation from machine.");
		}
		this->discardHistory();
		return false;
	default:
		DONUT_EXCEPTION(Exception, "[BUG] Unknown time operation.");
	}
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
XValue Clock::save()
{
	Handler<XObject> val( new XObject );
	val->set("now", this->now_);
	val->set("first", this->first_);
	val->set("last", this->last_);
	return val;
}
void Clock::load(XValue const& data)
{
	auto obj = data.as<XObject>();
	this->now_ = obj->get<XUInt>("now");
	this->first_ = obj->get<XUInt>("first");
	this->last_ = obj->get<XUInt>("last");
}

/**********************************************************************************
 * time function
 **********************************************************************************/

void Clock::seek( unsigned int const& time )
{
	if( unlikely( time < this->firstTime() || time > this->lastTime() ) ) {
		DONUT_EXCEPTION(Exception, "[BUG] Failed to seek. time: %d , available time: %d -> %d", time, this->first_, this->last_);
	}else if(time == this->now()){
		this->log().w(TAG, "Requested seeking to %d, but it's already %d", time, this->now());
		return;
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
		if(unlikely(++cnt > 1000)){
			DONUT_EXCEPTION(Exception, "[BUG] Endless discard/tick loop.");
		}
	}
}

void Clock::back()
{
	if(unlikely(this->enter_)){
		DONUT_EXCEPTION(Exception, "[BUG] You cannt seek back while discarding/seeking/ticking clock, homura!");
	}
	this->enter_=true;
	this->backStep();
	this->processRequest();
	this->enter_=false;
}
void Clock::forward()
{
	if(unlikely(this->enter_)){
		DONUT_EXCEPTION(Exception, "[BUG] You cannt seek forward while discarding/seeking/ticking clock, homura!");
	}
	this->enter_=true;
	this->forwardStep();
	this->processRequest();
	this->enter_=false;
}

void Clock::tick()
{
	if(unlikely(this->enter_)){
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
	if(unlikely(this->enter_)){
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
	if(unlikely(this->enter_)){
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
	if(this->log().t()){
		this->log().t(TAG, "time flies %d -> %d...", this->now_, this->now_+1);
	}
	this->last_ = ++this->now_;
	if(Handler<Donut> dnt = donut_.lock()) {
		dnt->onTickNotify();
	}
}

void Clock::backStep()
{
	if(this->log().t()){
		this->log().t(TAG, "time goes back from %d to %d...", this->now_, this->now_-1);
	}
	--this->now_;
	if(Handler<Donut> donut = this->donut_.lock()){
		donut->onBackNotify();
	}else{
		log().e(TAG, "Seeked to %d, but donut was already dead.", time);
	}
}
void Clock::forwardStep()
{
	if(this->log().t()){
		this->log().t(TAG, "time goes foward from %d to %d...", this->now_, this->now_+1);
	}
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
	if(this->log().t()){
		this->log().t(TAG, "Discarding future from %d to %d... now: %d", this->now_, this->last_, this->now_);
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
	if(this->log().t()){
		this->log().t(TAG, "Discarding history from %d to %d... now: %d", this->first_, this->now_, this->now_);
	}
	if(Handler<Donut> donut = this->donut_.lock()){
		donut->onDiscardHistoryNotify();
	}else{
		log().e(TAG, "Tried to discard future, but donut was already dead.");
	}
	this->first_ = this->now_;
}


}
