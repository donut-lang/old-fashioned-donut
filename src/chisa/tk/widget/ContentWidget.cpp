/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../tk/World.h"
#include "../../geom/Area.h"
#include "../../geom/Vector.h"
#include "../../doc/node/NodeReader.h"
#include "../../doc/node/Node.h"
#include "../../doc/dispose/Disposer.h"
#include "../Widget.h"
#include "ContentWidget.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {

static std::string const TAG("ContentWidget");

CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF(ContentWidget)
,renderTree_(new doc::RenderTree(this->log(), world.lock()->drawableManager()))
,reload_(false)
,lastConstraintSize_()
,root_(element)
{
	this->loadDocument("main",true);
}

void ContentWidget::loadDocument(std::string const& id, bool onInit)
{
	for(tinyxml2::XMLElement* elm = root_->FirstChildElement("doc"); elm; elm = elm->NextSiblingElement("doc")){
		if(id == elm->Attribute("id")) {
			this->documentId_ = id;
			this->rootNode_ = doc::NodeReader().parseTree(elm);
			if(this->log().t()){
				this->log().t(TAG, "Document id \"%s\" Parsed.", id.c_str());
			}
			if(!onInit){
				this->reload_ = true;
				this->requestRelayout();
				if(this->log().t()){
					this->log().t(TAG, "Render tree for \"%s\" constructed.", id.c_str());
				}
			}
			return;
		} else {
			if(this->log().t()){
				this->log().t(TAG, "Document id \"%s\" != \"%s\"", elm->Attribute("id"), id.c_str());
			}
		}
	}
	TARTE_EXCEPTION(Exception, "[BUG] Document for \"%s\" not found.", id.c_str());
}

void ContentWidget::renderImpl(gl::Canvas& cv, geom::Area const& area)
{
	this->renderTree_->render(cv, area, 0.0f);
}

void ContentWidget::idleImpl(const float delta_ms)
{
}

void ContentWidget::reshapeImpl(geom::Box const& areaSize)
{
	this->measure(areaSize);
}

geom::Box ContentWidget::measureImpl(geom::Box const& constraintSize)
{
	if(reload_
			|| geom::isUnspecified(this->lastConstraintSize_.width())
			|| geom::isUnspecified(this->lastConstraintSize_.height())
			|| !(std::fabs(constraintSize.width()-this->lastConstraintSize_.width()) < geom::VerySmall)
			|| !(std::fabs(constraintSize.height()-this->lastConstraintSize_.height()) < geom::VerySmall)
	){
		this->lastConstraintSize( constraintSize );
		this->lastSize(
			doc::Disposer(log(), renderTree_, constraintSize).start(this->rootNode_)
		);
		reload_ = false;
	}
	return geom::Box(geom::max(constraintSize.width(), this->lastSize().width()), geom::max(constraintSize.height(), this->lastSize().height()));
}

bool ContentWidget::onDownRaw(float const& timeMs, geom::Point const& ptInWidget)
{
	return true;
}

bool ContentWidget::onUpRaw(float const& timeMs, geom::Point const& ptInWidget)
{
	return true;
}

bool ContentWidget::onSingleTapUp(float const& timeMs, geom::Point const& ptInWidget)
{
	return true;
}

bool ContentWidget::onZoom(float const& timeMs, geom::Point const& center, const float ratio)
{
	return true;
}

doc::TreeNode* ContentWidget::findTreeNodeById(const std::string& id)
{
	return this->rootNode_->findTreeNodeById(id);
}

chisa::geom::Area ContentWidget::findTargetImpl(const std::string& target)
{
	doc::TreeNode* node = this->rootNode_->findTreeNodeById(target);
	if( unlikely(!node || !node->block()) ){
		return chisa::geom::Area();
	}
	return node->block()->area();
}

}}

