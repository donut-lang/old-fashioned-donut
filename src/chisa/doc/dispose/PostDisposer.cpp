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

#include <cinamo/Exception.h>

#include "PostDisposer.h"
#include "../node/Node.h"

namespace chisa {
namespace doc {

PostDisposer::PostDisposer(Logger& log)
:log_(log)
{
	this->offsetStack_.push_back(geom::Point(0,0));
}

geom::Point PostDisposer::offset()
{
	if(this->offsetStack_.size() <= 0){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. Offset stack is empty.");
	}
	return this->offsetStack_.back();
}

template <typename T, typename U>
void PostDisposer::walkBlock(T* block, U clos)
{
	geom::Box const offset = this->offset();
	block->area(geom::Area( offset + block->areaInBlock().point(), block->areaInBlock().box() ));
	this->offsetStack_.push_back( offset + block->areaInBlock().point() + block->margin().offset());

	clos();
	this->walkChildren(block);

	block->background()->area(geom::Area(this->offset(), block->areaInBlock().box() - block->margin().totalSpace()));
	this->offsetStack_.pop_back();
}


void PostDisposer::walk(Document* doc)
{
	this->walkBlock(doc, [](){});
}

void PostDisposer::walk(Paragraph* para)
{
	this->walkBlock(para, [](){});
}

void PostDisposer::walk(Heading* head)
{
	this->walkBlock(head, [](){});
}

void PostDisposer::walk(Text* text)
{
	geom::Distance const offset(this->offset());
	for(Handler<TextDrawableObject> obj : text->objects()){
		geom::Area area(obj->area().point() + offset, obj->area().box());
		obj->area(area);
	}
}

void PostDisposer::walk(Link* link)
{
	this->walkChildren(link);
}

void PostDisposer::walk(Font* font)
{
	this->walkChildren(font);
}

void PostDisposer::walk(BreakLine* br)
{
	this->walkChildren(br);
}

}}
