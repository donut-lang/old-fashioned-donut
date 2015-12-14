/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "TabButton.hpp"
#include "TabCombo.hpp"

namespace chisa {
namespace tk {


CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(TabButton, AbstractToggleButton)
{
	this->margin(geom::Space(0));
	this->padding(geom::Space(0));
}

TabButton::~TabButton() noexcept
{
}

void TabButton::tab(Handler<TabCombo> const& tab)
{
	this->tab_ = tab;
}
void TabButton::element(Handler<Element> const& element)
{
	this->element_ = element;
}

std::string TabButton::toString() const
{
	return ::cinamo::format("(TabButton text:\"%s\" %p)", this->text().c_str(), this);
}

void TabButton::checkedImpl(bool const& state)
{
	if(state && this->tab_){
		this->tab_->bringChildToFront(this->element_);
	}
}
bool TabButton::checkedImpl() const noexcept
{
	return this->tab_ && this->tab_->frontChild() == this->element_;
}

}}
