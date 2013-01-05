/**
 * Chisa
 * Copyright (C) 2013 psi
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

#include "../../TestCommon.h"
#include "../../src/chisa/audio/SoundSpec.h"

namespace chisa {

TEST(SoundSpecTest, Get)
{
	SoundSpec spec(SoundSpec::DataFormat::S16, 10, 12, 11);
	ASSERT_EQ(SoundSpec::DataFormat::S16, spec.format());
	ASSERT_EQ(10, spec.channels());
	ASSERT_EQ(12, spec.frequency());
	ASSERT_EQ(11, spec.samples());
}

TEST(SoundSpecTest, Set)
{
	SoundSpec spec(SoundSpec::DataFormat::S16, 10, 12, 11);
	spec.format() = SoundSpec::DataFormat::U16LSB;
	spec.channels() = 123;
	spec.frequency() = 456;
	spec.samples() = 789;
	ASSERT_EQ(SoundSpec::DataFormat::U16LSB, spec.format());
	ASSERT_EQ(123, spec.channels());
	ASSERT_EQ(456, spec.frequency());
	ASSERT_EQ(789, spec.samples());
}

TEST(SoundSpecTest, CompareSameValue)
{
	SoundSpec spec(SoundSpec::DataFormat::S16, 0, 0, 1);
	ASSERT_TRUE( spec == spec );
	ASSERT_TRUE( spec >= spec );
	ASSERT_TRUE( spec <= spec );
	ASSERT_FALSE( spec != spec );
	ASSERT_FALSE( spec > spec );
	ASSERT_FALSE( spec < spec );
}

TEST(SoundSpecTest, Compare)
{
	{
		SoundSpec spec1(SoundSpec::DataFormat::S16, 0, 0, 1);
		SoundSpec spec2(SoundSpec::DataFormat::S16, 0, 0, 2);
		ASSERT_TRUE(( spec1 < spec2 ));
		ASSERT_FALSE(( spec1 > spec2 ));
	}
	{
		SoundSpec spec1(SoundSpec::DataFormat::S16, 0, 0, 1);
		SoundSpec spec2(SoundSpec::DataFormat::S16, 0, 1, 0);
		ASSERT_TRUE(( spec1 < spec2 ));
		ASSERT_FALSE(( spec1 > spec2 ));
	}
	{
		SoundSpec spec1(SoundSpec::DataFormat::S16, 0, 1, 1);
		SoundSpec spec2(SoundSpec::DataFormat::S16, 0, 2, 0);
		ASSERT_TRUE(( spec1 < spec2 ));
		ASSERT_FALSE(( spec1 > spec2 ));
	}
	{
		SoundSpec spec1(SoundSpec::DataFormat::S16, 0, 2, 1);
		SoundSpec spec2(SoundSpec::DataFormat::S16, 0, 1, 0);
		ASSERT_TRUE(( spec1 > spec2 ));
		ASSERT_FALSE(( spec1 < spec2 ));
	}
}

TEST(SoundSpecTest, Invalid)
{
	SoundSpec spec;
	ASSERT_TRUE(spec.isInvalid());
}

}
