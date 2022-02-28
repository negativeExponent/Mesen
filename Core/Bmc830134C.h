#pragma once
#include "stdafx.h"
#include "MMC3.h"

class Bmc830134C : public MMC3
{
private:
	uint8_t _exReg;

protected:
	void InitMapper() override
	{
		_exReg = 0;
		MMC3::InitMapper();
		AddRegisterRange(0x6800, 0x68FF, MemoryOperation::Write);
	}

	void Reset(bool softReset) override
	{
		_exReg = 0;
		MMC3::Reset(softReset);
		UpdateState();
	}

	void StreamState(bool saving) override
	{
		MMC3::StreamState(saving);
		Stream(_exReg);
	}

	void SelectCHRPage(uint16_t slot, uint16_t page, ChrMemoryType memoryType = ChrMemoryType::Default) override
	{
		MMC3::SelectCHRPage(slot, ((_exReg & 0x01) << 8) | ((_exReg & 0x02) << 6) | ((_exReg & 0x08) << 3) | (page & 0xFF));
	}

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType = PrgMemoryType::PrgRom) override
	{
		if((_exReg & 0x06) == 0x06) {
			if(slot == 0) {
				MMC3::SelectPRGPage(0, ((_exReg & 0x06) << 3) | (page & 0x0F));
				MMC3::SelectPRGPage(2, 0x32 | (page & 0x0F));
			} else if (slot == 1) {
				MMC3::SelectPRGPage(1, ((_exReg & 0x06) << 3) | (page & 0x0F));
				MMC3::SelectPRGPage(3, 0x32 | (page & 0x0F));
			}
		} else
			MMC3::SelectPRGPage(slot, ((_exReg & 0x06) << 3) | (page & 0x0F));
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr < 0x8000) {
			_exReg = value;
			UpdateState();
		} else {
			MMC3::WriteRegister(addr, value);
		}
	}
};