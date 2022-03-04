#pragma once
#include "stdafx.h"
#include "VrcIrq.h"

class Unl831128C : public BaseMapper
{
private:
	unique_ptr<VrcIrq> _irq;

protected:
	virtual uint16_t GetPRGPageSize() override { return 0x2000; }
	virtual uint16_t GetCHRPageSize() override { return 0x400; }
	virtual uint32_t GetWorkRamSize() override { return 0x8000; }
	virtual uint32_t GetWorkRamPageSize() override { return 0x2000; }

	void InitMapper() override
	{
		_irq.reset(new VrcIrq(_console));

		SelectPRGPage(2, -2);
		SelectPRGPage(3, -1);

		RemoveRegisterRange(0x8000, 0x9FFF, MemoryOperation::Write);
		RemoveRegisterRange(0xD000, 0xFFFF, MemoryOperation::Write);
	}

	void ProcessCpuClock() override
	{
		_irq->ProcessCpuClock();
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		uint8_t gameReg = (addr >> 10) & 0x10;
		uint8_t prgMask = 0x0F | gameReg;

		switch(addr & 0xF) {
			case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
				SelectCHRPage(addr & 0x07, value + (gameReg << 4));
				break;

			case 8:
				if (value == 1) {
					SetCpuMemoryMapping(0x6000, 0x7FFF, 0, PrgMemoryType::WorkRam, MemoryAccessType::ReadWrite);
				} else {
					SetCpuMemoryMapping(0x6000, 0x7FFF, (value & prgMask) + gameReg, PrgMemoryType::PrgRom);
				}
				break;

			case 9: case 0xA:
				SelectPRGPage((addr >> 1) & 0x01, (value & prgMask) + gameReg);
				break;

			case 0xC:
				switch(value & 0x03) {
					case 0: SetMirroringType(MirroringType::Vertical); break;
					case 1: SetMirroringType(MirroringType::Horizontal); break;
					case 2: SetMirroringType(MirroringType::ScreenAOnly); break;
					case 3: SetMirroringType(MirroringType::ScreenBOnly); break;
				}
				break;

			case 0xD:
				_irq->SetControlValue(value);
				break;

			case 0xE:
				_irq->AcknowledgeIrq();
				break;

			case 0xF:
				_irq->SetReloadValue(value);
				break;
		}

		SelectPRGPage(2, (-2 & prgMask) + gameReg);
		SelectPRGPage(3, (-1 & prgMask) + gameReg);
	}
};