#include "nandGate.h"

namespace sim
{
    NandGate::NandGate(std::string instanceName)
        : IcPart("NAND", std::move(instanceName), PartKind::IC)
    {
        addPin("A", PinDir::Input);   // 0
        addPin("B", PinDir::Input);   // 1
        addPin("Y", PinDir::Output);  // 2
    }

    void NandGate::onReset()
    {
        setLogicLevel(2, LogicLevel::unconnected);
    }

    void NandGate::onTick()
    {
        LogicLevel a = read(0);
        LogicLevel b = read(1);

        LogicLevel t = logicAnd(a, b);
        LogicLevel y = logicNot(t);

        setLogicLevel(2, y);
    }

} // namespace sim
