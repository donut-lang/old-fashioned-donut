/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include "AbstractButton.hpp"

namespace chisa {
namespace tk {

class AbstractToggleButton: public AbstractButton {
	CHISA_ELEMENT_SUBKLASS_FINAL(AbstractToggleButton);
public:
	struct AttrName{
		static std::string const OnForegroundColor;
		static std::string const OnBackgroundColor;
		static std::string const OffForegroundColor;
		static std::string const OffBackgroundColor;
	};
private:
	gl::Color onForegroundColor_;
	gl::Color onBackgroundColor_;
	gl::Color offForegroundColor_;
	gl::Color offBackgroundColor_;
private:
	geom::Box renderOffset_;
public:
	void toggle();
	void checked(bool const& state);
	bool checked() const;
	void notifyCheckedStateChanged();
public:
	gl::Color onBackgroundColor() const;
	gl::Color onForegroundColor() const;
	gl::Color offBackgroundColor() const;
	gl::Color offForegroundColor() const;
public:
	void onBackgroundColor(gl::Color const& c);
	void onForegroundColor(gl::Color const& c);
	void offBackgroundColor(gl::Color const& c);
	void offForegroundColor(gl::Color const& c);
protected:
	virtual std::string toString() const override;
protected: /* AbstractButtonの実装 */
	virtual void loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element) override;
	virtual geom::Box measureButtonContent(geom::Box const& constraint) override;
	virtual void layoutButtonContent(geom::Box const& size) override;
	virtual void renderOn(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override;
	virtual void renderOff(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override;
	virtual void renderDisabled(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask) override;
	virtual bool isOn() const noexcept override;
	virtual void onClick() override;
protected:
	virtual void checkedImpl(bool const& state) = 0;
	virtual bool checkedImpl() const noexcept = 0;
};

}}
