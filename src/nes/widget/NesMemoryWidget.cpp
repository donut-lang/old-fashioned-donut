/*
 * NesMemoryWidget.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: psi
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
,numRenderer_(log, _world.lock()->drawableManager())
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

	this->addrWidth_ = numRenderer_.maxWidth()*(1+4+1);
}

NesMemoryWidget::~NesMemoryWidget()
{
}

void NesMemoryWidget::render(chisa::gl::Canvas& cv, chisa::geom::Area const& area)
{
	using namespace chisa;
	Handler<NesGeist> geist = this->geist_.lock();
	float const symwidth(numRenderer_.maxWidth());
	float const dwidth(symwidth*2);
	float const dheight(numRenderer_.maxHeight());

	int const rowStart = geom::max(0, area.y() / dheight);
	int const colStart = geom::max(0, area.x()-this->addrWidth_ / dwidth );
	int const rowEnd = std::ceil( (area.y()+area.height()) / dheight );
	int const colEnd = std::ceil( (area.x()+area.width() - this->addrWidth_) / dwidth );


	std::vector<unsigned int> addr_sym(4);
	std::vector<unsigned int> num(2);
	for(int y=rowStart; y <= rowEnd;++y) {
		float const rowOffset = y*dheight;

		this->numRenderer_.renderSym(cv, geom::Point(0,rowOffset)-area.point(), 16, 0.0f);

		uint16_t const baseaddr = (y*16);
		addr_sym[0] = ((baseaddr) >> 12) & 7;
		addr_sym[1] = ((baseaddr) >> 8) & 7;
		addr_sym[2] = ((baseaddr) >> 4) & 7;
		addr_sym[3] = ((baseaddr) & 7);
		geom::Point pt (symwidth, rowOffset);
		this->numRenderer_.renderSyms(cv, pt-area.point(), addr_sym, 0.0f);

		for(int x=colStart; x <= colEnd;++x) {
			geom::Point pt (this->addrWidth_ + x*dwidth, rowOffset);
			uint16_t const addr = (y*16)+x;
			uint8_t const val = geist->machine()->read(addr);
			num[0] = val >> 4;
			num[1] = val & 7;
			this->numRenderer_.renderSyms(cv, pt-area.point(), num, 0.0f);
		}
	}
}

void NesMemoryWidget::idle(const float delta_ms)
{
}

void NesMemoryWidget::reshape(chisa::geom::Box const& areaSize)
{
}

chisa::geom::Box NesMemoryWidget::measure(chisa::geom::Box const& constraintSize)
{
	chisa::geom::Box memAreaSize(this->numRenderer_.maxWidth()*32+this->addrWidth_, this->numRenderer_.maxHeight()*(65535/16));
	return memAreaSize;
}

}
