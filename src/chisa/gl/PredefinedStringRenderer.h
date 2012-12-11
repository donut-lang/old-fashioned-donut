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

#pragma once
#include "../util/ClassUtil.h"
#include "../geom/Decl.h"
#include "DrawableManager.h"

namespace chisa {
namespace gl {
class Canvas;

class PredefinedStringRenderer {
	DISABLE_COPY_AND_ASSIGN(PredefinedStringRenderer);
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private:
	Handler<DrawableManager> drawableManager_;
	std::map<unsigned int, Handler<Sprite> > spriteTable_;
public:
	typedef std::basic_string<unsigned int> String;
	static const constexpr unsigned int EOS = 0U;
	PredefinedStringRenderer(logging::Logger& log, Handler<DrawableManager> drawableManager);
	virtual ~PredefinedStringRenderer() noexcept = default;
public:
	void registerCharacter( unsigned int symbol, std::string const& str );
	geom::Area renderString( Canvas& cv, geom::Point const& point, String const& str, float depth );
};

}}
