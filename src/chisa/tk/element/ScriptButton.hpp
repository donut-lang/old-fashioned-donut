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

class ScriptButton: public ClickButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(ScriptButton);
private:
	struct AttrName{
		const static std::string DonutMachineName;
	};
private:
	std::string machineName_;
	XValue message_;
public:
	virtual std::string toString() const override;
private:
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual void onClick() override;
};

}}
