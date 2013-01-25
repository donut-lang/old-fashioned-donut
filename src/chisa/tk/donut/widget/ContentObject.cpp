/**
 * Chisa
 * Copyright (C) 2013 psi
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

#include <donut/object/Heap.h>
#include "ContentObject.h"

namespace chisa {
using namespace tarte;
using namespace donut;
namespace tk {

ContentObject::ContentObject(ContentProvider* provider)
:Super(provider)
{
}

void ContentObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void ContentObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

typename ContentObject::ResultType ContentObject::execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val)
{
	AntiSideEffect anti;
	switch(val.op){
	case AntiSideEffect::LoadDocument:
		anti.op = AntiSideEffect::LoadDocument;
		anti.docId = this->widget()->documentId();
		this->widget()->loadDocument(val.docId);
		break;
	case AntiSideEffect::None:
		break;
	}
	return std::tuple<bool, AntiSideEffect>(true, anti);
}

typename ContentObject::ResultType ContentObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onBack(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

typename ContentObject::ResultType ContentObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onForward(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

XValue ContentObject::saveImpl(const Handler<Heap>& heap)
{
}

void ContentObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

std::tuple<std::string,bool, ContentSideEffect> ContentObject::loadDocument(const std::string& id)
{
	ContentSideEffect anti;
	Handler<ContentWidget> cwidget = this->widget();
	anti.op = ContentSideEffect::LoadDocument;
	anti.docId = cwidget->documentId();
	cwidget->loadDocument(id);
	return std::tuple<std::string,bool, ContentSideEffect>(id, true, anti);
}

//---------------------------------------------------------------------------------------

ContentProvider::ContentProvider(Handler<Heap> const& heap, Handler<World> const& world)
:Super(heap, world)
{
	this->registerReactiveNativeClosure("loadDocument", &ContentObject::loadDocument);
}

}
}
