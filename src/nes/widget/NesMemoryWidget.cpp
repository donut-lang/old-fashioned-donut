/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "NesMemoryWidget.h"
#include "../../chisa/tk/World.h"
#include "../NesGeist.h"
#include "../../chisa/geom/Vector.h"
#include "../../chisa/geom/Area.h"
#include "../../chisa/gl/DrawableManager.h"

namespace nes {

NesMemoryWidget::NesMemoryWidget(chisa::Logger& log, chisa::HandlerW<chisa::tk::World> _world, tinyxml2::XMLElement* element)
:Widget(log, _world, element)
,numRenderer_(log, _world.lock()->drawableManager(), 32)
{
	chisa::Handler<chisa::tk::World> world(_world.lock());
	chisa::Handler<nes::NesGeist> geist(world->geist().cast<nes::NesGeist>());
	this->geist_ = geist;

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
	numRenderer_.registerSymbol(16, "$");
	numRenderer_.compile();

	this->addrWidth_ = numRenderer_.maxWidth()*(1+4+1);
}

void NesMemoryWidget::renderImpl(chisa::gl::Canvas& cv, chisa::geom::Area const& area)
{
	using namespace chisa;
	Handler<NesGeist> geist = this->geist_.lock();
	float const symwidth(numRenderer_.maxWidth());
	float const dwidth(symwidth*2+2);
	float const dheight(numRenderer_.maxHeight()+2);

	int const rowStart = geom::max(0, area.y() / dheight);
	int const colStart = geom::max(0, (area.x()-this->addrWidth_) / dwidth );
	int const rowEnd = std::ceil( (area.y()+area.height()) / dheight );
	int const colEnd = geom::min(15, std::ceil( (area.x()+area.width() - this->addrWidth_) / dwidth ));


	std::vector<unsigned int> addr_sym(4);
	std::vector<unsigned int> num(2);
	for(int y=rowStart; y <= rowEnd;++y) {
		float const rowOffset = y*dheight;
		cv.drawLine(2, gl::White, geom::Point(0,rowOffset), geom::Point(area.width(),rowOffset));

		this->numRenderer_.renderSym(cv, geom::Point(0,rowOffset), 16, 0.0f);

		uint16_t const baseaddr = (y*16);
		addr_sym[0] = ((baseaddr) >> 12) & 15;
		addr_sym[1] = ((baseaddr) >> 8) & 15;
		addr_sym[2] = ((baseaddr) >> 4) & 15;
		addr_sym[3] = ((baseaddr) & 15);
		geom::Point pt (symwidth, rowOffset);
		this->numRenderer_.renderSyms(cv, pt, addr_sym, 0.0f);

		for(int x=colStart; x <= colEnd;++x) {
			geom::Point pt (this->addrWidth_ + x*dwidth, rowOffset);
			uint16_t const addr = (y*16)+x;
			uint8_t const val = geist->machine()->debuggerRead(addr);
			num[0] = (val >> 4);
			num[1] = (val >> 0) & 0xf;
			this->numRenderer_.renderSyms(cv, pt, num, 0.0f);
		}
	}
	numRenderer_.flush(cv);
	cv.fillRect(gl::White, geom::Area(this->addrWidth_-symwidth,area.y(), symwidth, area.height()));
	for(int x=colStart; x <= colEnd;++x) {
		float const colOffset = this->addrWidth_ + (x+1)*dwidth-1;
		cv.drawLine(2, gl::White, geom::Point(colOffset,0), geom::Point(colOffset,area.height()));
	}
}

void NesMemoryWidget::idleImpl(const float delta_ms)
{
}

void NesMemoryWidget::reshapeImpl(chisa::geom::Box const& areaSize)
{
}

chisa::geom::Box NesMemoryWidget::measureImpl(chisa::geom::Box const& constraintSize)
{
	chisa::geom::Box memAreaSize((this->numRenderer_.maxWidth()+1)*32+this->addrWidth_, (this->numRenderer_.maxHeight()+2)*(65536/16)-2);
	return memAreaSize;
}

chisa::geom::Area NesMemoryWidget::findTargetImpl(const std::string& target)
{
	return addrToArea( ::cinamo::parseAs<uint16_t>(target, 0) );
}

chisa::geom::Area NesMemoryWidget::addrToArea(uint16_t const& addr)
{
	float const symwidth(numRenderer_.maxWidth());
	float const dwidth(symwidth*2+2);
	float const dheight(numRenderer_.maxHeight()+2);
	return chisa::geom::Area( this->addrWidth_+((addr&0xf) * dwidth), dheight * (addr/16), dwidth, dheight );
}
uint16_t NesMemoryWidget::ptToAddr(chisa::geom::Point const& pt)
{
	float const symwidth(numRenderer_.maxWidth());
	float const dwidth(symwidth*2+2);
	float const dheight(numRenderer_.maxHeight()+2);
	int const x = std::max(0, static_cast<int>((pt.x()-addrWidth_)/dwidth));
	int const y = std::min(std::max(0,static_cast<int>((pt.y()/dheight))), 0xffff);
	return
		x | y << 4;
}

bool NesMemoryWidget::onSingleTapUp(const float& timeMs, const chisa::geom::Point& ptInWidget)
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
