/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <utility>

namespace chisa {
namespace gl {
namespace internal {

template <class T>
struct WidthOrder : public std::binary_function<T const*, T const*, bool> {
	inline bool operator()(T const* const& a, T const* const& b) const noexcept
	{
		return a->width() == b->width() ? a->height() < b->height() : a->width() < b->width();
	}
	inline bool operator() (T const* const& a, const std::pair<int,int>& b) const noexcept
	{
		return a->width() == b.first ? a->height() < b.second : a->width() < b.first;
	}
	inline bool operator() (const std::pair<int,int>& a, T const* const& b) const noexcept
	{
		return a.first == b->width() ? a.second < b->height() : a.first < b->width();
	}
	inline bool operator() (const std::pair<int,int>& a, const std::pair<int,int>& b) const noexcept
	{
		return a.first == b.first ? a.second < b.second : a.first < b.first;
	}
};

template <class T>
struct SizeOrder : std::binary_function<const T*, const T*, bool> {
	inline bool operator()(T const* const& a, std::size_t const& b) const noexcept {
		return a->size() < b;
	}
	inline bool operator()(std::size_t const& a, std::size_t const& b) const noexcept {
		return a < b;
	}
	inline bool operator()(T const* const& a, T const* const& b) const noexcept {
		return a->size() < b->size();
	}
	inline bool operator()(std::size_t const& a, T const* const& b) const noexcept {
		return a < b->size();
	}
};

}}}
