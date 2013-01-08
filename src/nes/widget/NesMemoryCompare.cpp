/**
 * Chisa
 * Copyright (C) 2013 psi
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

#include "NesMemoryCompare.h"
#include "../../chisa/tk/World.h"
#include "../../chisa/gl/DrawableManager.h"
#include "../NesGeist.h"

namespace nes {

NesMemoryCompare::NesMemoryCompare(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
,numRenderer_(log, _world.lock()->drawableManager(), 16)
{
	using namespace chisa::tk;
	using namespace chisa::gl;
	Handler<World> world = _world.lock();
	Handler<DrawableManager> dm = world->drawableManager();
	// TODO Auto-generated constructor stub
	this->lAddr_ = dm->queryText("アドレス", 16);
	this->lLast_ = dm->queryText("前の値", 16);
	this->lNow_ = dm->queryText("今の値", 16);

	numRenderer_.registerSymbol(0, "0");
	numRenderer_.registerSymbol(1, "1");
	numRenderer_.registerSymbol(2, "2");
	numRenderer_.registerSymbol(3, "3");
	numRenderer_.registerSymbol(4, "4");
	numRenderer_.registerSymbol(5, "5");
	numRenderer_.registerSymbol(6, "6");
	numRenderer_.registerSymbol(7, "7");
	numRenderer_.registerSymbol(8, "8");
	numRenderer_.registerSymbol(9, "9");
	numRenderer_.registerSymbol(10, "a");
	numRenderer_.registerSymbol(11, "b");
	numRenderer_.registerSymbol(12, "c");
	numRenderer_.registerSymbol(13, "d");
	numRenderer_.registerSymbol(14, "e");
	numRenderer_.registerSymbol(15, "f");
	numRenderer_.registerSymbol(Dollar, "$");


	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;

	{
		float const numLen = this->numRenderer_.maxCharWidth();
		this->addrWidth_ = chisa::geom::max(this->lAddr_->width(), numLen * 5);
		this->lastWidth_ = chisa::geom::max(this->lLast_->width(), numLen * 2);
		this->nowWidth_ = chisa::geom::max(this->lNow_->width(), numLen * 2);
		this->width_ = addrWidth_ + 2 + lastWidth_ + 2 + nowWidth_;
	}
}

void NesMemoryCompare::render(chisa::gl::Canvas& cv, const chisa::geom::Area& area)
{
	using namespace chisa::geom;
	using namespace chisa::gl;
	Handler<NesGeist> geist = this->geist_.lock();
	if(!geist){
		return;
	}
	Debugger& dbg = geist->machine()->debugger();
	MemoryComparator& cmp = dbg.comparator();

	unsigned int const startRow = max(std::floor((area.y() - rowHeight())/rowHeight()), 0);
	unsigned int const endRow = min(std::ceil((area.y() - rowHeight() + area.height())/rowHeight()), cmp.candidates());

	unsigned int idx = 0;
	unsigned int drawn = 0;
	PredefinedSymRenderer::SymList asym_(5);
	asym_[0] = Dollar;
	PredefinedSymRenderer::SymList sym_(2);
	for(unsigned int i=0; i<2048;++i) {
		if(cmp.isCandidate(i)) {
			if(  idx >= startRow && idx <= endRow ) {
				float const offset = rowHeight() * (1+drawn);
				{ //addr
					asym_[1] = (i >> 12) & 0xf;
					asym_[2] = (i >>  8) & 0xf;
					asym_[3] = (i >>  4) & 0xf;
					asym_[4] = (i >>  0) & 0xf;
					this->numRenderer_.renderSyms(cv, Point(0, offset), asym_, 0);
				}
				{ //last
					uint8_t const v = cmp.last(i);
					sym_[0] = (v >>  4) & 0xf;
					sym_[1] = (v >>  0) & 0xf;
					this->numRenderer_.renderSyms(cv, Point(addrWidth_+2, offset), sym_, 0);
				}
				{ //now
					uint8_t const v = cmp.now(i);
					sym_[0] = (v >>  4) & 0xf;
					sym_[1] = (v >>  0) & 0xf;
					this->numRenderer_.renderSyms(cv, Point(addrWidth_+2+lastWidth_+2, offset), sym_, 0);
				}
				++drawn;
				++idx;
			}else if( idx > endRow ){
				break;
			}
		}
	}

	{ //ラベル
		this->lAddr_->draw(cv, Point((this->addrWidth_-this->lAddr_->width())/2, area.y()));
		this->lLast_->draw(cv, Point(this->addrWidth_ + 2 + (this->lastWidth_ - this->lLast_->width())/2,area.y()));
		this->lNow_->draw(cv, Point(this->addrWidth_ + 2 + this->lastWidth_ + 2 + (this->nowWidth_ - this->lNow_->width())/2,area.y()));
		cv.drawLine(2, DarkGray, Point(addrWidth_, area.y()), Point(addrWidth_, rowHeight()+area.y()));
	}
	cv.drawLine(2, DarkGray, Point(addrWidth_ + lastWidth_, area.y()), Point(addrWidth_ + lastWidth_, area.y()+area.height()));
	cv.drawLine(2, DarkGray, Point(0, area.y()), Point(width_, area.y()+area.height()));
}

void NesMemoryCompare::idle(const float delta_ms)
{
}

void NesMemoryCompare::reshape(const chisa::geom::Box& areaSize)
{
}

chisa::geom::Box NesMemoryCompare::measure(const chisa::geom::Box& constraintSize)
{
	MemoryComparator& cmp = this->geist_.lock()->machine()->debugger().comparator();
	float const height = rowHeight() *  (cmp.candidates()+1);
	return chisa::geom::Box( width_, height);
}

}
