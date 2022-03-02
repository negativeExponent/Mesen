#pragma once
#include "stdafx.h"
#include "MMC3.h"

class MMC3_BmcK3033 : public MMC3
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
		uint8_t outerBank =	((_exReg & 0x18) >> 3) | ((_exReg & 0x40) >> 4);
		if(_exReg & 0x20) {
			if(_exReg & 0x80) {			// MMC3 256 KiB
				MMC3::SelectCHRPage(slot, (page & 0xFF) | (outerBank << 8));
			} else {					// MMC3 128 KiB
				MMC3::SelectCHRPage(slot, (page & 0x7F) | (outerBank << 7));
			}
		} else {						// NROM mode
			MMC3::SelectCHRPage(slot, (page & 0x7F));
		}
	}

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType = PrgMemoryType::PrgRom) override
	{
		uint8_t outerBank =	((_exReg & 0x18) >> 3) | ((_exReg & 0x40) >> 4);
		if(_exReg & 0x20) {
			if (_exReg & 0x80) {		// MMC3 256 KiB
				MMC3::SelectPRGPage(slot, (page & 0x1F) | (outerBank << 5));
			} else {					// MMC3 128 KiB
				MMC3::SelectPRGPage(slot, (page & 0x0F) | (outerBank << 4));
			}
		} else {
			uint8_t innerBank = _exReg & 7;
			if(innerBank & 3) {			// NROM-256
				SelectPrgPage4x(0, (((outerBank << 3) | innerBank) & 0xFE) << 1);
			} else {					// NROM-128
				SelectPrgPage2x(0, ((outerBank << 3) | innerBank) << 1);
				SelectPrgPage2x(1, ((outerBank << 3) | innerBank) << 1);
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