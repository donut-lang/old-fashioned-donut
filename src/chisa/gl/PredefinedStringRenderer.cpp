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

#include <tarte/Logger.h>

#include "PredefinedStringRenderer.h"
#include "Canvas.h"

namespace chisa {
namespace gl {

static const std::string TAG("PredefinedStringRenderer");

PredefinedStringRenderer::PredefinedStringRenderer(Logger& log, Handler<DrawableManager> drawableManager)
:log_(log)
,drawableManager_(drawableManager)
,maxWidth_(0)
,maxHeight_(0)
{

}

void PredefinedStringRenderer::registerCharacter( unsigned int symbol, std::string const& str )
{
	auto it = this->spriteTable_.find(symbol);
	if(it != this->spriteTable_.end()){
		this->log().w(TAG, "Symbol: %d is already defined, and replaced with: %s", symbol, str.c_str());
		this->spriteTable_.erase(it);
	}
	Handler<Sprite> spr = drawableManager_->queryText(str)->sprite();
	this->maxWidth_ = geom::max(spr->width(), maxWidth_);
	this->maxHeight_ = geom::max(spr->height(), maxHeight_);
	this->spriteTable_.insert( std::pair<unsigned int, Handler<Sprite> >( symbol, spr ) );
}

geom::Area PredefinedStringRenderer::renderString( Canvas& cv, geom::Point const& point, String const& str, float depth)
{
	float x=0.0f;
	for(unsigned int symbol : str){
		auto it = this->spriteTable_.find(symbol);
		if(it == this->spriteTable_.end()){
			this->log().w(TAG, "Unknown symbol: %d", symbol);
			continue;
		}
		Handler<Sprite> const& spr( it->second );
		geom::Point rendered( point.x()+x, point.y()+(this->maxHeight_-spr->height())/2 );
		cv.drawSprite(spr, rendered, depth);
		x += spr->width();
	}
	return geom::Area(point, geom::Box(x,this->maxHeight_));
}


}}
