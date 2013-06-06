/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/object/NativeClosureObject.h>
#include <donut/provider/NativeClosureProvider.h>

namespace donut {

PureNativeClosureProvider::PureNativeClosureProvider( Handler<Heap> const& heap )
:Super(heap, "PureNativeClosureObject")
{
}

ReactiveNativeClosureProvider::ReactiveNativeClosureProvider( Handler<Heap> const& heap )
:Super(heap, "ReactiveNativeClosureObject")
{
}

}
