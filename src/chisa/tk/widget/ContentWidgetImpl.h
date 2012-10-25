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
#include "Content/Node.h"
#include "Content/NodeWalker.h"
#include "../../geom/Vector.h"
#include "Content/Margin.h"
#include <cairo/cairo.h>

namespace chisa {
namespace tk {
namespace widget {

class RenderCommand;

class RenderTree {
private:
	typedef std::vector<RenderCommand*> ListType;
	ListType objects_;
public:
	RenderTree();
	~RenderTree() noexcept(true);
	void reset() noexcept(true);
	void render(gl::Canvas& cv, const geom::Area& area);
	void append(RenderCommand* const cmd);
public:
	typedef ListType::iterator Iterator;
	typedef ListType::const_iterator ConstIterator;
	typedef ListType::reverse_iterator ReverseIterator;
	typedef ListType::const_reverse_iterator ConstReverseIterator;
public:
	std::size_t size() const noexcept{ return this->objects_.size(); };
	RenderCommand* at(std::size_t idx) const noexcept{ return this->objects_.at(idx); };
	Iterator begin() { return this->objects_.begin(); };
	Iterator end() { return this->objects_.end(); };
	ConstIterator cbegin() const { return this->objects_.cbegin(); };
	ConstIterator cend() const { return this->objects_.cend(); };
	ReverseIterator rbegin() { return this->objects_.rbegin(); };
	ReverseIterator rend() { return this->objects_.rend(); };
	ConstReverseIterator crbegin() const { return this->objects_.crbegin(); };
	ConstReverseIterator crend() const { return this->objects_.crend(); };
};

class RenderCommand {
	DISABLE_COPY_AND_ASSIGN(RenderCommand);
	DEFINE_MEMBER(public, private, gl::Handler<gl::RawSprite>, sprite);
	DEFINE_MEMBER(public, private, geom::Area, area);
private:
	RenderCommand() noexcept = delete;
public:
	RenderCommand(const geom::Area& area) noexcept:area_(area){};
	virtual ~RenderCommand() noexcept = default;
public:
	bool hasSprite() const noexcept { return this->sprite().operator bool(); };
	gl::Handler<gl::RawSprite> realize(gl::Canvas& cv) {
		if(!this->sprite()){
			this->sprite(this->realizeImpl(cv));
		}
		return this->sprite();
	};
	void free() noexcept{
		this->sprite().reset();
	}
protected:
	virtual gl::Handler<gl::RawSprite> realizeImpl(gl::Canvas& cv) = 0;
};

class ContentMeasurer : public NodeWalker {
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
	public:
		BlockSession(ContentMeasurer& parent);
		BlockSession(ContentMeasurer& parent, BlockNode* const node);
		~BlockSession() noexcept;
		float calcLeftWidth();
		void flushBlock();
		void nextLine();
		geom::Area extendBlock(const geom::Box& size, BlockNode::Direction dir=BlockNode::Direction::None);
		geom::Area extendInline(const geom::Box& size);
	};
private:
	//ウィジットそのものの横幅。何があろうとも、これは厳守ですよ〜。
	float const widgetWidth_;
	BlockSession* nowSession_;
	RenderTree& renderTree_;
private:
	geom::Area extendBlock(const geom::Box& size, BlockNode::Direction dir=BlockNode::Direction::None);
	geom::Area extendInline(const geom::Box& size);
	float calcLeftWidth();
	void nextLine();
	void flushBlock();
public:
	ContentMeasurer(float const width, RenderTree& tree) noexcept;
	geom::Box start(std::shared_ptr<Document> doc);
	virtual ~ContentMeasurer() noexcept (true) = default;
	virtual void walk(Document* model) override;
	virtual void walk(Paragraph* model) override;
	virtual void walk(Heading* model) override;
	virtual void walk(Link* model) override;
	virtual void walk(Text* model) override;
};

}}}
