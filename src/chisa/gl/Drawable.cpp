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

#include "Drawable.h"
#include "../geom/Area.h"
#include "../geom/Vector.h"
#include "../gl/Canvas.h"

namespace chisa {
namespace gl {

float Drawable::width() const noexcept
{
	return this->size().width();
}
float Drawable::height() const noexcept
{
	return this->size().height();
}

//-----------------------------------------------------------------------------

ColorDrawable::ColorDrawable(const Color& c, const geom::Box size)
:color_(c), size_(size)
{

}

Color ColorDrawable::color() const noexcept
{
	return this->color_;
}
geom::Box ColorDrawable::size() const noexcept
{
	return this->size_;
}

void ColorDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	canvas.fillRect(this->color_, area.intersect(geom::Area(area.point(), size_)), depth);
}

//-----------------------------------------------------------------------------

SpriteDrawable::SpriteDrawable(Handler<gl::Sprite> spr)
:sprite_(spr)
{
}

Handler<gl::Sprite> SpriteDrawable::sprite() const
{
	return this->sprite_;
}

geom::Box SpriteDrawable::size() const noexcept
{
	return this->sprite_ ? geom::Box(this->sprite_->width(), this->sprite_->height()) : geom::Box();
}

void SpriteDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	canvas.drawSprite(this->sprite_, area.point(), geom::Area(geom::ZERO, area.box()), depth);
}

//-----------------------------------------------------------------------------

RepeatDrawable::RepeatDrawable(Handler<Drawable> child, const geom::Box& size)
:child_(child)
,size_(size)
{

}

Handler<Drawable> RepeatDrawable::child() const
{
	return this->child_;
}

geom::Box RepeatDrawable::size() const noexcept
{
	return this->child_ ? this->child_->size() : geom::Box();
}

void RepeatDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	geom::Area rendered(area.intersect(geom::Area(area.point(), this->size_)));
	float y=rendered.y();
	const float mx = rendered.width()+rendered.x();
	const float my = rendered.height()+rendered.y();
	const float cy = this->child_->height();
	const float cx = this->child_->width();
	while(y < my){
		float x=rendered.x();
		while(x < mx){
			this->child_->draw(canvas, geom::Area(x,y, geom::min(cx,mx-x), geom::min(cy,my-y)),depth);
			x+=cy;
		}
		y += cy;
	}
}

//-----------------------------------------------------------------------------

StretchDrawable::StretchDrawable(Handler<Drawable> child, const geom::Box& size)
:child_(child)
,size_(size)
{

}

Handler<Drawable> StretchDrawable::child() const
{
	return this->child_;
}

geom::Box StretchDrawable::size() const noexcept
{
	return this->child_ ? this->child_->size() : geom::Box();
}

void StretchDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	geom::Area rendered(area.intersect(geom::Area(area.point(), this->size_)));
	Canvas::AffineScope as(canvas);
	{
		canvas.translate(area.point());
		float const scaleX = this->child_->width() / this->size_.width();
		float const scaleY = this->child_->height() / this->size_.height();
		canvas.scale(geom::ScaleVector(scaleX, scaleY));
		this->child_->draw(canvas, geom::Area(geom::ZERO, this->child_->size()), depth);
	}
}

}}
