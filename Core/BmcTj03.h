#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

class BmcTj03 : public BaseMapper
{
protected:
	virtual uint16_t GetPRGPageSize() override { return 0x8000; }
	virtual uint16_t GetCHRPageSize() override { return 0x2000; }

	void InitMapper() override
	{
		SelectPRGPage(0, 0);
		SelectCHRPage(0, 0);
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		uint8_t bank = (addr >> 8) & 0x07;
		SelectPRGPage(0, bank);
		SelectCHRPage(0, bank);

		SetMirroringType(addr & 0x200 ? MirroringType::Horizontal : MirroringType::Vertical);
	}
};
