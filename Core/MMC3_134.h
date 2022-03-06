#pragma once

#include "stdafx.h"
#include "MMC3.h"

// T4A54A, WX-KB4K, or BS-5652

class MMC3_134 : public MMC3
{
private:
	uint8_t _mode;
	uint8_t _outerBank;
	uint8_t _cnrom;

	void SelectCHRPage(uint16_t slot, uint16_t page, ChrMemoryType memoryType = ChrMemoryType::Default) override
	{
		uint16_t mask = (_outerBank & 0x40) ? 0x7F: 0xFF;
		uint16_t outerBank = ((_outerBank << 3) & 0x180);

		if(_mode & 0x08) {
			page = ((outerBank & ~mask) >> 3) | (_cnrom & (mask >> 3));
			BaseMapper::SelectChrPage8x(0, page << 3, memoryType);
		} else {
			BaseMapper::SelectCHRPage(slot, (outerBank & ~mask) | (page & mask), memoryType);
		}
	}

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType = PrgMemoryType::PrgRom) override
	{
		uint16_t mask = (_outerBank & 0x04) ? 0x0F: 0x1F;
		uint16_t outerBank = (_outerBank << 4) & 0x30;

		if(_outerBank & 0x80) {		// NROM
			page = (outerBank & ~mask) | (MMC3::_registers[6] & mask);
			page >>= 1;
			if(_outerBank & 0x08) {	// NROM-128
				BaseMapper::SelectPrgPage2x(0, page << 1, memoryType);
				BaseMapper::SelectPrgPage2x(1, page << 1, memoryType);
			} else {				// NROM-256
				BaseMapper::SelectPrgPage4x(0, (page & 0xFE) << 1, memoryType);
			}
		} else {
			BaseMapper::SelectPRGPage(slot, (outerBank & ~mask) | (page & mask));
		}
	}

	void InitMapper() override 
	{
		MMC3::InitMapper();

		AddRegisterRange(0x6000, 0x7FFF, MemoryOperation::Write);
	}

	void Reset(bool softReset) override 
	{
		BaseMapper::Reset(softReset);

		if(softReset) {
			_mode = 0;
			_outerBank = 0;
			_cnrom = 0;
			MMC3::UpdateState();
		}
	}
	
	void StreamState(bool saving) override
	{
		MMC3::StreamState(saving);
		Stream(_mode, _outerBank, _cnrom);
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr < 0x8000) {
			if((_mode & 0x80) == 0) {
				switch (addr & 0x03) {
					case 0:
						_mode = value;
						break;

					case 1:
						_outerBank = value;
						break;

					case 2:
						_cnrom = value;
						break;
				}
			} else if((addr & 0x03) == 2) {
				_cnrom &= ~0x03;
				_cnrom |= value & 0x03;
			}
			MMC3::UpdateState();
		} else {
			MMC3::WriteRegister(addr, value);
		}
	}
};