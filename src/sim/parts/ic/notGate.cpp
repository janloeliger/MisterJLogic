#include "notGate.h"

namespace sim
{
    NotGate::NotGate(std::string instanceName)
        : Part("NOT", std::move(instanceName), PartKind::IC)
    {
        // Two pins: A input, Y output
        addPin("A", PinDir::Input);
        addPin("Y", PinDir::Output);
    }

    void NotGate::onReset()
    {
        // output floats until we compute
        setLogicLevel(1, LogicLevel::unconnected);
    }

    LogicLevel NotGate::invert(LogicLevel in) const
    {
        if (in == LogicLevel::High)
        {
            return LogicLevel::Low;
        }

        if (in == LogicLevel::Low)
        {
            return LogicLevel::High;
        }

        if (in == LogicLevel::unconnected)
        {
            return LogicLevel::unconnected;
        }

        return LogicLevel::X;
    }

    void NotGate::onTick()
    {
        LogicLevel a = read(0);
        LogicLevel y = invert(a);

        setLogicLevel(1, y);
    }

} // namespace sim
