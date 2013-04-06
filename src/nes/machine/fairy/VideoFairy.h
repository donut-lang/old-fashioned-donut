/*
 * VideoFairy.h
 *
 *  Created on: 2011/08/23
 *      Author: psi
 */

#pragma once

#include <stdint.h>
#include <cstddef>
#include <cinamo/XArchiver.h>
#include "../exception/EmulatorException.h"

namespace nes {

class VideoFairy
{
public:
	enum{
		screenWidth = 256,
		screenHeight = 240
	};
	static const uint32_t nesPaletteARGB[64];
	explicit VideoFairy(){}
	virtual ~VideoFairy(){}
	virtual void dispatchRendering(const uint8_t (&nesBuffer)[screenHeight][screenWidth], const uint8_t paletteMask) = 0;
	virtual void save(::cinamo::XArchiverOut & arc) {};
	virtual void load(::cinamo::XArchiverIn & arc) {};
};

class DummyVideoFairy : public VideoFairy
{
public:
	explicit DummyVideoFairy(){}
	virtual ~DummyVideoFairy(){}
};

}
