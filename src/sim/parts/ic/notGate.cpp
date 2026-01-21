#include "notGate.h"

namespace sim
{
    NotGate::NotGate(std::string instanceName)
        : IcPart("NOT", std::move(instanceName))
    {
        addPin("A", PinDir::Input);
        addPin("Y", PinDir::Output);
    }

    void NotGate::onReset()
    {
        // output floats until we compute
        setLogicLevel(1, LogicLevel::unconnected);
    }

    void NotGate::onTick()
    {
        LogicLevel a = read(0);
        LogicLevel y = logicNot(a);

        setLogicLevel(1, y);
    }

} // namespace sim
