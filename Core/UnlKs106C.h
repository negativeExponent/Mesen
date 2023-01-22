#pragma once
#include "stdafx.h"
#include "BaseMapper.h"

// UNL-KS106C (Mapper 352)
// - Kaiser 4-in-1(Unl,KS106C)[p1] - B-Wings, Kung Fu, 1942, SMB1 (wrong mirroring)

class UnlKs106C : public BaseMapper
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

		SetMirroringType((_game & 0x01) ? MirroringType::Vertical : MirroringType::Horizontal);
	}
};
