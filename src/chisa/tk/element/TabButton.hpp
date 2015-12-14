/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "AbstractToggleButton.hpp"

namespace chisa {
namespace tk {
class TabCombo;

class TabButton: public AbstractToggleButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(TabButton);
private:
	Handler<TabCombo> tab_;
	Handler<Element> element_;
public:
	virtual std::string toString() const override final;
public:
	void tab(Handler<TabCombo> const& tab);
	void element(Handler<Element> const& element);
private:
	virtual void checkedImpl(bool const& state) override final;
	virtual bool checkedImpl() const noexcept  override final;
};

}}
