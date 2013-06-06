/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>

#include "Empty.h"
#include "ElementFactory.h"

namespace chisa {
namespace tk {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(Empty, Element)
{

}

Empty::~Empty() noexcept
{
}

void Empty::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* element)
{

}

std::string Empty::toString() const
{
	return ::cinamo::format("(Empty %p)",this);
}

void Empty::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	//何も描画しない
}

geom::Box Empty::measureImpl(geom::Box const& constraint)
{
	return geom::Box(geom::Unspecified, geom::Unspecified);
}

void Empty::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	//何もしない
}

}}
