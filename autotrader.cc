// Copyright 2021 Optiver Asia Pacific Pty. Ltd.
//
// This file is part of Ready Trader Go.
//
//     Ready Trader Go is free software: you can redistribute it and/or
//     modify it under the terms of the GNU Affero General Public License
//     as published by the Free Software Foundation, either version 3 of
//     the License, or (at your option) any later version.
//
//     Ready Trader Go is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
//
//     You should have received a copy of the GNU Affero General Public
//     License along with Ready Trader Go.  If not, see
//     <https://www.gnu.org/licenses/>.
#include <array>

#include <boost/asio/io_context.hpp>

#include <ready_trader_go/logging.h>

#include "autotrader.h"

using namespace ReadyTraderGo;

RTG_INLINE_GLOBAL_LOGGER_WITH_CHANNEL(LG_AT, "AUTO")

constexpr int LOT_SIZE = 10;
constexpr int POSITION_LIMIT = 100;
constexpr int TICK_SIZE_IN_CENTS = 100;
constexpr int MIN_BID_NEARST_TICK = (MINIMUM_BID + TICK_SIZE_IN_CENTS) / TICK_SIZE_IN_CENTS * TICK_SIZE_IN_CENTS;
constexpr int MAX_ASK_NEAREST_TICK = MAXIMUM_ASK / TICK_SIZE_IN_CENTS * TICK_SIZE_IN_CENTS;

<<<<<<< HEAD
constexpr int MIN_VALID_FUT_ORDER_VOLUME = 100;

constexpr Instrument FUT = Instrument.FUTURE;
constexpr Instrument ETF = Instrument.ETF;
constexpr double TAKER_FEE = 0.0002;
constexpr double MAKER_FEE = -0.0001;

=======
>>>>>>> 1a6fbbb2ed785b1c6b28cf9d41e1c0503ffe462e
AutoTrader::AutoTrader(boost::asio::io_context& context) : BaseAutoTrader(context)
{
}

void AutoTrader::DisconnectHandler()
{
    BaseAutoTrader::DisconnectHandler();
    RLOG(LG_AT, LogLevel::LL_INFO) << "execution connection lost";
}

void AutoTrader::ErrorMessageHandler(unsigned long clientOrderId,
                                     const std::string& errorMessage)
{
    RLOG(LG_AT, LogLevel::LL_INFO) << "error with order " << clientOrderId << ": " << errorMessage;
    if (clientOrderId != 0 && ((mAsks.count(clientOrderId) == 1) || (mBids.count(clientOrderId) == 1)))
    {
        OrderStatusMessageHandler(clientOrderId, 0, 0, 0);
    }
}

void AutoTrader::HedgeFilledMessageHandler(unsigned long clientOrderId,
                                           unsigned long price,
                                           unsigned long volume)
{
    RLOG(LG_AT, LogLevel::LL_INFO) << "hedge order " << clientOrderId << " filled for " << volume
                                   << " lots at $" << price << " average price in cents";
}

