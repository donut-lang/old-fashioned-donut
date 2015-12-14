/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/Exception.h>

#include "PostDisposer.hpp"
#include "../node/Node.hpp"

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
