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
#include "../../../logging/Logger.h"
#include "../../../gl/StringRenderer.h"
#include "../Content/Node.h"
#include "../Content/NodeWalker.h"
#include "../../../geom/Area.h"
#include <functional>

namespace chisa {
namespace tk {
namespace widget {
class RenderContext;

class ContentMeasurer : public NodeWalker {
	DISABLE_COPY_AND_ASSIGN(ContentMeasurer);
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private:
	static const float DefaultFontSize;
public:
	class BlockSession {
	private:
		BlockNode* const node_;
		ContentMeasurer& parent_;
		BlockSession* const lastSession_;
		DEFINE_MEMBER(public, private, geom::Distance, offsetFromTop);
		DEFINE_MEMBER(public, private, float, maxWidth);
		DEFINE_MEMBER(private, private, BlockNode::Direction, lastDirection)
		//完全に使い切った高さ
		DEFINE_MEMBER(public, private, float, consumedWidth);
		DEFINE_MEMBER(public, private, float, consumedHeight);
		//ブロック要素が横に並んでいるなどで使い切ってる横幅・縦幅
		DEFINE_MEMBER(public, private, float, reservedBlockWidth);
		DEFINE_MEMBER(public, private, float, reservedBlockHeight);
		//インライン要素が並んでいる横幅・縦幅
		DEFINE_MEMBER(public, private, float, reservedInlineWidth);
		DEFINE_MEMBER(public, private, float, reservedInlineHeight);
		DEFINE_MEMBER(public, private, float, inlineHeight);
		//
		bool inlineRendered_;
		geom::Area& area_;
	public:
		BlockSession(ContentMeasurer& parent, geom::Area& area);
		BlockSession(ContentMeasurer& parent, geom::Area& area, BlockNode* const node);
		~BlockSession() noexcept;
		float calcLeftWidth();
		void flushBlock();
		void endLine();
		geom::Area extendBlock(const geom::Box& size, BlockNode::Direction dir=BlockNode::Direction::None);
		geom::Area extendInline(const geom::Box& size);
	};
private:
	//ウィジットそのものの横幅。何があろうとも、これは厳守ですよ〜。
	float const widgetWidth_;
	BlockSession* nowSession_;
private:
	gl::StringRenderer renderer_;
	RenderContext& context_;
private:
	geom::Area extendBlock(const geom::Box& size, BlockNode::Direction dir=BlockNode::Direction::None);
	geom::Area extendInline(const geom::Box& size);
	float calcLeftWidth();
	void nextLine();
	void flushBlock();
private:
	template <typename T, typename U> void walkBlock(T* block, U clos);
public:
	ContentMeasurer(logging::Logger& log, Handler<gl::FontManager> fontManager, RenderContext& context, float const width) noexcept;
	geom::Box start(std::shared_ptr<Document> doc);
	virtual ~ContentMeasurer() noexcept (true) = default;
public:
	virtual void walk(Document* doc) override;
	virtual void walk(Paragraph* para) override;
	virtual void walk(Heading* head) override;
	virtual void walk(Link* link) override;
	virtual void walk(Text* text) override;
	virtual void walk(Font* font) override;
	virtual void walk(BreakLine* br) override;
};

}}}
