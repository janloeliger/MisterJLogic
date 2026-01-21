#include "norGate.h"

namespace sim
{
    NorGate::NorGate(std::string instanceName)
        : IcPart("NOR", std::move(instanceName))
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

        LogicLevel orResult = logicOr(a, b);
        LogicLevel y = logicNot(orResult);

        setLogicLevel(2, y);
    }

} // namespace sim
