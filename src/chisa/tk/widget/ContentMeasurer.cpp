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

#include "ContentWidgetImpl.h"
#include "ContentMeasurerUtil.h"
#include "Content/Node.h"
#include "../../gl/StringRenderer.h"
#include "../../geom/Area.h"

namespace chisa {
namespace tk {
namespace widget {

const float ContentMeasurer::DefaultFontSize = 16.0f;

ContentMeasurer::ContentMeasurer(logging::Logger& log, Handler<gl::FontManager> fontManager, float const width, RenderTree& tree) noexcept
:log_(log)
,widgetWidth_(width)
,nowSession_(nullptr)
,renderTree_(tree)
,renderer_(fontManager)
{
	this->renderTree_.reset();
}

geom::Box ContentMeasurer::start(std::shared_ptr<Document> doc)
{
	BlockSession bs(*this);
	this->NodeWalker::start(doc);
	return geom::Box(bs.reservedBlockWidth(), bs.reservedBlockHeight());
}


void ContentMeasurer::walk(Document* doc)
{
	BlockSession bs(*this, doc);
	this->walkChildren(doc);
}

void ContentMeasurer::walk(Paragraph* para)
{
	BlockSession bs(*this, para);
	this->walkChildren(para);
}

void ContentMeasurer::walk(Heading* heading)
{
	BlockSession bs(*this, heading);
	this->renderer_.pushSize(ContentMeasurer::DefaultFontSize*(1.0f+heading->level()/2.0f));
	this->walkChildren(heading);
	this->renderer_.popSize();
}

void ContentMeasurer::walk(Link* link)
{
	this->walkChildren(link);
}

void ContentMeasurer::walk(Font* font)
{
	if(!font->color().isUnchangedColor()){
		this->renderer_.pushColor(font->color());
	}
	if(font->size() > 0){
		this->renderer_.pushSize(font->size());
	}
	this->walkChildren(font);
	if(!font->color().isUnchangedColor()){
		this->renderer_.popColor();
	}
	if(font->size() > 0){
		this->renderer_.popSize();
	}
}

class TextRenderCommand : public RenderCommand
{
private:
	gl::StringRenderer::Command cmd_;
public:
	TextRenderCommand(const geom::Area& area, const gl::StringRenderer::Command& cmd) noexcept
	:RenderCommand(area),cmd_(cmd){};
	virtual ~TextRenderCommand() noexcept = default;
public:
	virtual Handler<gl::RawSprite> realizeImpl(gl::Canvas& cv) override
	{
		return this->cmd_.renderString(cv);
	}
};

void ContentMeasurer::walk(BreakLine* br)
{
	//改行するだけ
	this->nextLine();
}

void ContentMeasurer::walk(Text* text)
{
	std::vector<std::string> lines;
	std::string str(shrinkSpace(text->text()));
	size_t now=0;
	while(now < str.length()){
		gl::StringRenderer::Command cmd = this->renderer_.calcMaximumStringLength(str, this->calcLeftWidth(), now);
		if(!cmd){//そもそも１文字すら入らない
			this->nextLine();
			continue;
		}
		now += cmd.str().size();
		//文字分のエリアを確保し、その位置とレンダリングコマンドを記録
		geom::Area rendered = this->extendInline(cmd.size());
		this->renderTree_.append(new TextRenderCommand(rendered, cmd));
	}
}

}}}