void AutoTrader::OrderBookMessageHandler(Instrument instrument,
                                         unsigned long sequenceNumber,
                                         const std::array<unsigned long, TOP_LEVEL_COUNT>& askPrices,
                                         const std::array<unsigned long, TOP_LEVEL_COUNT>& askVolumes,
                                         const std::array<unsigned long, TOP_LEVEL_COUNT>& bidPrices,
                                         const std::array<unsigned long, TOP_LEVEL_COUNT>& bidVolumes)
{
<<<<<<< HEAD
    if (instrument == FUT) {
        // Get out of bad orders right away
        // Cancel an arbitragable bid order
        if (askPrices[0] < mBidPrice) {
            SendCancelOrder(mBidId);
            mBidId = 0;
        }
        // Cancel an arbitragable ask order
        if (bidPrices[0] > mAskPrices) {
            SendCancelOrder(mAskId);
            mAskId = 0;
        }

        // Proper market making code
        //self.future_mid_price = int(round((bid_prices[0] + ask_prices[0]) / 2, -2))
        //self.logger.info(f"Future trading at {bid_prices[0]}:{ask_prices[0]} with mid price {self.future_mid_price}")
        
        constexpr unsigned long additional_spread = 1 * TICK_SIZE_IN_CENTS;
        unsigned long priceAdjustment = -((int)round(mPosition / (3.0 * LOT_SIZE))) * TICK_SIZE_IN_CENTS;
        
        unsigned long newBidPrice = 0, newAskPrice = 0;
        if (bidPrices[0] != 0) {
            newBidPrice = bidPrices[0] + priceAdjustment - additional_spread;
        }
        if (askPrices[0] != 0) {
            newAskPrice = askPrices[0] + priceAdjustment + additional_spread;
        }

        if (mBidId != 0 && newBidPrice != 0 && newBidPrice != mBidPrice) {
            SendCancelOrder(mBidId);
            mBidId = 0;
        }
        if (mAskId != 0 && newAskPrice != 0 && newAskPrice != mAskPrice) {
            SendCancelOrder(mAskId);
            mAskId = 0;
        }

        if (mBidId == 0 && newBidPrice != 0) {
            int bidSize = POSITION_LIMIT - mPosition;
            if (bidSize > 0) {
                mBidId = mNextMessageId++;
                mBidPrice = newBidPrice;
                SendInsertOrder(mBidId, Side::BUY, newBidPrice, std::min(LOT_SIZE, bidSize), Lifespan::GOOD_FOR_DAY);
                mBids.emplace(mBidId);
            }
        }
        if (mAskId == 0 && newAskPrice != 0) {
            int askSize = mPosition - POSITION_LIMIT;
            if (askSize > 0) {
                mAskId = mNextMessageId++;
                mAskPrice = newAskPrice;
                SendInsertOrder(mAskId, Side::SELL, newAskPrice, std::min(LOT_SIZE, askSize), Lifespan::GOOD_FOR_DAY);
                mAsks.emplace(mAskId);
            }
        }
    }

=======
>>>>>>> 1a6fbbb2ed785b1c6b28cf9d41e1c0503ffe462e
    RLOG(LG_AT, LogLevel::LL_INFO) << "order book received for " << instrument << " instrument"
                                   << ": ask prices: " << askPrices[0]
                                   << "; ask volumes: " << askVolumes[0]
                                   << "; bid prices: " << bidPrices[0]
                                   << "; bid volumes: " << bidVolumes[0];
<<<<<<< HEAD
=======

    if (instrument == Instrument::FUTURE)
    {
        unsigned long priceAdjustment = - (mPosition / LOT_SIZE) * TICK_SIZE_IN_CENTS;
        unsigned long newAskPrice = (askPrices[0] != 0) ? askPrices[0] + priceAdjustment : 0;
        unsigned long newBidPrice = (bidPrices[0] != 0) ? bidPrices[0] + priceAdjustment : 0;

        if (mAskId != 0 && newAskPrice != 0 && newAskPrice != mAskPrice)
        {
            SendCancelOrder(mAskId);
            mAskId = 0;
        }
        if (mBidId != 0 && newBidPrice != 0 && newBidPrice != mBidPrice)
        {
            SendCancelOrder(mBidId);
            mBidId = 0;
        }

        if (mAskId == 0 && newAskPrice != 0 && mPosition > -POSITION_LIMIT)
        {
            mAskId = mNextMessageId++;
            mAskPrice = newAskPrice;
            SendInsertOrder(mAskId, Side::SELL, newAskPrice, LOT_SIZE, Lifespan::GOOD_FOR_DAY);
            mAsks.emplace(mAskId);
        }
        if (mBidId == 0 && newBidPrice != 0 && mPosition < POSITION_LIMIT)
        {
            mBidId = mNextMessageId++;
            mBidPrice = newBidPrice;
            SendInsertOrder(mBidId, Side::BUY, newBidPrice, LOT_SIZE, Lifespan::GOOD_FOR_DAY);
            mBids.emplace(mBidId);
        }
    }
>>>>>>> 1a6fbbb2ed785b1c6b28cf9d41e1c0503ffe462e
}

void AutoTrader::OrderFilledMessageHandler(unsigned long clientOrderId,
                                           unsigned long price,
                                           unsigned long volume)
{
    RLOG(LG_AT, LogLevel::LL_INFO) << "order " << clientOrderId << " filled for " << volume
                                   << " lots at $" << price << " cents";
    if (mAsks.count(clientOrderId) == 1)
    {
        mPosition -= (long)volume;
        SendHedgeOrder(mNextMessageId++, Side::BUY, MAX_ASK_NEAREST_TICK, volume);
    }
    else if (mBids.count(clientOrderId) == 1)
    {
        mPosition += (long)volume;
        SendHedgeOrder(mNextMessageId++, Side::SELL, MIN_BID_NEARST_TICK, volume);
    }
}

void AutoTrader::OrderStatusMessageHandler(unsigned long clientOrderId,
                                           unsigned long fillVolume,
                                           unsigned long remainingVolume,
                                           signed long fees)
{
    if (remainingVolume == 0)
    {
        if (clientOrderId == mAskId)
        {
            mAskId = 0;
        }
        else if (clientOrderId == mBidId)
        {
            mBidId = 0;
        }

        mAsks.erase(clientOrderId);
        mBids.erase(clientOrderId);
    }
}

void AutoTrader::TradeTicksMessageHandler(Instrument instrument,
                                          unsigned long sequenceNumber,
                                          const std::array<unsigned long, TOP_LEVEL_COUNT>& askPrices,
                                          const std::array<unsigned long, TOP_LEVEL_COUNT>& askVolumes,
                                          const std::array<unsigned long, TOP_LEVEL_COUNT>& bidPrices,
                                          const std::array<unsigned long, TOP_LEVEL_COUNT>& bidVolumes)
{
    RLOG(LG_AT, LogLevel::LL_INFO) << "trade ticks received for " << instrument << " instrument"
                                   << ": ask prices: " << askPrices[0]
                                   << "; ask volumes: " << askVolumes[0]
                                   << "; bid prices: " << bidPrices[0]
                                   << "; bid volumes: " << bidVolumes[0];
}
