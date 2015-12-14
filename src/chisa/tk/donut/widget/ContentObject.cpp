/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/object/Heap.hpp>
#include "ContentObject.hpp"

namespace chisa {
using namespace cinamo;
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
	return Just<AntiSideEffect>(anti);
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

std::tuple<std::string, Maybe<ContentSideEffect> > ContentObject::loadDocument(const std::string& id)
{
	ContentSideEffect anti;
	Handler<ContentWidget> cwidget = this->widget();
	anti.op = ContentSideEffect::LoadDocument;
	anti.docId = cwidget->documentId();
	cwidget->loadDocument(id);
	return std::tuple<std::string, Maybe<ContentSideEffect> >(id, Just<ContentSideEffect>(anti));
}

//---------------------------------------------------------------------------------------

ContentProvider::ContentProvider(Handler<Heap> const& heap, Handler<World> const& world)
:Super(heap, world)
{
	this->registerReactiveNativeClosure("loadDocument", &ContentObject::loadDocument);
}

}
}
