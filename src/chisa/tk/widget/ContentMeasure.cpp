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

#include "ContentWidget.h"
#include "Content/Node.h"

namespace chisa {
namespace tk {
namespace widget {

ContentMeasure::ContentMeasure(float const width) noexcept
:lineHeight_(0.0f)
,widgetWidth_(width)
,boxWidth_(width)
{
	this->pt_.x(0.0f);
	this->pt_.y(0.0f);
}

ContentMeasure::BoxSession::BoxSession(ContentMeasure& parent, const Margin& m)
:parent_(parent)
,margin_(m)
{
	parent.pt_.x(0);
}
ContentMeasure::BoxSession::~BoxSession() noexcept
{

}

void ContentMeasure::extend(float width, float lineHeight)
{
	float newX = this->pt_.x() + width;
	float maxLineHeight = std::max(lineHeight, this->lineHeight_);
	if(newX <= this->boxWidth_){
		this->pt_.x(newX);
		this->lineHeight_ = maxLineHeight;
	}else{
		newX -= this->boxWidth_;
		this->pt_.x(newX);
		this->pt_.y(this->pt_.y() + maxLineHeight);
		this->lineHeight_ = lineHeight;
	}
}

void ContentMeasure::walk(Document* model)
{
	this->margin_ = model->margin();
}

void ContentMeasure::walk(Paragraph* model)
{
}

void ContentMeasure::walk(Heading* model)
{
}

void ContentMeasure::walk(Link* model)
{
}

void ContentMeasure::walk(Text* model)
{
}

}}}
