/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include <cinamo/Handler.h>

#include "../Widget.h"
#include "../../geom/Vector.h"
#include "../../gl/Font.h"
#include "../../doc/render/RenderTree.h"
#include "../../doc/node/Node.h"

namespace chisa {
namespace doc {
class Document;
}

namespace tk {

class ContentWidget: public chisa::tk::Widget {
	CHISA_WIDGET_SUBKLASS(ContentWidget);
	std::shared_ptr<doc::Document> rootNode_;
	Handler<doc::RenderTree> renderTree_;
	bool reload_;
	DEFINE_MEMBER(private, private, geom::Box, lastConstraintSize);
	DEFINE_MEMBER(private, private, geom::Box, lastSize);
private:
	tinyxml2::XMLElement* root_;
	std::string documentId_;
public:
	inline std::string documentId() const noexcept { return this->documentId_; };
public:
	void loadDocument(std::string const& id, bool onInit=false);
	virtual void renderImpl(gl::Canvas& cv, geom::Area const& area) override;
	virtual void idleImpl(const float delta_ms) override;
	virtual void reshapeImpl(geom::Box const& areaSize) override;
	virtual geom::Box measureImpl(geom::Box const& constraintSize) override;
public:
	doc::TreeNode* findTreeNodeById(std::string const& id);
	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInWidget) override;
	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInWidget) override;
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInWidget) override;
	virtual bool onZoom(float const& timeMs, geom::Point const& center, const float ratio) override;
	virtual chisa::geom::Area findTargetImpl(std::string const& target);
};

}}
