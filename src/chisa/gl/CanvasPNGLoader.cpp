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


#include "Canvas.h"
#include <algorithm>
#include <libpng15/png.h>

namespace chisa{
namespace gl {

Sprite::Handler Canvas::loadPNG(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");
	if(!fp){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] oops. failed to open: %s",filename.c_str());
	}
	unsigned char header[8];
	fread(header, 1, 8, fp);
	if(png_sig_cmp(header,0,8)){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] oops. File \"%s\" is not a PNG file.",filename.c_str());
	}
	png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if(!png){
		fclose(fp);
		throw logging::Exception(__FILE__, __LINE__, "[BUG] oops. failed to create PNG read struct!!");
	}
	png_infop info = png_create_info_struct(png);
	if(!info){
		fclose(fp);
		png_destroy_read_struct(&png, (png_infopp)nullptr, (png_infopp)nullptr);
		throw logging::Exception(__FILE__, __LINE__, "[BUG] oops. failed to create PNG info struct!!");
	}
	png_infop end_info = png_create_info_struct(png);
	if (!end_info) {
		fclose(fp);
	    png_destroy_read_struct(&png, &info, (png_infopp)NULL);
		throw logging::Exception(__FILE__, __LINE__, "[BUG] oops. failed to create PNG info struct!!");
	}
	if (setjmp(png_jmpbuf(png))) {
		//例外処理
		fclose(fp);
		throw logging::Exception(__FILE__, __LINE__, "[BUG] oops. failed read PNG file: \"%s\"!", filename.c_str());
	}
	//ファイルポインタの設定
	png_init_io(png, fp);
	png_set_sig_bytes(png, 8);

	//情報を読み込んでサイズだけ調べる
	png_read_info(png, info);
	const int width = png_get_image_width(png, info);
	const int height = png_get_image_height(png, info);

	Sprite::Handler spr = this->querySprite(width, height);
	{
		Sprite::Session session(spr);
		unsigned char* data = session.data();
		const int stride = session.stride();
		unsigned char* ptr[height];
		for(int i=0;i<height;++i){
			ptr[i] = &data[i*stride];
		}
		png_read_image(png, ptr);
	}
	png_read_end(png, end_info);
	png_destroy_read_struct(&png, &info, &end_info);

	return spr;
}

}}
