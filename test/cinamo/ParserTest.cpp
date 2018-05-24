///* coding: utf-8 */
///**
// * Cinamo
// *
// * Copyright 2012-2013, PSI
// */
//
//#include "../TestCommon.h"
//#include <cinamo/functional/Parser.h>
//
//namespace cinamo {
//
//TEST(ParserTest, NumberTest)
//{
//	using namespace cinamo::parser;
//
//	constexpr auto b = number();
//	constexpr auto ans = (b("012133afg")).answer();
//	static_assert(ans.first.begin == 0, "Failed to parse");
//	static_assert(ans.first.end == 6, "Failed to parse");
//}
//TEST(ParserTest, SelectCombineTest)
//{
//	using namespace cinamo::parser;
//
//	constexpr auto b = ((loop<digit>()) || (loop<alphabet>())) >> (loop<alphabet>());
//	constexpr auto ans = (b("0aaaaa")).answer();
//	static_assert(ans.first.begin == 0, "Failed to parse");
//	static_assert(ans.first.end == 6, "Failed to parse");
//}
//
//TEST(ParserTest, CombinatorTest)
//{
//	using namespace cinamo::parser;
//	constexpr ParserCombinator<
//		Entry<0, number>,
//		Entry<1, loop<alphabet> >
//	> parser;
//
//	constexpr auto ans = parser.parseOne("test").answer();
//	static_assert(std::get<0>(ans) == 1, "Failed to parse");
//	static_assert(std::get<1>(ans).begin == 0, "Failed to parse");
//	static_assert(std::get<1>(ans).end == 4, "Failed to parse");
//}
//}
