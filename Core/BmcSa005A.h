#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

// INES Mapper 200 is similar but with the mirroring bit's meaning flipped, and the mirroring bit being also used as a fourth bank bit.

class BmcSa005A : public BaseMapper
{
protected:
	virtual uint16_t GetPRGPageSize() override { return 0x4000; }
	virtual uint16_t GetCHRPageSize() override { return 0x2000; }

	void InitMapper() override
	{
		SelectPRGPage(0, 0);
		SelectPRGPage(1, 0);
		SelectCHRPage(0, 0);
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		uint8_t bank = addr & 0x0F;
		SelectPRGPage(0, bank);
		SelectPRGPage(1, bank);
		SelectCHRPage(0, bank);

		SetMirroringType(addr & 0x08 ? MirroringType::Vertical : MirroringType::Horizontal);
	}
};
