#pragma once
#include "stdafx.h"
#include "MMC3.h"

// Mapper 126 CHR A18/A19 reversed
// Mapper 422
// Mapper 534 $C000 value inverted

class MMC3_422 : public MMC3
{
protected:
	uint8_t _exRegs[4];

	virtual uint16_t GetChrOuterBank()
	{
		return ((_exRegs[0] << 4) & 0x380);
	}

	virtual void WriteMMC3(uint16_t addr, uint8_t value)
	{
		MMC3::WriteRegister(addr, value);
	}

private:
	uint8_t _solderPad;

	uint32_t GetDipSwitchCount() override { return 2; }
	uint16_t RegisterStartAddress() override { return 0x6000; }
	uint16_t RegisterEndAddress() override { return 0xFFFF; }
	bool AllowRegisterRead() override { return true; }

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType = PrgMemoryType::PrgRom) override
	{
		uint8_t mask = (_exRegs[0] & 0x40) ? 0x0F: 0x1F;
		uint8_t outerBank = (((_exRegs[0] << 3) & 0x180) & ~mask) | ((_exRegs[0] << 4) & 0x70);

		if(!(_exRegs[3] & 0x03)) {				// MMC3
			MMC3::SelectPRGPage(slot, outerBank | (page & mask));
		} else {
			page = (outerBank & ~mask) | (MMC3::_registers[6] & mask);
			if((_exRegs[3] & 0x03) == 0x03) {	// NROM-256
				page &= 0xFE;
				MMC3::SelectPRGPage(0, page);
				MMC3::SelectPRGPage(1, page + 1);
				MMC3::SelectPRGPage(2, page + 2);
				MMC3::SelectPRGPage(3, page + 3);
			} else {							// NROM-128
				MMC3::SelectPRGPage(0, page);
				MMC3::SelectPRGPage(1, page + 1);
				MMC3::SelectPRGPage(2, page);
				MMC3::SelectPRGPage(3, page + 1);
			}
		}
	}

	void SelectCHRPage(uint16_t slot, uint16_t page, ChrMemoryType memoryType) override
	{
		uint8_t mask = (_exRegs[0] & 0x80) ? 0x7F: 0xFF;

		if(!(_exRegs[3] & 0x10)) {
			MMC3::SelectCHRPage(slot, (GetChrOuterBank() & ~mask) | (page & mask));
		}
	}

	void UpdateChrCnrom() {
		uint8_t mask = (_exRegs[0] & 0x80) ? 0x7F: 0xFF;
		uint16_t page = ((GetChrOuterBank() & ~mask) >> 3) | (_exRegs[2] & (mask >> 3));

		for(int i = 0; i < 8; i++) {
			MMC3::SelectCHRPage(i, (page << 3) + i);
		}
	}

	void InitMapper() override
	{
		MMC3::InitMapper();
		memset(_exRegs, 0, sizeof(_exRegs));

		RemoveRegisterRange(0x6000, 0x7FFF, MemoryOperation::Read);
	}

	void Reset(bool softreset) override
	{
		_solderPad = GetDipSwitches();
		memset(_exRegs, 0, sizeof(_exRegs));
		MMC3::ResetMmc3();
		UpdateState();
	}

	void StreamState(bool saving) override
	{
		MMC3::StreamState(saving);
		Stream(_exRegs[0], _exRegs[1], _exRegs[2], _exRegs[3]);
	}

	uint8_t ReadRegister(uint16_t addr) override
	{
		uint8_t value = InternalReadRam(addr);
		if(_exRegs[1] & 0x01) {
			value &= ~0x03;
			value |= _solderPad & 0x03;
		}
		return value;
	}

	virtual void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr < 0x8000) {
			WritePrgRam(addr, value);
			if(!(_exRegs[3] & 0x80)) {
				_exRegs[addr & 0x03] = value;
				if(_exRegs[3] & 0x10) {
					UpdateChrCnrom();
				} else {
					MMC3::UpdateChrMapping();
				}
				MMC3::UpdatePrgMapping();
			} else {
				if((addr & 0x03) == 0x02) {
					uint8_t mask = ((~_exRegs[2] >> 3) & 0x02) | 0x01;
					_exRegs[2] &= ~mask;
					_exRegs[2] |= value & mask;
					UpdateChrCnrom();
				}
			}
		} else {
			WriteMMC3(addr, value);
		}
	}
};