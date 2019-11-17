#include "feed_handler.h"
#include "order_book.h"
#include <sstream>
#include "error_monitor.h"

using namespace std;

namespace CS {
    FeedHandler::FeedHandler() {

    }

    bool FeedHandler::ParseOrder(char *tokenMsg, Order &order, Trade &trd) {
        // order id
        ParseResult result = ParseTokenAsUInt64(tokenMsg, order.orderId);
        if (result != ParseResult ::Good){
            return false;
        }

        // side
        result = ParseSide(tokenMsg, order.side);
        if (result != ParseResult ::Good) {
            return false;
        }

        // size
        result = ParseTokenAsUInt(tokenMsg, order.size);
	trd.tradeSize=order.size;
        if (result != ParseResult::Good) {
            return false;
        }

        if (order.size > MaxTradeSize) {
            result = ParseResult ::InvalidMsgData;
            return false;
        }

        // price
        double price;
        result = ParsePrice(tokenMsg, price);
        if (result != ParseResult ::Good) {
            return false;
        }
        order.price = price * 100;
	trd.tradePrice=order.price;

        if (order.price > MaxTradePrice) {
            result = ParseResult ::InvalidMsgData;
            return false;
        }
        order.status = OrderStatus::Normal;
        return true;
    }

    void FeedHandler::ProcessMessage(const std::string &line) {
        static Order order;
        static Trade trd;

        char *buf = const_cast<char*>(line.c_str());
        MessageType mt = GetMessageType(buf);

        if (mt == MessageType::order) {
            if (ParseOrder(buf, order,trd)) {
		engine_.HandleOrder(order);
                engine_.HandleTrade(trd);
            } 
        } else if(mt == MessageType::cancel) {
		ParseResult result = ParseTokenAsUInt64(buf, order.orderId);
		engine_.Cancel(order);
	
	}
	else if ( mt == MessageType::amend){
		ParseResult result = ParseTokenAsUInt64(buf, order.orderId);
		result = ParseTokenAsUInt(buf, order.size);
        	trd.tradeSize=order.size;
		engine_.Amend(order);


        }

    }


    void FeedHandler::PrintCurrentOrderBook(std::ostream &os) const {
        engine_.PrintOrderBook(os);
    }
}
