#ifndef CS_ORDERBOOK_FEED_HANDLER_H
#define CS_ORDERBOOK_FEED_HANDLER_H

#include <string>
#include <string.h>
#include "engine.h"
#include "error_monitor.h"
using namespace std;
namespace CS {
    class FeedHandler
    {
    private:
        Engine engine_;

        enum class ParseResult {
            Good,
            CorruptMessage,
            InvalidMsgData
        };


        bool ParseOrder(char *tokenMsg, Order &order, Trade &trd);


        ParseResult ParseTokenAsUInt64(char *&tk_msg, uint64_t &dest) {
            tk_msg = strtok(NULL, " ");
            if (tk_msg == NULL) {
                return ParseResult::CorruptMessage;
            }
            else {
                char * pEnd = NULL;
                dest = std::strtoull(tk_msg, &pEnd, 10);
                if (*pEnd)
                    return ParseResult::InvalidMsgData;
            }
            return ParseResult ::Good;
        }


        ParseResult ParseTokenAsUInt(char *&tk_msg, uint32_t &dest) {
            tk_msg = strtok(NULL, " ");
            if (tk_msg == NULL) {
                return ParseResult::CorruptMessage;
            }
            else {
                char * pEnd = NULL;
                dest = strtol(tk_msg, &pEnd, 10);
                if (*pEnd)
                    return ParseResult::InvalidMsgData;
            }
            return ParseResult ::Good;
        }

        ParseResult ParseSide(char *&tk_msg, uint32_t &dest) {
            tk_msg = strtok(NULL, " ");
            if (tk_msg == NULL) {
                return ParseResult::CorruptMessage;
            }
            else {
                if ( ( strncmp(tk_msg,"sell",4)==0 ) || ( strncmp(tk_msg,"SELL",4)== 0 ) ) {
                    dest = SELL;
                } else if (( strncmp(tk_msg,"buy",3)==0 ) || ( strncmp(tk_msg,"BUY",3)== 0 )) {
                    dest = BUY;
                } else {
                    return ParseResult::InvalidMsgData;
                }
            }
            return ParseResult ::Good;
        }

        ParseResult ParsePrice(char *&tk_msg, double &dest) {
            tk_msg = strtok(NULL, " ");
            if (tk_msg == NULL) {
                return ParseResult::CorruptMessage;
            }
            else {
                char * pEnd = NULL;
                //dest = strtol(tk_msg, &pEnd, 10);
                dest = strtod (tk_msg, &pEnd);
                if (*pEnd)
                    return ParseResult::InvalidMsgData;
            }
            return ParseResult ::Good;
        }
        MessageType GetMessageType(char *tokenMsg) {
            uint32_t len = strlen(tokenMsg);
            //check length
            if (len == 0 || len > MaxMsgLength) {
                ErrorMonitor::GetInstance().CorruptMessage();
                return MessageType::Unknown;
            }
		
            tokenMsg = strtok(tokenMsg, " ");
	    if ( strncmp(tokenMsg,"order",5) ==0 ){
                    return MessageType::order;}
            else if(strncmp(tokenMsg,"cancel",6) ==0 ) 
                    return MessageType::cancel;
            else if( strncmp(tokenMsg,"amend",5) ==0 )        
		    return MessageType::amend;
	    else if(strncmp(tokenMsg,"query",5) ==0 ) 
                    return MessageType::query;
            else
		return MessageType::Unknown;
        }
    public:
        FeedHandler();
        void ProcessMessage(const std::string &line);
        void PrintCurrentOrderBook(std::ostream &os) const;

    };
}


#endif //CS_ORDERBOOK_FEED_HANDLER_H
