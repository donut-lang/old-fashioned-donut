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

#include "NesScreenWidget.h"
#include "../NesGeist.h"
#include "../../chisa/tk/World.h"
#include "../../chisa/geom/Vector.h"
#include "../../chisa/geom/Area.h"

namespace nes {

NesScreenWidget::NesScreenWidget(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
,conf_(new XObject)
,lastState_(true)
{
	chisa::Handler<chisa::tk::World> world(_world.lock());
	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;
	this->conf_->accumlate(element->FirstChildElement());
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
	if( Handler<NesGeist> geist = this->geist_.lock() ) {
		if(lastState_) {
			geist->startNES();
		}else{
			geist->stopNES();
		}
	}
}

void NesScreenWidget::onHiddenImpl()
{
	if( Handler<NesGeist> geist = this->geist_.lock() ) {
		if((lastState_ = geist->isRunning())){
			geist->stopNES();
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
