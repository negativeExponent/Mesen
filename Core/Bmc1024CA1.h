#pragma once
#include "stdafx.h"
#include "MMC3.h"

class Bmc1024CA1 : public MMC3
{
private:
	uint8_t _exReg;

protected:
	uint16_t GetChrRamPageSize() override { return 0x400; }
	uint32_t GetChrRamSize() override { return 0x2000; }
	uint32_t GetWorkRamSize() override { return 0x2000; }
	uint32_t GetWorkRamPageSize() override { return 0x2000; }
	bool ForceWorkRamSize() override { return true; }
	
	void InitMapper() override
	{
		AddRegisterRange(0x6000, 0x7FFF, MemoryOperation::Write);
		_exReg = 0;
		MMC3::InitMapper();
	}

	void StreamState(bool saving) override
	{
		MMC3::StreamState(saving);
		Stream(_exReg);
	}

	void SelectCHRPage(uint16_t slot, uint16_t page, ChrMemoryType memoryType = ChrMemoryType::Default) override
	{
		if (_exReg & 0x10) {
			MMC3::SelectCHRPage(slot, page, ChrMemoryType::ChrRam);
		} else if (_exReg & 0x20) {
			MMC3::SelectCHRPage(slot, ((_exReg & 0x07) << 7) | (page & 0xFF));
		} else {
			MMC3::SelectCHRPage(slot, ((_exReg & 0x07) << 7) | (page & 0x7F));
		}
	}

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType = PrgMemoryType::PrgRom) override
	{
		if(_exReg & 0x08) {
			MMC3::SelectPRGPage(slot, ((_exReg & 0x07) << 4) | (page & 0x1F));
		} else {
			MMC3::SelectPRGPage(slot, ((_exReg & 0x07) << 4) | (page & 0x0F));
		}
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr <= 0x7FFF) {
			if(CanWriteToWorkRam()) {
				WritePrgRam(addr, value);
				if ((_exReg & 0x07) == 0) {
					_exReg = addr & 0x3F;
					UpdatePrgMapping();
					UpdateChrMapping();
				}
			}
		} else {
			MMC3::WriteRegister(addr, value);
		}
	}
};