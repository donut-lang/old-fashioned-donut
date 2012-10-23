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

#include "../Widget.h"
#include "Content/Decl.h"
#include "Content/NodeWalker.h"
#include "../../geom/Vector.h"
#include "Content/Margin.h"

namespace chisa {
namespace tk {
namespace widget {

class ContentMeasure : public NodeWalker {
private:
	const int width_;
private:
	geom::Point pt_;
	float lineHeight_;
	Margin margin_;
private:
	void nextLine();
public:
	ContentMeasure(int const width) noexcept;
	virtual ~ContentMeasure() noexcept (true) = default;
	void walk(std::shared_ptr<Node> model);
	virtual void walk(Document* model) override;
	virtual void walk(Paragraph* model) override;
	virtual void walk(Heading* model) override;
	virtual void walk(Link* model) override;
	virtual void walk(Text* model) override;
};

class ContentWidget: public chisa::tk::Widget {
	CHISA_WIDGET_SUBKLASS(ContentWidget);
	DEFINE_MEMBER(private, private, std::shared_ptr<Document>, rootNode);
public:
	virtual void render(gl::Canvas& cv, const geom::Area& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(const geom::Box& areaSize) override;
	virtual geom::Box measure(const geom::Box& constraintSize) override;
public:
	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInWidget) override;
	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInWidget) override;
	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInWidget) override;
	virtual bool onZoom(const float timeMs, const geom::Point& center, const float ratio) override;
};

}}}
