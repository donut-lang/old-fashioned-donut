/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/functional/Box.h>

namespace cinamo {

TEST(BoxTest, BasicTest)
{
	Box<int> b = Full(10);
}

}
