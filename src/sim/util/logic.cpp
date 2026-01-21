#include "logic.h"

namespace sim
{
    const char* logicLevelToStr(LogicLevel v) noexcept
    {
        if (v == LogicLevel::Low)
        {
            return "Low";
        }
        else if (v == LogicLevel::High)
        {
            return "High";
        }
        else if (v == LogicLevel::unconnected)
        {
            return "unconnected";
        }
        else if (v == LogicLevel::X)
        {
            return "X";
        }
        else
        {
            return "error";
        }
    }
} // namespace sim