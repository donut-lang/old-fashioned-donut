/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "DecoUtil.h"
#include "Canvas.h"
#include "../geom/Vector.h"

namespace chisa {
namespace gl {


void renderCubicLine(gl::Canvas& cv, float width, gl::Color const& color, geom::Point const& pt1, geom::Point const& pt2)
{
	static const float _sin[10] = {
			0.0,
			0.3090081824816504,
			0.5877702605258085,
			0.8090006559383217,
			0.95104506302846,
			0.999999998926914,
			0.9510736937458322,
			0.8090551149629135,
			0.5878452173406992,
			0.30909630022076356};

	geom::Distance delta( pt1-pt2 );
	geom::Point const vec_( delta.y()/delta.x() , -1 );
	geom::Point const vec( vec_ / (vec_.length()/(delta.length()/7)) );
	std::vector<geom::Point> pts;
	pts.reserve(10);
	pts.push_back(pt1);
	for(int x=9; x >= 1; --x) {
		pts.push_back( ((pt1 * x + pt2*(10-x)) / 10)+vec*_sin[x] );
	}
	pts.push_back(pt2);
	cv.drawLines(width, color, pts, 0.0f);
}

}}
