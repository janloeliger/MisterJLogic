#include "andGate.h"

namespace sim
{
    AndGate::AndGate(std::string instanceName)
        : IcPart("AND", std::move(instanceName))
    {
        addPin("A", PinDir::Input);   // index 0
        addPin("B", PinDir::Input);   // index 1
        addPin("Y", PinDir::Output);  // index 2
    }

    void AndGate::onReset()
    {
        // output floats until we compute
        setLogicLevel(2, LogicLevel::unconnected);
    }

    void AndGate::onTick()
    {
        LogicLevel a = read(0);
        LogicLevel b = read(1);
        LogicLevel y = logicAnd(a, b);

        setLogicLevel(2, y);
    }

} // namespace sim
