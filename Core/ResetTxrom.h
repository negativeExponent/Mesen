#pragma once
#include "stdafx.h"
#include "MMC3.h"

class ResetTxrom : public MMC3
{
private:
	uint8_t _resetCounter;

protected:
	void Reset(bool softReset) override
	{
		MMC3::Reset(softReset);
		if(softReset) {
			_resetCounter = (_resetCounter + 1) & 0x03;
			UpdateState();
		} else {
			_resetCounter = 0;
		}
	}

	void StreamState(bool saving) override
	{
		MMC3::StreamState(saving);
		Stream(_resetCounter);
	}

	void SelectCHRPage(uint16_t slot, uint16_t page, ChrMemoryType memoryType) override
	{
		switch (_romInfo.SubMapperID) {
			default:
			case 0: page = (_resetCounter << 7) | (page & 0x7F); break;
			case 1: page = (_resetCounter << 7) | (page & 0x7F); break;
			case 2: page = (_resetCounter << 8) | (page & 0xFF); break;
			case 3: page = (_resetCounter << 8) | (page & 0xFF); break;
			case 4: page = (_resetCounter << 7) | (page & 0x7F); break;
		}

		MMC3::SelectCHRPage(slot, page, memoryType);
	}

	void SelectPRGPage(uint16_t slot, uint16_t page, PrgMemoryType memoryType) override
	{
		switch (_romInfo.SubMapperID) {
			default:
			case 0: page = (_resetCounter << 4) | (page & 0x0F); break;
			case 1: page = (_resetCounter << 5) | (page & 0x1F); break;
			case 2: page = (_resetCounter << 4) | (page & 0x0F); break;
			case 3: page = (_resetCounter << 5) | (page & 0x1F); break;
			case 4:
				if (_resetCounter == 0) {
					page &= 0x1F;
				} else {
					page &= 0x0F;
				}
				page |= (_resetCounter + 1) << 4;
				break;
		}

		MMC3::SelectPRGPage(slot, page, memoryType);
	}
};