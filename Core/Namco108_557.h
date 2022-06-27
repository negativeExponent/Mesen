#pragma once

#include "stdafx.h"
#include "Namco108.h"

class Namco108_557 : public Namco108
{
protected:
	virtual uint16_t GetCHRPageSize() override { return 0x2000; }

	virtual void UpdatePrgMapping() override
	{
		SetCpuMemoryMapping(0x6000, 0x7FFF, 0, PrgMemoryType::WorkRam);

		SelectPRGPage(0, _registers[6] & 0x0F);
		SelectPRGPage(1, _registers[7] & 0x0F);
		SelectPRGPage(2, (-2) & 0x0F);
		SelectPRGPage(3, (-1) & 0x0F);
	}

	virtual void UpdateChrMapping() override
	{
		SelectCHRPage(0, 0);
	}

	virtual void UpdateState() override
	{
		SetMirroringType((_registers[5] & 0x20) ? MirroringType::Horizontal : MirroringType::Vertical);

		Namco108::UpdateState();
	}
};