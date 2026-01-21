#include "xorGate.h"

namespace sim
{
    XorGate::XorGate(std::string instanceName)
        : IcPart("XOR", std::move(instanceName), PartKind::IC)
    {
        addPin("A", PinDir::Input);   // 0
        addPin("B", PinDir::Input);   // 1
        addPin("Y", PinDir::Output);  // 2
    }

    void XorGate::onReset()
    {
        setLogicLevel(2, LogicLevel::unconnected);
    }

    void XorGate::onTick()
    {
        LogicLevel a = read(0);
        LogicLevel b = read(1);

        LogicLevel y = logicXor(a, b);

        setLogicLevel(2, y);
    }

} // namespace sim
