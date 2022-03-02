#pragma once
#include "stdafx.h"
#include "MMC3.h"

class MMC3_BmcK3006 : public MMC3
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
	
	void SelectCHRPage(uint16_t slot, uint16_t page, ChrMemoryType memoryType = ChrMemoryType::Default) override
	{
		MMC3::SelectCHRPage(slot, (page & 0x7F) | ((_exReg << 4) & ~0x7F));
	}

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType = PrgMemoryType::PrgRom) override
	{
		if (_exReg & 0x20) {				// MMC3 mode
			MMC3::SelectPRGPage(slot, (page & 0x0F) | ((_exReg << 1) & ~0x0F));
		} else {
			uint8_t bank = (_exReg & 0x1F);
			if ((_exReg & 0x07) == 0x06) {	// NROM-256
				SelectPrgPage4x(0, (bank & 0xFE) << 1);
			} else {						// NROM-128
				SelectPrgPage2x(0, bank << 1);
				SelectPrgPage2x(1, bank << 1);
			}
		}

	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr < 0x8000) {
			if(CanWriteToWorkRam()) {
				_exReg = (uint8_t)addr;
				UpdateState();
			}
		} else {
			MMC3::WriteRegister(addr, value);
		}
	}
};