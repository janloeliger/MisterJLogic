#include "andGate.h"

namespace sim
{
    AndGate::AndGate(std::string instanceName)
        : Part("AND", std::move(instanceName), PartKind::IC)
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
        LogicLevel pinA = read(0);
        LogicLevel pinB = read(1);
        LogicLevel pinY = LogicLevel::Low;

        if (pinA == LogicLevel::X || pinB == LogicLevel::X)
        {
            pinY = LogicLevel::X;
        }
        else if (pinA == LogicLevel::unconnected || pinB == LogicLevel::unconnected)
        {
            pinY = LogicLevel::unconnected;
        }
        else if (pinA == LogicLevel::High && pinB == LogicLevel::High)
        {
            pinY = LogicLevel::High;
        }
        else
        {
            pinY = LogicLevel::Low;
        }

        setLogicLevel(2, pinY);
    }

} // namespace sim
