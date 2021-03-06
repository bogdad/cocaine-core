/*
    Copyright (c) 2011-2012 Andrey Sibiryov <me@kobology.ru>
    Copyright (c) 2011-2012 Other contributors as noted in the AUTHORS file.

    This file is part of Cocaine.

    Cocaine is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Cocaine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef COCAINE_AUTH_HPP
#define COCAINE_AUTH_HPP

#include <openssl/evp.h>

#include "cocaine/common.hpp"

#include "cocaine/helpers/blob.hpp"

namespace cocaine { namespace crypto {

class auth_t:
    public boost::noncopyable
{
    public:
        auth_t(context_t& context);
        ~auth_t();

        void verify(const blob_t& message,
                    const blob_t& signature,
                    const std::string& username) const;

        // std::string sign(const std::string& message, 
        //                  const std::string& username) const;

    private:
        boost::shared_ptr<logging::logger_t> m_log;

        EVP_MD_CTX * m_context;

        typedef std::map<const std::string, EVP_PKEY*> key_map_t;
        key_map_t m_keys;
};

}}

#endif
