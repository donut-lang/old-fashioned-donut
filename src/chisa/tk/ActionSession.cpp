/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Action.hpp"
#include "World.hpp"
#include "Element.hpp"
#include <algorithm>

namespace chisa {
namespace tk {

static const std::string TAG("GestureSession");

ActionSession::ActionSession(Logger& log, const unsigned int pointerIndex, HandlerW<Element> targetElement, geom::Point const& startPoint, const float startTimeMs)
:log_(log)
,target_(targetElement)
,pointerIndex_(pointerIndex)
,startPoint_(startPoint)
,startTimeMs_(startTimeMs)
,lastPoint_(startPoint)
,lastTimeMs_(startTimeMs)
,totalMoved_(0,0)
{
	const Handler<Element> orig_target = targetElement.lock();
	if(!orig_target){
		log.e(TAG, "[Touch Session %d] oops. Target Element was already deleted.", this->pointerIndex_);
		return;
	}
	//このセッションに関わるレイアウトを列挙
	Handler<Element> _it = orig_target;
	while( _it ) {
		this->elementChain_.push_back( _it );
		_it = _it->parent().lock();
	}
	std::reverse(this->elementChain_.begin(), this->elementChain_.end());
	for(ElementIterator it = this->elementChain_.begin(); it != this->elementChain_.end(); ++it){
		Handler<Element> target = it->lock();
		if(log.t()){
			log.t(TAG, "Touch Session creating: %s at %f index: %d element: %s", startPoint.toString().c_str(), startTimeMs, pointerIndex, target->toString().c_str());
		}
		if(target->onDownRaw(this->startTimeMs_, this->startPoint_)){
			//onDownイベントをconsumeした先には一切イベントを分け与えない。
			if(it+1 != this->elementChain_.end()){
				this->elementChain_.erase(it+1, this->elementChain_.end());
				HandlerW<Element>().swap(this->target_);
			}
			break;
		}
	}
	this->elementChainSorted_.insert(this->elementChainSorted_.end(), this->elementChain_.begin(), this->elementChain_.end());
	std::sort(this->elementChainSorted_.begin(), this->elementChainSorted_.end());
}

ActionSession::~ActionSession()
{
}

void ActionSession::onTouchUp(float const& timeMs, geom::Point const& pt)
{
	this->lastTimeMs_ = timeMs;
	this->lastPoint_ = pt;
	this->invokeUpRaw(timeMs, pt);

	this->totalMoved_ += geom::abs(pt-this->lastPoint_);

	const float timeDiff = this->lastTimeMs_-this->startTimeMs_;
	const geom::Velocity vel(this->totalMoved_, timeDiff);
	if(fabs(vel.x()) > MinFlingVelocity || fabs(vel.y()) > MinFlingVelocity ){
		this->invokeFling(timeMs, this->startPoint_, pt, vel);
		return;
	}
	Handler<Element> target;
	if( totalMoved_.x() < MaxSingleTapupDistance && totalMoved_.y() < MaxSingleTapupDistance && (target = this->target_.lock()) ){
		if(target->lastDrawnAreaInRoot().contain(pt)){
			target->onSingleTapUp(timeMs, pt);
			return;
		}
	}
}

void ActionSession::onTouchMove(float const& timeMs, geom::Point const& pt)
{
	this->invokeMoveRaw(timeMs, pt);
	this->invokeScroll(timeMs, this->lastPoint_, pt, pt-this->lastPoint_);
	this->totalMoved_ += geom::abs(pt-this->lastPoint_);
	this->lastTimeMs_ = timeMs;
	this->lastPoint_ = pt;
}

void ActionSession::onScroll(float const& timeMs, float const ratio)
{
	geom::Distance d(0, ratio);
	this->lastTimeMs_ = timeMs;
	this->invokeScroll(timeMs, this->lastPoint_, this->lastPoint_, d);
}

void ActionSession::onTextInput(float const& timeMs, std::string const& text)
{
	this->invokeTextInput(timeMs, text);
}
void ActionSession::onTextEdit(float const& timeMs, std::string const& text, int const& start, int const& length)
{
	this->invokeTextEdit(timeMs, text, start, length);
}

void ActionSession::onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym)
{
	this->invokeKeyDown(timeMs, isRepeat, sym);
}
void ActionSession::onKeyUp(float const& timeMs, SDL_Keysym const& sym)
{
	this->invokeKeyUp(timeMs, sym);
}

void ActionSession::onFocusGained(float const& timeMs, ActionSession* const last)
{
	this->lastTimeMs_ = timeMs;
	this->invokeFocusGained(timeMs,last);
}
void ActionSession::onFocusLost(float const& timeMs, ActionSession* const last)
{
	this->lastTimeMs_ = timeMs;
	this->invokeFocusLost(timeMs,last);
}

void ActionSession::invokeDownRaw(float const& timeMs, geom::Point const& pt)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			if(this->log().t()){
				this->log().t(TAG, "Touch Session creating: %s at %f index: %d element: %s", pt.toString().c_str(), this->startTimeMs_, this->pointerIndex_, target->toString().c_str());
			}
			if(target->onDownRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void ActionSession::invokeUpRaw(float const& timeMs, geom::Point const& pt)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			if(this->log().t()){
				this->log().t(TAG, "Touch Session ending: %s at %f index: %d element: %s", pt.toString().c_str(), this->lastTimeMs_, this->pointerIndex_, target->toString().c_str());
			}
			if(target->onUpRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void ActionSession::invokeMoveRaw(float const& timeMs, geom::Point const& pt)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			if(target->onMoveRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void ActionSession::invokeFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			if(target->onFling(timeMs, start, end, velocity)){
				break;
			}
		}
	}
}

void ActionSession::invokeScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			if(target->onScroll(timeMs, start, end, distance)){
				break;
			}
		}
	}
}



