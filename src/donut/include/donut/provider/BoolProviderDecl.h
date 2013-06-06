/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cstdint>
#include "../object/ObjectDecl.h"
#include "ProviderDecl.h"

namespace donut {

class BoolProvider final : public Provider {
public:
	BoolProvider(Handler<Heap> const& heap);
	virtual ~BoolProvider() noexcept = default;
public:
	static constexpr inline bool fromPointer(const Object* const ptr) noexcept {
		return reinterpret_cast<std::intptr_t>(ptr) >> 2;
	}
	static constexpr inline Object* toPointer(bool const& val) noexcept {
		return reinterpret_cast<Object*>((val << 2) | Object::Tag::Bool);
	}
public:
	std::string repr(const Object* ptr) const;
	std::string print(const Object* ptr) const;
	bool toBool(const Object* ptr) const;
public:
	Handler<Object> create( bool const& val );
};

}
