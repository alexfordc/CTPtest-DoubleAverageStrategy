#pragma once
// ---- �򵥲��Խ��׵��� ---- //

#include <functional>
#include "CTP_API/ThostFtdcUserApiStruct.h"
#include "TickToKlineHelper.h"
#include "CustomTradeSpi.h"

class DoubleAverageStrategy
{
public:

void StrategyCheckAndTrade(TThostFtdcInstrumentIDType instrumentID, CustomTradeSpi *customTradeSpi);
};
