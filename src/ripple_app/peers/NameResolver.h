//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2012, 2013 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef RIPPLE_PEER_NAMERESOLVER_H_INCLUDED
#define RIPPLE_PEER_NAMERESOLVER_H_INCLUDED

#include <functional>

namespace ripple {

class NameResolver
{
public:
    typedef std::function <
        void (std::string, std::vector<IPAddress>, boost::system::error_code)>
            HandlerType;

    static NameResolver* New (boost::asio::io_service& io_service,
        Journal journal);

    virtual ~NameResolver () = 0;

    /** Initiate an asynchronous cancellation. */
    virtual void stop_async () = 0;

    /** Cancel all pending resolutions.
        This call blocks until all pending work items are canceled. It is
        guaranteed that no handlers will be called after this function
        returns.
        You *must* call this function before the object is destroyed.
    */
    virtual void stop () = 0;

    /** resolve all hostnames on the list 
        @param names the names to be resolved
        @param handler the handler to call

        @note the handler may be called multiple times for a single name
              since a name may resolve to multiple IPs.
    */
    /** @{ */
    template <class Handler>
    void resolve (std::vector <std::string> const& names, Handler handler)
    {
        resolve (names, HandlerType (handler));
    }

    virtual void resolve (
        std::vector <std::string> const& names,
        HandlerType const& handler) = 0;
    /** @} */
};

};

#endif