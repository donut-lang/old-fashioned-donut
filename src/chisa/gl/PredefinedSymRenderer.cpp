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

#include <cinamo/Logger.h>

#include "PredefinedSymRenderer.h"
#include "Canvas.h"

namespace chisa {
namespace gl {

static const std::string TAG("PredefinedStringRenderer");

PredefinedSymRenderer::PredefinedSymRenderer(Logger& log, Handler<DrawableManager> drawableManager, float size)
:log_(log)
,drawableManager_(drawableManager)
,font_(drawableManager->queryFont())
,size_(size)
,color_(gl::Black)
,ascent_(0)
,descent_(0)
,height_(0)
,maxWidth_(0)
{
}

void PredefinedSymRenderer::registerSymbol( unsigned int symbol, std::string const& str )
{
	if( unlikely(!this->renderBuffer_.empty()) ) {
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. PredefinedSymRenderer already compiled.");
	}
	auto it = this->entryTable_.find(symbol);
	if(it != this->entryTable_.end()){
		this->log().w(TAG, "Symbol: %d is already defined, and replaced with: %s", symbol, str.c_str());
		this->entryTable_.erase(it);
	}
	Entry ent;
	ent.str = str;
	ent.glyphs = font_->lookupGlyph(str, size_, ascent_, descent_, height_);
	int totalX = 0;
	for(Handler<BitmapGlyph> const& g : ent.glyphs){
		totalX += g->get()->root.advance.x;
	}
	float const width = FLOAT_FROM_16_16(totalX);
	ent.areaInSprite.width(width);
	ent.areaInSprite.height(height_);

	this->maxWidth_ = geom::max(width, maxWidth_);
	this->entryTable_.insert( std::pair<unsigned int, Entry >( symbol, ent ) );
}

void PredefinedSymRenderer::compile()
{
	if( unlikely(!this->renderBuffer_.empty()) ) {
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. PredefinedSymRenderer already compiled.");
	}
	int nowSprite = 0;
	int const maxSize = drawableManager_->maxTextureSize();
	this->renderBuffer_.resize(1);
	Image* image = &renderBuffer_.back();
	image->sprite = this->drawableManager_->queryRawSprite(ImageFormat::RGBA8, maxSize, maxSize);

	int nowX = 0;
	int nowY = 0;

	for(std::pair<const Symbol, Entry>& p : entryTable_) {
		Entry& ent = p.second;
		if(ent.areaInSprite.width() + FLOAT_FROM_16_16(nowX) > image->sprite->width()) {
			nowX = 0;
			nowY += height_;
		}
		if( nowY + height_ > image->sprite->height()){
			nowX = 0;
			nowY = 0;
			renderBuffer_.resize((++nowSprite)+1);
			image = &renderBuffer_.back();
			image->sprite = this->drawableManager_->queryRawSprite(ImageFormat::RGBA8, maxSize, maxSize);
		}
		ent.spriteNo = nowSprite;
		ent.areaInSprite.x(FLOAT_FROM_16_16(nowX));
		ent.areaInSprite.y(nowY);
		Sprite::Session ss(image->sprite);
		int const spriteAlign = ss.align();
		int const spriteStride = ss.stride();
		unsigned char* const spriteBuf = ss.data();

		for( Handler<BitmapGlyph> const& glyph : ent.glyphs ) {
			FT_BitmapGlyph g = glyph->get();
			int const startX = INT_FROM_16_16_FLOOR(nowX)+g->left;
			int const bmpPitch = g->bitmap.pitch;
			int const bmpWidth = g->bitmap.width;

			int const startY = ascent_-g->top;
			int const endY = g->bitmap.rows+startY;

			int bmpY = 0;
			for(int y=startY;y<endY;++y,++bmpY) {
				unsigned int* const sprBuf = reinterpret_cast<unsigned int*>(&spriteBuf[((nowY+y)*spriteStride)+(startX*spriteAlign)]);
				unsigned char* const bmpBuf = &g->bitmap.buffer[bmpPitch*bmpY];
				for(int x=0;x<bmpWidth; ++x) {
				#if IS_BIG_ENDIAN
					sprBuf[x] = bmpBuf[x] | 0xffffff00;
				#else
					sprBuf[x] = bmpBuf[x] << 24 | 0xffffff;
				#endif
				}
			}
			nowX += g->root.advance.x;
		}
		ent.glyphs.clear();
		ent.glyphs.shrink_to_fit();
	}
}

void PredefinedSymRenderer::flush( Canvas& cv )
{
	for(Image& img : renderBuffer_){
		cv.drawSprite(img.sprite, img.vertexBuffer, img.coordBuffer, this->color_);
		img.vertexBuffer.clear();
		img.coordBuffer.clear();
	}
}

geom::Area PredefinedSymRenderer::renderSyms( Canvas& cv, geom::Point const& point, SymList const& str, float depth)
{
	float x=0.0f;
	for(unsigned int symbol : str){
		auto it = this->entryTable_.find(symbol);
		if(it == this->entryTable_.end()){
			this->log().w(TAG, "Unknown symbol: %d", symbol);
			continue;
		}
		Entry const& ent( it->second );
		Image& img = renderBuffer_[ent.spriteNo];
		geom::Point rendered( point.x()+x, point.y() );
		img.add(rendered, ent.areaInSprite);
		x += ent.areaInSprite.width();
	}
	return geom::Area(point, geom::Box(x,this->height_));
}

geom::Area PredefinedSymRenderer::renderSym( Canvas& cv, geom::Point const& point, Symbol const& symbol, float depth)
{
	auto it = this->entryTable_.find(symbol);
	if( unlikely(it == this->entryTable_.end()) ){
		this->log().w(TAG, "Unknown symbol: %d", symbol);
		return geom::Area();
	}
	Entry const& ent( it->second );
	Image& img = renderBuffer_[ent.spriteNo];
	img.add(point, ent.areaInSprite);
	return geom::Area(point, geom::Box(ent.areaInSprite.width(),this->height_));
}


}}
