#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

class Super40in1Ws : public BaseMapper
{
private:
	uint8_t _reg[2];
	uint8_t _latch;

protected:
	uint32_t GetDipSwitchCount() override { return 2; }
	uint16_t GetPRGPageSize() override { return 0x4000; }
	uint16_t GetCHRPageSize() override { return 0x2000; }
	uint16_t RegisterStartAddress() override { return 0x6000; }
	uint16_t RegisterEndAddress() override { return 0xFFFF; }
	bool AllowRegisterRead() override { return true; }

	void InitMapper() override
	{
		_reg[0] = _reg[1] = _latch = 0;
		UpdateState();

		RemoveRegisterRange(0x6000, 0x7FFF, MemoryOperation::Read);
	}

	void StreamState(bool saving) override
	{
		BaseMapper::StreamState(saving);
		Stream(_reg[0], _reg[1], _latch);
	}

	void UpdateState()
	{
		uint8_t prgPage = ((_reg[0] >> 3) & 0x08) | (_reg[0] & 0x07); 
		uint8_t chrPage = ((_reg[0] >> 3) & 0x08) | (_reg[1] & 0x07); 

		if(_reg[1] & 0x10) {
			SelectCHRPage(0, chrPage);
		} else {
			uint8_t chrMask = ((~_reg[1] >> 4) & 0x02) | 0x01;
			SelectCHRPage(0, (chrPage & ~chrMask) | (_latch & chrMask));
		}

		SelectPRGPage(0, prgPage & ~(~_reg[0] >> 3 & 0x01));
		SelectPRGPage(1, prgPage | (~_reg[0] >> 3 & 0x01));

		SetMirroringType((_reg[0] & 0x10) ? MirroringType::Horizontal : MirroringType::Vertical);
	}

	uint8_t ReadRegister(uint16_t addr) override
	{
		// There is no DPSW 3 setting, doing so causes rom to freeze
		bool openbus = GetDipSwitches() != 0x03 && ((_reg[1] & 0xC0) & (GetDipSwitches() << 6));
		return openbus ? _console->GetMemoryManager()->GetOpenBus() : InternalReadRam(addr);
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if(addr < 0x8000) {
			if(!(_reg[0] & 0x20)) {
				_reg[addr & 0x01] = value;
				UpdateState();
			}
		} else {
			_latch = value;
			UpdateState();
		}
	}
};