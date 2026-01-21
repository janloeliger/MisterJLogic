#include "orGate.h"

namespace sim
{
    OrGate::OrGate(std::string instanceName)
        : IcPart("OR", std::move(instanceName), PartKind::IC)
    {
        addPin("A", PinDir::Input);   // 0
        addPin("B", PinDir::Input);   // 1
        addPin("Y", PinDir::Output);  // 2
    }

    void OrGate::onReset()
    {
        setLogicLevel(2, LogicLevel::unconnected);
    }

    void OrGate::onTick()
    {
        LogicLevel a = read(0);
        LogicLevel b = read(1);

        LogicLevel y = logicOr(a, b);

        setLogicLevel(2, y);
    }

} // namespace sim