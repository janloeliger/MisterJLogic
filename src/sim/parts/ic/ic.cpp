#include "ic.h"

namespace sim
{
    AndGate::AndGate(std::string instanceName)
        : Part("NOT", std::move(instanceName), PartKind::IC)
    {
        addPin("A", PinDir::Input);
        addPin("B", PinDir::Input);
        addPin("Y", PinDir::Output);
    }

    void AndGate::onReset()
    {
        // output floats until we compute
        setLogicLevel(1, LogicLevel::unconnected);
    }

    void AndGate::onTick()
    {
        LogicLevel pinA = read(0);
        LogicLevel pinB = read(1);
        LogicLevel pinY;
        if(pinA == pinB && pinA != LogicLevel::unconnected && pinA != LogicLevel::X)
        {
            pinY = LogicLevel::High;
        }
        else
        {
            pinY = LogicLevel::Low;
        }

        setLogicLevel(1, pinY);
    }

} // namespace sim
