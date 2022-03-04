#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

class UnlBj56 : public BaseMapper
{
private:
	uint16_t _irqCounter;

protected:
	virtual uint16_t GetPRGPageSize() override { return 0x2000; }
	virtual uint16_t GetCHRPageSize() override { return 0x400; }

	void InitMapper() override
	{
		_irqCounter = 0;

		SelectPRGPage(0, -4);
		SelectPRGPage(1, -3);
		SelectPRGPage(2, -2);
		SelectPRGPage(3, -1);

		SetMirroringType(MirroringType::Vertical);

		RemoveRegisterRange(0x9000, 0xFFFF, MemoryOperation::Write);
	}

	void StreamState(bool saving) override 
	{
		BaseMapper::StreamState(saving);
		Stream(_irqCounter);
	}

	void ProcessCpuClock() override
	{
		_irqCounter++;
		if(_irqCounter & 4096) {
			_console->GetCpu()->SetIrqSource(IRQSource::External);
		}
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		switch(addr & 0xF) {
			case 0: case 1: case 2: case 3:
			case 4: case 5: case 6: case 7:
				SelectCHRPage(addr & 0x07, value);
				break;

			case 8: case 9: case 0xA: case 0xB:
				SelectPRGPage(addr & 0x03, value);
				break;

			case 0xD: case 0xF:
				_irqCounter = 0;
				_console->GetCpu()->ClearIrqSource(IRQSource::External);
				break;
		}
	}
};