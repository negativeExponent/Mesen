#pragma once
#include "stdafx.h"
#include "MMC3_126.h"

class MMC3_534 : public MMC3_126
{
private:
	uint16_t GetChrOuterBank() override
	{
		return ((_exRegs[0] << 4) & 0x380);
	}

	void WriteMMC3(uint16_t addr, uint8_t value) override
	{
		if((addr >= 0xC000) && (addr <= 0xDFFF)) {
			value ^= 0xFF;
		}

		MMC3::WriteRegister(addr, value);
	}
};