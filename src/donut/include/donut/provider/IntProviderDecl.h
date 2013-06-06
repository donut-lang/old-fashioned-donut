/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../object/ObjectDecl.h"
#include "ProviderDecl.h"

namespace donut {


class IntProvider final : public Provider {
public:
	IntProvider(Handler<Heap> const& heap);
	virtual ~IntProvider() noexcept = default;
private:
	static constexpr inline int fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
	static constexpr inline Object* toPointer(int const& val) noexcept {
		return reinterpret_cast<Object*>((val << 2) | Object::Tag::Int);
	}
public:
	std::string repr(const Object* ptr) const;
	std::string print(const Object* ptr) const;
	int toInt(const Object* ptr) const;
public:
	Handler<Object> create( int const& val );
};

}
