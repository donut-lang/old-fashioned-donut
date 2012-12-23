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
#include <tarte/Logger.h>
#include <vector>

#include "../node/NodeWalker.h"
#include "../../geom/Vector.h"

namespace chisa {
namespace doc {

class PostDisposer: public chisa::doc::NodeWalker {
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	std::vector<geom::Point> offsetStack_;
public:
	PostDisposer(Logger& log);
	virtual ~PostDisposer() noexcept = default;
private:
	template <typename T, typename U> void walkBlock(T* block, U clos);
	geom::Point offset();
public:
	virtual void walk(Document* doc) override;
	virtual void walk(Paragraph* para) override;
	virtual void walk(Heading* head) override;
	virtual void walk(Text* text) override;
	virtual void walk(Link* link) override;
	virtual void walk(Font* font) override;
	virtual void walk(BreakLine* br) override;
};

}}
