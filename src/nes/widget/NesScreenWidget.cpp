/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "NesScreenWidget.hpp"
#include "../NesGeist.hpp"
#include "../../chisa/tk/World.hpp"
#include "../../chisa/geom/Vector.hpp"
#include "../../chisa/geom/Area.hpp"

namespace nes {

NesScreenWidget::NesScreenWidget(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
,conf_(new XObject)
,lastState_(true)
{
	chisa::Handler<chisa::tk::World> world(_world.lock());
	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;
	if(element->FirstChildElement()){
		this->conf_->accumlate(element->FirstChildElement());
	}
	if(this->conf_->has<XString>("rom")){
		geist->loadNES(this->world().lock()->resolveUniverseFilepath(this->conf_->get<XString>("rom")));
	}
}

void NesScreenWidget::renderImpl(chisa::gl::Canvas& cv, chisa::geom::Area const& area)
{
	chisa::Handler<nes::NesGeist> geist = this->geist_.lock();
	if(!geist){
		return;
	}
	nes::NesGeist::VideoLock lock(*geist.get());
	cv.drawSprite(lock.getSprite(), chisa::geom::Vector(0.0f, 0.0f));
}

void NesScreenWidget::idleImpl(const float delta_ms)
{
}

void NesScreenWidget::reshapeImpl(chisa::geom::Box const& areaSize)
{

}

chisa::geom::Box NesScreenWidget::measureImpl(chisa::geom::Box const& constraintSize)
{
	return chisa::geom::Box(256,240);
}

void NesScreenWidget::onFocusGained(const float& timeMs, const chisa::geom::Point& lastPtInScreen)
{
	chisa::Handler<nes::NesGeist> geist = this->geist_.lock();
	if(!geist){
		return;
	}
	geist->gamepad().onFocusGained();
}

void NesScreenWidget::onFocusLost(const float& timeMs)
{
	chisa::Handler<nes::NesGeist> geist = this->geist_.lock();
	if(!geist){
		return;
	}
	geist->gamepad().onFocusLost();
}

bool NesScreenWidget::onKeyDown(const float& timeMs, bool isRepeat, const SDL_Keysym& sym)
{
	chisa::Handler<nes::NesGeist> geist = this->geist_.lock();
	if(geist){
		geist->gamepad().onKeyDown(isRepeat, sym);
	}
	return true;
}

bool NesScreenWidget::onKeyUp(const float& timeMs, const SDL_Keysym& sym)
{
	chisa::Handler<nes::NesGeist> geist = this->geist_.lock();
	if(geist){
		geist->gamepad().onKeyUp(sym);
	}
	return true;
}

void NesScreenWidget::onShownImpl()
{
	if(this->conf_->has<XString>("rom")){
		if( Handler<NesGeist> geist = this->geist_.lock() ) {
			if(lastState_) {
				geist->startNES();
			}else{
				geist->stopNES();
			}
		}
	}
}

void NesScreenWidget::onHiddenImpl()
{
	if(this->conf_->has<XString>("rom")){
		if( Handler<NesGeist> geist = this->geist_.lock() ) {
			if((lastState_ = geist->isRunning())){
				geist->stopNES();
			}
		}
	}
}

chisa::geom::Area NesScreenWidget::findTargetImpl(const std::string& target)
{
	int x;
	int y;
	int w = 0;
	int h = 0;
	if(sscanf(target.c_str(), "%dx%dx%dx%d", &x, &y, &w, &h) < 2) {
		return chisa::geom::Area();
	}
	return chisa::geom::Area(x,y,w,h);
}

}
