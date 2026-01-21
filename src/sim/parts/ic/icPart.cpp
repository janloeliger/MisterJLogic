#include "icPart.h"

namespace sim
{
    IcPart::IcPart(std::string typeName, std::string instanceName, PartKind kind)
        : Part(std::move(typeName), std::move(instanceName), kind)
    {
    }

    LogicLevel IcPart::logicNot(LogicLevel a) const
    {
        if (a == LogicLevel::High)
        {
            return LogicLevel::Low;
        }

        if (a == LogicLevel::Low)
        {
            return LogicLevel::High;
        }

        if (a == LogicLevel::unconnected)
        {
            return LogicLevel::unconnected;
        }

        return LogicLevel::X;
    }

    LogicLevel IcPart::logicAnd(LogicLevel a, LogicLevel b) const
    {
        if (a == LogicLevel::X || b == LogicLevel::X)
        {
            return LogicLevel::X;
        }

        if (a == LogicLevel::unconnected || b == LogicLevel::unconnected)
        {
            return LogicLevel::unconnected;
        }

        if (a == LogicLevel::High && b == LogicLevel::High)
        {
            return LogicLevel::High;
        }

        return LogicLevel::Low;
    }

    LogicLevel IcPart::logicOr(LogicLevel a, LogicLevel b) const
    {
        // OR = NOT( AND( NOT(a), NOT(b) ) )
        LogicLevel na = logicNot(a);
        LogicLevel nb = logicNot(b);
        LogicLevel t = logicAnd(na, nb);
        return logicNot(t);
    }

    LogicLevel IcPart::logicXor(LogicLevel a, LogicLevel b) const
    {
        // XOR = (A OR B) AND NOT(A AND B)
        LogicLevel t0 = logicOr(a, b);
        LogicLevel t1 = logicAnd(a, b);
        LogicLevel t2 = logicNot(t1);
        return logicAnd(t0, t2);
    }

    bool IcPart::toBit(LogicLevel v, bool& ok) const
    {
        if (v == LogicLevel::High)
        {
            return true;
        }

        if (v == LogicLevel::Low)
        {
            return false;
        }

        ok = false;
        return false;
    }

    LogicLevel IcPart::fromBit(bool bit) const
    {
        if (bit)
        {
            return LogicLevel::High;
        }

        return LogicLevel::Low;
    }

    void IcPart::addBus(const std::string& baseName, size_t width, PinDir dir)
    {
        for (size_t i = 0; i < width; i++)
        {
            addPin(baseName + std::to_string(i), dir);
        }
    }

    uint32_t IcPart::readBus(size_t firstPinIndex, size_t width, bool& ok) const
    {
        ok = true;

        uint32_t value = 0;

        for (size_t i = 0; i < width; i++)
        {
            LogicLevel v = read(firstPinIndex + i);

            bool localOk = true;
            bool bit = toBit(v, localOk);

            if (!localOk)
            {
                ok = false;
            }

            if (bit)
            {
                value |= (1u << static_cast<uint32_t>(i));
            }
        }

        return value;
    }

    void IcPart::writeBus(size_t firstPinIndex, size_t width, uint32_t value)
    {
        for (size_t i = 0; i < width; i++)
        {
            bool bit = false;

            if ((value & (1u << static_cast<uint32_t>(i))) != 0u)
            {
                bit = true;
            }

            setLogicLevel(firstPinIndex + i, fromBit(bit));
        }
    }

} // namespace sim
