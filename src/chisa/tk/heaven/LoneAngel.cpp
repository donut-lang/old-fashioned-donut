/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "../Heaven.h"
#include "LoneAngel.h"
#include "../donut/Patron.h"

namespace chisa {
namespace tk {

LoneAngel::LoneAngel(Handler<Heaven> const& heaven)
:Angel(heaven)
{
}

void LoneAngel::renderImpl(gl::Canvas& canvas)
{
}

void LoneAngel::idleImpl(const float delta_ms)
{
}

void LoneAngel::reshapeImpl(const geom::Area& area)
{
}

Handler<LoneAngel> LoneAngel::self() noexcept
{
	return Handler<LoneAngel>::__internal__fromRawPointerWithoutCheck(this);
}


Handler< ::donut::Object> LoneAngel::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->loneAngelProvider()->newInstance(heap, self()) );
}

Handler<AngelTarget> LoneAngel::attatchTarget(const Handler<AngelTarget>& target)
{
	if(numTargets() > 0) {
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. Lone angel can hold only one target.")
	}
	return this->Angel::attatchTarget(target);
}

Handler<AngelTarget> LoneAngel::detatchTarget(const Handler<AngelTarget>& target)
{
	return this->Angel::detatchTarget(target);
}

}}
