#include "VirtualMachine.hpp"

namespace nes {

IOPort::IOPort(VirtualMachine& vm, GamepadFairy* pad1, GamepadFairy* pad2)
:vm_(vm)
,debugger_(vm.debugger())
,pad1Fairy(pad1 == 0 ? dummyPad : *pad1)
,pad2Fairy(pad2 == 0 ? dummyPad : *pad2)
,pad1Idx(GamepadFairy::A)
,pad2Idx(GamepadFairy::A)
{

}


}
