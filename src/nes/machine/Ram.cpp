#include "VirtualMachine.hpp"

namespace nes {

Ram::Ram(VirtualMachine& vm)
:vm_(vm)
,debugger_(vm.debugger())
{

}

}
