#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

// BMC-RESETNROM-XIN1
// - Sheng Tian 2-in-1(Unl,ResetBase)[p1] - Kung Fu (Spartan X), Super Mario Bros (alt)
// - Sheng Tian 2-in-1(Unl,ResetBase)[p2] - B-Wings, Twin-beeSMB1 (wrong mirroring)

class BmcResetNromX1n1 : public BaseMapper
{
private:
	uint8_t _game;

protected:
	virtual uint16_t GetPRGPageSize() override { return 0x8000; }
	virtual uint16_t GetCHRPageSize() override { return 0x2000; }

	void InitMapper() override
	{
		_game = 0;

		RemoveRegisterRange(0x8000, 0xFFFF, MemoryOperation::Write);
	}

	void Reset(bool softReset) override
	{
		if(softReset == true) {
			_game++;
			_game &= (GetPRGPageCount() - 1);
		} else {
			_game = 0;
		}
		
		SelectPRGPage(0, _game);
		SelectCHRPage(0, _game);

		if(_romInfo.Hash.PrgChrCrc32 == 0x3470F395) {
			// Sheng Tian 2-in-1(Unl,ResetBase)[p1].unf
			SetMirroringType(MirroringType::Vertical);
		} else {
			// Sheng Tian 2-in-1(Unl,ResetBase)[p2].unf
			SetMirroringType(MirroringType::Horizontal);
		}
	}
};
