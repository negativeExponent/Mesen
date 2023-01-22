#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

// BMC-RESETNROM-XIN1 (Mapper 343)
// submapper 1 - Sheng Tian 2-in-1(Unl,ResetBase)[p1] - Kung Fu (Spartan X), Super Mario Bros (alt)
// submapper 1 - Sheng Tian 2-in-1(Unl,ResetBase)[p2] - B-Wings, Twin-beeSMB1 (wrong mirroring)

class BmcResetNromX1n1 : public BaseMapper
{
private:
	uint8_t _game;

protected:
	virtual uint16_t GetPRGPageSize() override { return 0x4000; }
	virtual uint16_t GetCHRPageSize() override { return 0x2000; }

	void InitMapper() override
	{
		if (!_romInfo.IsNes20Header || !_romInfo.IsInDatabase) {
			if (_romInfo.Hash.PrgChrCrc32 == 0x3470F395 ||	// Sheng Tian 2-in-1(Unl,ResetBase)[p1].unf
			    _romInfo.Hash.PrgChrCrc32 == 0x39F9140F) {	// Sheng Tian 2-in-1(Unl,ResetBase)[p2].unf
				_romInfo.SubMapperID = 1;
			}
		}
	}

	void Reset(bool softReset) override
	{
		if(!softReset) {
			_game = 0;
		}
		UpdateState();
	}

	void StreamState(bool saving) override
	{
		BaseMapper::StreamState(saving);
		Stream(_game);
	}

	void UpdateState()
	{
		if(_romInfo.SubMapperID == 1) {
			SelectPrgPage2x(0, _game << 1);
		} else {
			SelectPRGPage(0, _game);
			SelectPRGPage(1, _game);
		}
		SelectCHRPage(0, _game);
		SetMirroringType((_game & 0x80) ? MirroringType::Vertical : MirroringType::Horizontal);
	}

	void WriteRegister(uint16_t addr, uint8_t value) override
	{
		_game = ~value;
		UpdateState();
	}
};
