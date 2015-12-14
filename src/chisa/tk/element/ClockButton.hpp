/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "../Element.hpp"
#include "ClickButton.hpp"

namespace chisa {
namespace tk {

class ClockButton final: public ClickButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(ClockButton);
private:
	struct AttrName{
		const static std::string Operation;
	};
public:
	enum ClockOperation{
		Back,
		Forward
	};
private:
	enum ClockOperation operation_;
public:
	virtual std::string toString() const override;
private:
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual void onClick() override;
	virtual bool enabledImpl();
};

}}
