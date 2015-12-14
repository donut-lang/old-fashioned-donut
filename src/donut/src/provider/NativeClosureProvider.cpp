/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/object/NativeClosureObject.hpp>
#include <donut/provider/NativeClosureProvider.hpp>

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
