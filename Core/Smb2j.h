#pragma once
#include "stdafx.h"
#include "BaseMapper.h"
#include "CPU.h"

class Smb2j : public BaseMapper
{
private:
	bool _irqEnabled;

protected:
	uint16_t GetPRGPageSize() override { return 0x2000; }
	uint16_t GetCHRPageSize() override { return 0x2000; }
	bool AllowRegisterRead() override { return true; }

	void InitMapper() override
	{
		SelectPrgPage4x(0, 0);
		SelectCHRPage(0, 0);
		SetCpuMemoryMapping(0x6000, 0x7FFF, 4, PrgMemoryType::PrgRom);

		_irqEnabled = false;

		AddRegisterRange(0x4042, 0x4055, MemoryOperation::Read);
		AddRegisterRange(0x4027, 0x4027, MemoryOperation::Write);
		AddRegisterRange(0x4068, 0x4068, MemoryOperation::Write);
		RemoveRegisterRange(0x8000, 0xFFFF, MemoryOperation::Any);
	}

	void StreamState(bool saving) override
	{
		BaseMapper::StreamState(saving);
		Stream(_irqEnabled);
	}

	void ProcessCpuClock() override
	{
		if(_irqEnabled) {
			_console->GetCpu()->SetIrqSource(IRQSource::External);
		}
	}

	uint8_t ReadRegister(uint16_t addr) override
	{
		return 0xFF;
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if((addr & 0x4027) == 0x4027) {
			SetCpuMemoryMapping(0x6000, 0x7FFF, 0x04 | (value & 0x01), PrgMemoryType::PrgRom);
		} else if((addr & 0x4068) == 0x4068) {
			_irqEnabled = (value & 0x01) == 1;
			_console->GetCpu()->ClearIrqSource(IRQSource::External);
		}
	}
};