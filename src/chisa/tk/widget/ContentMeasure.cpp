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

namespace chisa {
namespace tk {
namespace widget {

ContentMeasure::ContentMeasure(int const width) noexcept
:width_(width)
,lineHeight_(0)
{
}

void ContentMeasure::nextLine()
{
	this->pt_.y(this->pt_.y()+this->lineHeight_);
	this->pt_.x(0);
}

void ContentMeasure::walk(std::shared_ptr<Node> model)
{
}

void ContentMeasure::walk(Document* model)
{
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