void ActionSession::invokeZoom(float const& timeMs, geom::Point const& center, const float ratio)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			if(target->onZoom(timeMs, center, ratio)){
				break;
			}
		}
	}
}

void ActionSession::invokeFocusGained(float const& timeMs, ActionSession* const last)
{
	std::vector<HandlerW<Element> > gained;
	if(last){
		std::set_difference(this->elementChainSorted_.begin(), this->elementChainSorted_.end(), last->elementChainSorted_.begin(), last->elementChainSorted_.end(), std::back_inserter(gained));
	}
	std::vector<HandlerW<Element> > const& targets(last ? gained : this->elementChain_);
	for(HandlerW<Element> const& it : targets) {
		if(Handler<Element> target = it.lock()){
			target->onFocusGained(timeMs, this->lastPoint_);
		}
	}
}

void ActionSession::invokeFocusLost(float const& timeMs, ActionSession* const last)
{
	std::vector<HandlerW<Element> > lost;
	if(last){
		std::set_difference(this->elementChainSorted_.begin(), this->elementChainSorted_.end(), last->elementChainSorted_.begin(), last->elementChainSorted_.end(), std::back_inserter(lost));
	}
	std::vector<HandlerW<Element> > const& targets(last ? lost : this->elementChain_);
	for(HandlerW<Element> const& it : targets) {
		if(Handler<Element> target = it.lock()){
			target->onFocusLost(timeMs);
		}
	}
}

void ActionSession::invokeTextInput(float const& timeMs, std::string const& text)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			target->onTextInput(timeMs, text);
		}
	}
}

void ActionSession::invokeTextEdit(float const& timeMs, std::string const& text, int const start, int const length)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			target->onTextEdit(timeMs, text, start, length);
		}
	}
}

void ActionSession::invokeKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			if(target->onKeyDown(timeMs, isRepeat, sym)){
				break;
			}
		}
	}
}
void ActionSession::invokeKeyUp(float const& timeMs, SDL_Keysym const& sym)
{
	for(HandlerW<Element> const& it : this->elementChain_) {
		if(Handler<Element> target = it.lock()){
			if(target->onKeyUp(timeMs, sym)){
				break;
			}
		}
	}
}
}}
