#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

class BmcCtc09 : public BaseMapper
{
protected:
	uint16_t GetPRGPageSize() override { return 0x4000; }
	uint16_t GetCHRPageSize() override { return 0x2000; }

	void InitMapper() override
	{
		WriteRegister(0x8000, 0);
		WriteRegister(0xC000, 0);
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr >= 0xC000) {
			if (value & 0x10) {
				SelectPRGPage(0, ((value << 1) & 0x0E) | ((value >> 3) & 0x01));
				SelectPRGPage(1, ((value << 1) & 0x0E) | ((value >> 3) & 0x01));
			} else {
				SelectPrgPage2x(0, (value & 0x07) << 1);
			}
			SetMirroringType(((value & 0x40) == 0x40) ? MirroringType::Horizontal : MirroringType::Vertical);
		} else {
			SelectCHRPage(0, value & 0x0F);
		}
	}
};