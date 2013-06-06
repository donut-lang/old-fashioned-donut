/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include <cinamo/Exception.h>
#include <cinamo/Logger.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/Handler.h>
#include "../geom/Decl.h"
#include "Color.h"
#include "Sprite.h"
#include "Drawable.h"

namespace chisa {
namespace geom {
class Area;
}
namespace gl {

class Canvas {
	DISABLE_COPY_AND_ASSIGN(Canvas);
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	float width_;
	float height_;
public:
	Canvas(Logger& log);
	virtual ~Canvas();
public:
	void ortho(const float left, const float right, const float bottom, const float top, const float near_val, const float far_val);
	void resize2d(geom::Box const& box);
	void translate(geom::Point const& pt);
	void rotate(const float angle);
	void scale(geom::ScaleVector const& scale);
	void drawSprite(Handler<Sprite> const& sprite, geom::Point const& pt, const float depth = 0.0f, Color const& color=White);
	void drawSprite(Handler<Sprite> const& sprite, geom::Point const& pt, geom::Area const& mask, const float depth = 0.0f, Color const& color=White);
	void drawSprite(Handler<Sprite> const& sprite, std::vector<float> const& pts, std::vector<float> const texCoords, Color const& color=White);
	void drawTexture(unsigned int texId, geom::Area const& areaInRoot, geom::Area const& coordinateInSprite, const float depth, Color const& color);
	void drawTexture(unsigned int texId, std::vector<float> const& pts, std::vector<float> const texCoords, Color const& color);

	void drawLine(const float width, Color const& color, geom::Point const& start, geom::Point const& end, const float depth=0.0f);
	void drawLines(const float width, Color const& color, std::vector<geom::Point> const& pts, const float depth);
	void drawRect(const float width, Color const& color, geom::Area const& area, const float depth=0.0f);
	void fillRect(Color const& color, geom::Area const& area, const float depth=0.0f);
	void flush();
public:
	class ScissorScope {
	private:
		Canvas& canvas_;
		geom::Area area_;
		ScissorScope* const orig_;
	public:
		ScissorScope(Canvas& canvas, geom::Area const& area);
		virtual ~ScissorScope();
	};
	class AffineScope {
	private:
		Canvas& canvas_;
	public:
		AffineScope(Canvas& canvas);
		virtual ~AffineScope();
	};
private:
	enum GLOperation{
		None=0,
		Lines,
		LineStrip,
		Texture,
		Rect
	} glOperation_;
	std::vector<float> vertexs_;
	std::vector<float> texCoords_;
	gl::Color nowColor_;
	float nowLineWidth_;
	ScissorScope* nowScissor_;
	bool vertexArrayEnabled_;
	bool textureEnabled_;
	GLuint nowTexId_;
private: /* 次の状態を設定 */
	void setOperation(GLOperation const& op);
	void setColor( gl::Color const& nextColor );
	void setLineWidth( float const& lineWidth );
	void bindTexture(GLuint texId);
	inline void pushVertex( float const& x, float const& y, float const& z=0 ) { vertexs_.push_back(x);vertexs_.push_back(y);vertexs_.push_back(z); };
	inline void pushTexCoord( float const& x, float const& y ) { texCoords_.push_back(x);texCoords_.push_back(y); };
private:
	void scissor(geom::Area const& area);
private:
	void flushGL();
	void disableVertexArray();
	void enableVertexArray();
	void disableTexture();
	void enableTexture();
};

}}
