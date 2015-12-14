/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Debugger.hpp"

namespace nes {

Debugger::Debugger(VirtualMachine& vm, DebuggerFairy& debuggerFairy)
:vm_(vm)
,disasm_(vm)
,watcher_(vm, debuggerFairy)
,comparator_(vm)
{

}

}
