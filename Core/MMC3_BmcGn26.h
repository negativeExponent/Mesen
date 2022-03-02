#pragma once
#include "stdafx.h"
#include "MMC3.h"

class MMC3_BmcGn26 : public MMC3
{
private:
	uint8_t _exReg;

protected:
	void InitMapper() override
	{
		_exReg = 0;
		MMC3::InitMapper();
		AddRegisterRange(0x6000, 0x7FFF, MemoryOperation::Write);
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
		uint8_t mask = (_exReg & 0x04) ? 0xFF : 0x7F;
		page &= mask;
		page |= ((_exReg << 7) & ~mask);
		MMC3::SelectCHRPage(slot, page, memoryType);
	}

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType = PrgMemoryType::PrgRom) override
	{
		if (_exReg & 0x04) {
			if (slot == 0) {
				page >>= 2;
				page |= (_exReg << 2);
				SelectPrgPage4x(0, page << 2);
			}
		} else {
			page &= 0x0F;
			page |= (_exReg << 4);
			MMC3::SelectPRGPage(slot, page);
		}
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr <= 0x7FFF) {
			if (CanWriteToWorkRam()) {
				_exReg = (uint8_t)addr;
				UpdateState();
			}
		} else {
			MMC3::WriteRegister(addr, value);
		}
	}
};