#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

class Mapper225 : public BaseMapper
{
protected:
	virtual uint16_t GetPRGPageSize() override { return 0x4000; }
	virtual uint16_t GetCHRPageSize() override { return 0x2000; }
	bool AllowRegisterRead() override { return true; }

	uint8_t RAM[4];

	void InitMapper() override
	{
		memset(RAM, 0, sizeof(RAM));

		SelectPRGPage(0, 0);
		SelectPRGPage(1, 1);
		SelectCHRPage(0, 0);

		AddRegisterRange(0x5800, 0x5FFF, MemoryOperation::Any);
		RemoveRegisterRange(0x8000, 0xFFFF, MemoryOperation::Read);
	}

	uint8_t ReadRegister(uint16_t addr) override
	{
		return RAM[addr & 0x03] & 0x0F;
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr <= 0x5FFF) {
			RAM[addr & 0x03] = value;
			return;
		}

		uint8_t highBit = (addr >> 8) & 0x40;
		uint8_t prgPage = ((addr >> 6) & 0x3F) | highBit;
		if(addr & 0x1000) {
			SelectPRGPage(0, prgPage);
			SelectPRGPage(1, prgPage);
		} else {
			SelectPRGPage(0, prgPage & 0xFE);
			SelectPRGPage(1, (prgPage & 0xFE) + 1);
		}

		SelectCHRPage(0, (addr & 0x3F) | highBit);

		SetMirroringType(addr & 0x2000 ? MirroringType::Horizontal : MirroringType::Vertical);
	}
};
