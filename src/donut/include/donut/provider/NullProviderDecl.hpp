/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../object/ObjectDecl.hpp"
#include "ProviderDecl.hpp"

namespace donut {

class NullProvider final : public Provider {
public:
	NullProvider(Handler<Heap> const& heap);
	virtual ~NullProvider() noexcept = default;
private:
	static inline Object* toPointer() noexcept {
		return reinterpret_cast<Object*>(Object::Tag::Null);
	}
public:
	std::string repr(const Object* ptr) const;
	std::string print(const Object* ptr) const;
public:
	Handler<Object> createNull();
};

}
