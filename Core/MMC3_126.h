#pragma once
#include "stdafx.h"
#include "MMC3_422.h"

class MMC3_126 : public MMC3_422
{
private:
	uint16_t GetChrOuterBank() override
	{
		return ((_exRegs[0] << 4) & 0x080) |
			((_exRegs[0] << 3) & 0x100) |
			((_exRegs[0] << 5) & 0x200);
	}
};