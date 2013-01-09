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
#include <tarte/ClassUtil.h>
#include "../geom/Decl.h"
#include "DrawableManager.h"

namespace chisa {
using namespace tarte;
namespace gl {
class Canvas;

class PredefinedSymRenderer {
	DISABLE_COPY_AND_ASSIGN(PredefinedSymRenderer);
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	Handler<DrawableManager> drawableManager_;
	std::map<unsigned int, Handler<TextDrawable> > spriteTable_;
	float maxWidth_;
	float maxHeight_;
private:
	Handler<gl::Font> font_;
	float size_;
	gl::Color color_;
	gl::Color backgroundColor_;
public:
	typedef unsigned int Symbol;
	typedef std::vector<Symbol> SymList;
	PredefinedSymRenderer(Logger& log, Handler<DrawableManager> drawableManager, float size);
	virtual ~PredefinedSymRenderer() noexcept = default;
public:
	void registerSymbol( Symbol symbol, std::string const& str );
	inline float const& maxWidth() const noexcept { return this->maxWidth_; };
	inline float const& maxHeight() const noexcept { return this->maxHeight_; };
	geom::Area renderSyms( Canvas& cv, geom::Point const& point, SymList const& str, float depth );
	geom::Area renderSym( Canvas& cv, geom::Point const& point, Symbol const& str, float depth );
};

}}
