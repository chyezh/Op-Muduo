#ifndef _CYZPP_COMMON_H__
#define _CYZPP_COMMON_H__

#include <iostream>
#include <memory>

#define CYZPP_BEGIN namespace cyzpp {
#define CYZPP_END }


CYZPP_BEGIN

#define ERRLOG std::cerr

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;


CYZPP_END
 
#endif // !_CYZPP_COMMON_H__