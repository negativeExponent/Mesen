#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

class BmcK3036 : public BaseMapper
{
protected:
	virtual uint16_t GetPRGPageSize() override { return 0x4000; }
	virtual uint16_t GetCHRPageSize() override { return 0x2000; }

	void InitMapper() override
	{
		WriteRegister(0x8000, 0);
	}

	void Reset(bool softReset) override
	{
		BaseMapper::Reset(softReset);
		WriteRegister(0x8000, 0);
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if (addr & 0x20) {					// NROM-128
			SelectPRGPage(0, addr & 0x1F);
			SelectPRGPage(1, addr & 0x1F);
		} else {							// UNROM
			SelectPRGPage(0, (addr & 0x1F) | (value & 0x07));
			SelectPRGPage(1, (addr & 0x1F) | 0x07);
		}
		SelectCHRPage(0, 0, ChrMemoryType::ChrRam);
		SetMirroringType(((addr & 0x25) == 0x25) ?  MirroringType::Horizontal : MirroringType::Vertical);
	}
};