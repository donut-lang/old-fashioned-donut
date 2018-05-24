///* coding: utf-8 */
///**
// * Cinamo
// *
// * Copyright 2013, psi
// */
//#pragma once
//
//#include "Either.h"
//#include "Maybe.h"
//#include "List.h"
//#include <utility>
//namespace cinamo {
//namespace parser {
//
//typedef const char* error_msg_type;
//
///**
// * パースした後の結果を表す
// */
//class Range {
//public:
//	int const begin;
//	int const end;
//public:
//	constexpr Range():begin(-1),end(-1){
//	}
//	constexpr Range(int b,int e):begin(b),end(e){
//	}
//	constexpr Range(Range const& b,Range const& e):begin(b.begin),end(e.end){
//	}
//	constexpr int size() const{
//		return end-begin;
//	}
//};
///**
// * パースしている文字列、今まで消化した文字数などのコンテキストを記録している
// */
//class Context {
//public:
//	const char* const string;
//	const int length;
//	const int now;
//	const bool ended;
//public:
//	constexpr Context()
//	:string(nullptr),length(-1),now(-1),ended(true){}
//	template <size_t n>
//	constexpr Context(const char (&str)[n])
//	:string(str),length(n),now(0),ended(false){}
//private:
//	constexpr Context(const char* const string, int length, int now=0)
//	:string(string),length(length),now(now),ended(now >= length){}
//public:
//	constexpr std::pair<Range, Context> forward(int pos) const{
//		return std::pair<Range, Context>(Range(now, now+pos), Context(string, length, this->now+pos));
//	}
//	constexpr Maybe<char> next() const{
//		return ended ? Nothing<char>() : Just<char>(this->string[this->now]);
//	}
//};
//constexpr Either<error_msg_type, std::pair<Range, Context> > Left(error_msg_type err){
//	return ::cinamo::Left<error_msg_type, std::pair<Range, Context> >(err);
//}
//constexpr Either<error_msg_type, std::pair<Range, Context> > Right(Range const& range, Context const& ctx){
//	return ::cinamo::Right<error_msg_type, std::pair<Range, Context> >(std::pair<Range, Context>(range, ctx));
//}
//constexpr Either<error_msg_type, std::pair<Range, Context> > Right(std::pair<Range, Context> const& x){
//	return ::cinamo::Right<error_msg_type, std::pair<Range, Context> >(x);
//}
//
//template <typename FST, typename NXT> class CombinedParser;
//template <typename FST, typename NXT> class SelectParser;
//
//template <typename Self>
//class Parser{
//public:
//	template <typename B>
//	constexpr CombinedParser<Self,B> operator >> (Parser<B> const& b) const{
//		return CombinedParser<Self,B>(static_cast<Self const&>(*this), static_cast<B const&>(b));
//	}
//	template <typename B>
//	constexpr SelectParser<Self,B> operator || (Parser<B> const& b) const{
//		return SelectParser<Self,B>(static_cast<Self const&>(*this), static_cast<B const&>(b));
//	}
//};
//
///**
// * 継続。
// * Combined ::= FST NXT
// */
//template <typename FST, typename NXT>
//class CombinedParser : public Parser<CombinedParser<FST,NXT> > {
//private:
//	FST const f;
//	NXT const n;
//public:
//	constexpr CombinedParser(FST const& f, NXT const& n):f(f),n(n){
//	}
//private:
//	struct bind{
//		Context const& ctx;
//		constexpr bind(Context const& ctx):ctx(ctx){
//		}
//		constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(std::pair<Range, Context> res) const{
//			return Right(ctx.forward(Range(ctx.now, res.first.end).size()));
//		}
//	};
//	struct bindn {
//		NXT const& n;
//		constexpr bindn(NXT const& n):n(n){
//		}
//		constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(std::pair<Range, Context> res) const{
//			return n(res.second);
//		}
//	};
//public:
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const {
//		return ((f(ctx) >>= bindn(n)) >>= bind(ctx));
//	}
//};
///**
// * 選択
// * Select ::= (FST|NXT)
// */
//template <typename FST, typename NXT>
//class SelectParser : public Parser<SelectParser<FST,NXT> > {
//private:
//	FST const f;
//	NXT const n;
//public:
//	constexpr SelectParser(FST const& f, NXT const& n):f(f),n(n){
//	}
//public:
//	constexpr Either<error_msg_type, std::pair<Range, Context> > inner(Context const& ctx, Either<error_msg_type, std::pair<Range, Context> > const& first) const {
//		return first.isRight ? first : n(ctx);
//	}
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const {
//		return inner(ctx, f(ctx));
//	}
//};
///**
// * ０回以上の繰り返し。
// * loop ::= Base*
// */
//template <typename Base>
//class loop:public Parser<loop<Base> >{
//	Base const b;
//	constexpr Either<error_msg_type, std::pair<Range, Context> > inner(int begin, int end, Context const& ctx, Either<error_msg_type, std::pair<Range, Context> > const& result) const{
//		return result.isLeft ?
//				Right(Range(begin, end), ctx) :
//				inner(begin, result.answer().first.end, result.answer().second, b(result.answer().second));
//	}
//public:
//	constexpr loop():b(){
//	}
//public:
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const{
//		return inner(ctx.now, ctx.now, ctx, b(ctx));
//	}
//};
//
///**
// * １回以上の繰り返し。
// * plus ::= Base+
// */
//template <typename Base>
//class plus:public Parser<plus<Base> >{
//public:
//	constexpr plus(){
//	}
//public:
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const{
//		return (Base() >> loop<Base>())(ctx);
//	}
//};
//
///**
// * 特定の述語を満たす文字を一つだけ取る。
// */
//template <typename PRED>
//class satisfy : public Parser<satisfy<PRED> > {
//private:
//	PRED const pred;
//	constexpr Either<error_msg_type, std::pair<Range, Context> > inner(Context const& ctx, Maybe<char> const& nxt) const{
//		return !nxt.isNothing && pred(nxt.value()) ?
//				Right(ctx.forward(1)):
//				Left("It's not digit.");
//	}
//public:
//	constexpr satisfy():pred(){
//	}
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const{
//		return inner(ctx, ctx.next());
//	}
//};
//
///**
// * 以下satisfy用の述語
// */
//
//struct isnumber{
//	constexpr bool operator()(char c){
//		return c >= '0' && c <= '9';
//	}
//};
//struct is_nonzero_number{
//	constexpr bool operator()(char c){
//		return c >= '1' && c <= '9';
//	}
//};
//struct is_oct_number{
//	constexpr bool operator()(char c){
//		return c >= '0' && c <= '7';
//	}
//};
//struct is_hex_number{
//	constexpr bool operator()(char c){
//		return (isnumber()(c)) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
//	}
//};
//struct isalphabet{
//	constexpr bool operator()(char c){
//		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
//	}
//};
//struct isalphanum{
//	static constexpr isnumber n = isnumber();
//	static constexpr isalphabet a = isalphabet();
//	constexpr bool operator()(char c){
//		return n(c) || a(c);
//	}
//};
//template <char C>
//struct is_a{
//	constexpr bool operator()(char c){
//		return C == c;
//	}
//};
//
//typedef satisfy<isnumber> digit;
//typedef satisfy<isalphabet> alphabet;
//
//class natural:public Parser<natural>{
//public:
//	constexpr natural(){
//	}
//public:
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const{
//		return (satisfy<is_nonzero_number>() >> loop<digit>())(ctx);
//	}
//};
//
//class oct:public Parser<oct>{
//public:
//	constexpr oct(){
//	}
//public:
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const{
//		return (satisfy<is_a<'0'> >() >> loop<satisfy<is_oct_number> >())(ctx);
//	}
//};
//
//class hex:public Parser<cinamo::parser::hex>{
//public:
//	constexpr hex(){
//	}
//public:
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const{
//		return (
//				(satisfy<is_a<'0'> >())
//				>> (satisfy<is_a<'x'> >() || satisfy<is_a<'X'> >())
//				>>(plus<satisfy<is_hex_number> >())
//				)(ctx);
//	}
//};
//
//class number:public Parser<cinamo::parser::number>{
//public:
//	constexpr number(){
//	}
//public:
//	constexpr Either<error_msg_type, std::pair<Range, Context> > operator()(Context const& ctx) const{
//		return ( hex() || oct() || natural() )(ctx);
//	}
//};
//
//
//template <int sym_, typename PARSER>
//class Entry {
//public:
//	static const constexpr int sym = sym_;
//	static const constexpr PARSER parser = PARSER();
//};
//
//template <typename... Args>
//class ParserCombinator {
//	constexpr Either<error_msg_type, std::tuple<int, Range, Context> > makeResult(int sym, std::pair<Range, Context> const& p) const{
//		return ::cinamo::Right<error_msg_type, std::tuple<int, Range, Context> >(std::tuple<int, Range, Context>(sym, p.first, p.second));
//	}
//
//	template <typename T>
//	constexpr Either<error_msg_type, std::tuple<int, Range, Context> > tryPEnd( Either<error_msg_type, std::pair<Range, Context> > const& result ) const{
//		return result.isRight ? makeResult(T::sym, result.answer()) : ::cinamo::Left<error_msg_type, std::tuple<int, Range, Context> >("parse failed");
//	}
//	template <typename T>
//	constexpr Either<error_msg_type, std::tuple<int, Range, Context> > tryP(Context const& ctx) const{
//		return tryPEnd<T>(T::parser(ctx));
//	}
//
//	template <typename T, typename R, typename... Left>
//	constexpr Either<error_msg_type, std::tuple<int, Range, Context> > tryPLoop(Context const& ctx, Either<error_msg_type, std::pair<Range, Context> > const& result) const{
//		return result.isRight ? makeResult(T::sym, result.answer()) : tryP<R, Left...>(ctx);
//	}
//	template <typename T, typename R, typename... Left>
//	constexpr Either<error_msg_type, std::tuple<int, Range, Context> > tryP(Context const& ctx) const{
//		return tryPLoop<Args...>(ctx, T::parser(ctx));
//	}
//public:
//	constexpr Either<error_msg_type, std::tuple<int, Range, Context> > parseOne(Context const& ctx) const{
//		return tryP<Args...>(ctx);
//	}
//private:
////	template <typename... Left>
////	constexpr auto parseIC(Either<error_msg_type, std::tuple<int, Range, Context> > const& result, Left... args)
////	-> decltype(result.isLeft ? toList(args...) : parseI(std::get<2>(result.answer()), std::pair<int, Range>(std::get<0>(result.answer()), std::get<1>(result.answer())), args...))
////	{
////		return
////				result.isLeft ?
////						List<std::pair<int, Range>, sizeof...(Left)>(args...) :
////						parseI<std::pair<int, Range>, Left...>(std::get<2>(result.answer()), std::pair<int, Range>(std::get<0>(result.answer()), std::get<1>(result.answer())), args...);
////	}
////	template <typename... Left>
////	constexpr auto parseI(Context const& ctx, Left... args)
////	-> decltype(parseIC<Left...>(parseOne(ctx), args...)){
////		return parseIC<Left...>(parseOne(ctx), args...);
////	}
////public:
////	template <size_t N>
////	constexpr auto parse(const char (&str)[N])
////	-> decltype(parseI<>(Context(str)))
////	{
////		return parseI<>(Context(str));
////	}
//};
//
//
//}}
