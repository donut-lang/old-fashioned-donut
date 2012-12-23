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

#include "ImageManager.h"
#include <algorithm>
#include <libpng15/png.h>
#include "../Sprite.h"

namespace chisa {
namespace gl {
namespace internal {

static const std::string TAG("ImageManager");

ImageManager::ImageManager(Logger& log, Handler<SpriteManager> spriteManager)
:log_(log), spriteManager_(spriteManager)
{
}

Handler<Sprite> ImageManager::queryImage(std::string const& filename)
{
	for(auto it = this->imageCache_.begin(); it != this->imageCache_.end(); ++it){
		if(it->first == filename){
			auto pair = *it;
			this->imageCache_.erase(it);
			this->imageCache_.push_front(pair);
			if(log().t()){
				log().t(TAG, "Cache hit: %s",filename.c_str());
			}
			return pair.second;
		}
	}
	if(log().t()){
		log().t(TAG, "Cache not found: %s",filename.c_str());
	}
	Handler<Sprite> img = this->loadPNG(filename);
	this->imageCache_.push_front(std::make_pair(filename, img));
	while(this->imageCache_.size() > 10) {
		this->imageCache_.pop_back();
	}
	return img;
}

Handler<Sprite> ImageManager::loadPNG(std::string const& filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");
	if(!fp){
		TARTE_EXCEPTION(Exception, "[BUG] oops. failed to open: %s",filename.c_str());
	}
	unsigned char header[8];
	fread(header, 1, 8, fp);
	if(png_sig_cmp(header,0,8)){
		TARTE_EXCEPTION(Exception, "[BUG] oops. File \"%s\" is not a PNG file.",filename.c_str());
	}
	png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if(!png){
		fclose(fp);
		TARTE_EXCEPTION(Exception, "[BUG] oops. failed to create PNG read struct!!");
	}
	png_infop info = png_create_info_struct(png);
	if(!info){
		fclose(fp);
		png_destroy_read_struct(&png, (png_infopp)nullptr, (png_infopp)nullptr);
		TARTE_EXCEPTION(Exception, "[BUG] oops. failed to create PNG info struct!!");
	}
	png_infop end_info = png_create_info_struct(png);
	if (!end_info) {
		fclose(fp);
	    png_destroy_read_struct(&png, &info, (png_infopp)NULL);
		TARTE_EXCEPTION(Exception, "[BUG] oops. failed to create PNG info struct!!");
	}
	if (setjmp(png_jmpbuf(png))) {
		//例外処理
		fclose(fp);
		TARTE_EXCEPTION(Exception, "[BUG] oops. failed read PNG file: \"%s\"!", filename.c_str());
	}
	//ファイルポインタの設定
	png_init_io(png, fp);
	png_set_sig_bytes(png, 8);

	if( ! (png_get_color_type(png, info) & PNG_COLOR_MASK_ALPHA)){
		png_set_filler( png, 0xff, 1 );
	}

	//情報を読み込んでサイズだけ調べる
	png_read_info(png, info);
	const int width = png_get_image_width(png, info);
	const int height = png_get_image_height(png, info);

	Handler<Sprite> spr;
	if(Handler<SpriteManager> mgr = this->spriteManager_.lock()){
		spr = mgr->queryRawSprite(width, height);
		{
			Sprite::Session session(spr, Sprite::BufferType::RGBA8);
			unsigned char* data = session.data();
			const int stride = session.stride();
			unsigned char* ptr[height];
			for(int i=0;i<height;++i){
				ptr[i] = &data[i*stride];
			}
			png_read_image(png, ptr);
		}
	}else{
		this->log().e(TAG, "Oops. SpriteManager already dead!!");
	}
	png_read_end(png, end_info);
	png_destroy_read_struct(&png, &info, &end_info);
	fclose(fp);

	return spr;
}

}}}
