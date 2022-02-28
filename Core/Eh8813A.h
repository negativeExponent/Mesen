#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

class Eh8813A : public BaseMapper
{
private:
	uint8_t _prgBank;
	uint8_t _chrBank;
	bool _alterReadAddress;
	bool _lock;
	bool _horizontalMirorring;
	bool _nrom128;

protected:
	uint32_t GetDipSwitchCount() override { return 4; }
	uint16_t GetPRGPageSize() override { return 0x4000; }
	uint16_t GetCHRPageSize() override { return 0x2000; }
	bool AllowRegisterRead() override {	return true; }

	void InitMapper() override
	{
		_lock = 0;
		_nrom128 = 0;
		_prgBank = 0;
		_chrBank = 0;
		_alterReadAddress = 0;
		_horizontalMirorring = 0;
		UpdateState();
	}

	void Reset(bool softReset) override
	{
		_lock = 0;
		_nrom128 = 0;
		_prgBank = 0;
		_chrBank = 0;
		_alterReadAddress = 0;
		_horizontalMirorring = 0;
		UpdateState();
	}

	void StreamState(bool saving) override
	{
		BaseMapper::StreamState(saving);
		Stream(_alterReadAddress, _prgBank, _chrBank, _lock, _horizontalMirorring, _nrom128);
	}

	void UpdateState()
	{
		if (_nrom128) {
			SelectPRGPage(0, _prgBank);
			SelectPRGPage(1, _prgBank);
		} else {
			SelectPrgPage2x(0, _prgBank & 0x1E);
		}

		SelectCHRPage(0, _chrBank);
		SetMirroringType(_horizontalMirorring ? MirroringType::Horizontal : MirroringType::Vertical);
	}

	uint8_t ReadRegister(uint16_t addr) override
	{
		if(_alterReadAddress) {
			addr = (addr & 0xFFF0) + GetDipSwitches();
		}
		return InternalReadRam(addr);
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		if (_lock == false) {
			_prgBank = addr & 0x3F;
			_alterReadAddress = (addr & 0x40) == 0x40;
			_nrom128 = (addr & 0x80) == 0x80;
			_lock = (addr & 0x100) == 0x100;
			_chrBank = value & 0x7F;
			_horizontalMirorring = (value & 0x80) == 0x80;
		}
		_chrBank = (_chrBank & 0xFC) | (value & 0x03);
		UpdateState();
	}
};