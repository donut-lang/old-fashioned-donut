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
#include "../../chisa/tk/element/WidgetElement.h"
#include "../../chisa/gl/DrawableManager.h"
#include "../NesGeist.h"

namespace nes {

NesMemoryCompare::NesMemoryCompare(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
,numRenderer_(log, _world.lock()->drawableManager(), 16)
,lastCandidates_(0)
{
	using namespace chisa::tk;
	using namespace chisa::gl;
	Handler<World> world = _world.lock();
	Handler<DrawableManager> dm = world->drawableManager();
	this->lAddr_ = dm->queryText("アドレス", 16);
	this->lLast_ = dm->queryText("記録メモ", 16);
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
	float const numWidth = this->numRenderer_.maxWidth();
	numRenderer_.registerSymbol(HexStart, "0x");
	float const hexWidth = this->numRenderer_.maxWidth();
	numRenderer_.registerSymbol(Dollar, "$");
	numRenderer_.registerSymbol(Eq, "=");
	numRenderer_.registerSymbol(LParen, "(");
	numRenderer_.registerSymbol(RParen, ")");
	numRenderer_.registerSymbol(Space, " ");
	numRenderer_.compile();

	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;

	{
		this->addrWidth_ = chisa::geom::max(this->lAddr_->width()+2, numWidth * 5);
		this->lastWidth_ = chisa::geom::max(this->lLast_->width()+5, hexWidth+numWidth * 8);
		this->nowWidth_ = chisa::geom::max(this->lNow_->width()+5, hexWidth+numWidth * 8);
		this->width_ = addrWidth_ + 2 + lastWidth_ + 2 + nowWidth_;
	}
}

void NesMemoryCompare::renderImpl(chisa::gl::Canvas& cv, const chisa::geom::Area& area)
{
	using namespace chisa::geom;
	using namespace chisa::gl;
	Handler<NesGeist> geist = this->geist_.lock();
	Handler<chisa::tk::WidgetElement> parent = this->wrapper().lock();
	if(!geist || !parent){
		return;
	}
	Debugger& dbg = geist->machine()->debugger();
	MemoryComparator& cmp = dbg.comparator();
	const float rowHeight = this->rowHeight();

	int const startRow = max(std::floor((area.y() - rowHeight)/rowHeight), 0);
	int const endRow = min(max(std::ceil((area.y() - rowHeight + area.height())/rowHeight),0), cmp.candidates());

	int idx = 0;
	int drawn = 0;
	PredefinedSymRenderer::SymList asym_(5);
	asym_[0] = Dollar;
	PredefinedSymRenderer::SymList sym_(9);
	for(unsigned int i=0; i<2048;++i) {
		if(cmp.isCandidate(i)) {
			if(  idx >= startRow && idx <= endRow ) {
				float const offset = rowHeight * (1+idx);
				{ //addr
					asym_[1] = (i >> 12) & 0xf;
					asym_[2] = (i >>  8) & 0xf;
					asym_[3] = (i >>  4) & 0xf;
					asym_[4] = (i >>  0) & 0xf;
					this->numRenderer_.renderSyms(cv, Point(0, offset), asym_, 0);
				}
				{ //last
					uint8_t const v = cmp.last(i);
					sym_[0] = HexStart;
					sym_[1] = (v >>  4) & 0xf;
					sym_[2] = (v >>  0) & 0xf;
					sym_[3] = LParen;
					sym_[4] = Eq;
					uint8_t vv = v;
					sym_[7] = (vv % 10);
					vv /= 10;
					sym_[6] = (vv % 10) > 0 || vv >= 10 ? vv%10 : Space;
					vv /= 10;
					sym_[5] = (vv % 10) > 0 ? vv%10 : Space;
					sym_[8] = RParen;
					this->numRenderer_.renderSyms(cv, Point(addrWidth_+2, offset), sym_, 0);
				}
				{ //now
					uint8_t const v = cmp.now(i);
					sym_[0] = HexStart;
					sym_[1] = (v >>  4) & 0xf;
					sym_[2] = (v >>  0) & 0xf;
					sym_[3] = LParen;
					sym_[4] = Eq;
					uint8_t vv = v;
					sym_[7] = (vv % 10);
					vv /= 10;
					sym_[6] = (vv % 10) > 0 || vv >= 10 ? vv%10 : Space;
					vv /= 10;
					sym_[5] = (vv % 10) > 0 ? vv%10 : Space;
					sym_[8] = RParen;
					this->numRenderer_.renderSyms(cv, Point(addrWidth_+2+lastWidth_+2, offset), sym_, 0);
				}
				++drawn;
			}else if( idx > endRow ){
				break;
			}
			++idx;
		}
	}
	numRenderer_.flush(cv);

	{ //ラベル
		cv.fillRect(parent->backgroundColor(), Area(ZERO, Point(width_, rowHeight+area.y())));
		this->lAddr_->draw(cv, Point((this->addrWidth_-this->lAddr_->width())/2, area.y()));
		this->lLast_->draw(cv, Point(this->addrWidth_ + 2 + (this->lastWidth_ - this->lLast_->width())/2,area.y()));
		this->lNow_->draw(cv, Point(this->addrWidth_ + 2 + this->lastWidth_ + 2 + (this->nowWidth_ - this->lNow_->width())/2,area.y()));
		cv.drawLine(2, DarkGray, Point(0, rowHeight+area.y()), Point(width_, rowHeight+area.y()));
	}
	cv.drawLine(2, DarkGray, Point(addrWidth_ + 2 + lastWidth_, area.y()), Point(addrWidth_ + 2 + lastWidth_, area.y()+area.height()));
	cv.drawLine(2, DarkGray, Point(addrWidth_, area.y()), Point(addrWidth_, area.y()+area.height()));
}

void NesMemoryCompare::idleImpl(const float delta_ms)
{
	MemoryComparator& cmp = this->geist_.lock()->machine()->debugger().comparator();
	if(	lastCandidates_ != cmp.candidates() ){
		lastCandidates_ = cmp.candidates();
		this->requestRelayout();
	}
}

void NesMemoryCompare::reshapeImpl(const chisa::geom::Box& areaSize)
{
}

chisa::geom::Area NesMemoryCompare::findTargetImpl(const std::string& target)
{
	uint16_t addr = ::tarte::parseAs<uint16_t>(target);
	return addrToArea(addr);
}

chisa::geom::Area NesMemoryCompare::addrToArea(const uint16_t& addr)
{
	const float rowHeight = this->rowHeight();
	MemoryComparator& cmp = this->geist_.lock()->machine()->debugger().comparator();
	if(!cmp.isCandidate(addr) || addr > 2048){
		return chisa::geom::Area(0,0,0,0);
	}
	int row=1;
	for(int i=0;i<2048;++i) {
		if( i == addr ){
			break;
		}else if(cmp.isCandidate(i)) {
			++row;
		}
	}
	return chisa::geom::Area(0, row*rowHeight, width_, rowHeight);
}

uint16_t NesMemoryCompare::ptToAddr(const chisa::geom::Point& pt)
{
	MemoryComparator& cmp = this->geist_.lock()->machine()->debugger().comparator();
	const float rowHeight = this->rowHeight();
	int row = std::max(0, static_cast<int>(pt.y()/rowHeight)-1);
	uint16_t last=0;
	for(int i=0;i<2048;++i) {
		if(cmp.isCandidate(i)){
			if(row == 0) {
				return i;
			}
			last = i;
			--row;
		}
	}
	return last;
}

chisa::geom::Box NesMemoryCompare::measureImpl(const chisa::geom::Box& constraintSize)
{
	MemoryComparator& cmp = this->geist_.lock()->machine()->debugger().comparator();
	float const height = rowHeight() *  (cmp.candidates()+1);
	lastCandidates_ = cmp.candidates();
	return chisa::geom::Box( width_, height);
}


bool NesMemoryCompare::onSingleTapUp(const float& timeMs, const chisa::geom::Point& ptInWidget)
{
	Handler<chisa::tk::World> world = this->world().lock();
	if( unlikely(!world) ) {
		return true;
	}
	uint16_t const addr = this->ptToAddr(ptInWidget);
	Handler<NesGeist> geist = this->geist_.lock();
	uint8_t val ( geist->machine()->debuggerRead(addr) );

	{
		Handler<XObject> obj(new XObject);
		obj->set("action","new-angel");
		obj->set("addr", addr);
		obj->set("val", val);

		world->sendMessage(obj);
	}

	return true;
}
}
