#pragma once
#include "stdafx.h"
#include "MMC3.h"

class MMC3_BmcL6in1 : public MMC3
{
private:
	uint8_t _exReg;

protected:
	void InitMapper() override
	{
		AddRegisterRange(0x6000, 0x7FFF, MemoryOperation::Write);
		_exReg = 0;

		MMC3::InitMapper();
	}

	void Reset(bool softreset) override
	{
		_exReg = 0;
		UpdateState();
	}

	void StreamState(bool saving) override
	{
		MMC3::StreamState(saving);
		Stream(_exReg);
	}

	void UpdateMirroring() override
	{
		if(_exReg & 0x20) {
			SetMirroringType((_exReg & 0x10) ? MirroringType::ScreenBOnly : MirroringType::ScreenAOnly);
		} else {
			MMC3::UpdateMirroring();
		}
	}

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType = PrgMemoryType::PrgRom) override
	{
		if(_exReg & 0x0C) {
			MMC3::SelectPRGPage(slot, (page & 0x0F) | (_exReg & 0xC0) >> 2);
		} else {
			uint8_t bank = ((_exReg & 0xC0) >> 4) | (_exReg & 0x03);
			SelectPrgPage4x(0, bank << 2);
		}
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr < 0x8000) {
			if(CanWriteToWorkRam()) {
				_exReg = value;
				UpdatePrgMapping();
				UpdateMirroring();
			}
		} else {
			MMC3::WriteRegister(addr, value);
		}
	}
};