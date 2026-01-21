#include "norGate.h"

namespace sim
{
    NorGate::NorGate(std::string instanceName)
        : IcPart("NOR", std::move(instanceName), PartKind::IC)
    {
        addPin("A", PinDir::Input);   // 0
        addPin("B", PinDir::Input);   // 1
        addPin("Y", PinDir::Output);  // 2
    }

    void NorGate::onReset()
    {
        setLogicLevel(2, LogicLevel::unconnected);
    }

    void NorGate::onTick()
    {
        LogicLevel a = read(0);
        LogicLevel b = read(1);

        LogicLevel t = logicOr(a, b);
        LogicLevel y = logicNot(t);

        setLogicLevel(2, y);
    }

} // namespace sim
