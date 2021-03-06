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

#ifndef COCAINE_SANDBOX_INTERFACE_HPP
#define COCAINE_SANDBOX_INTERFACE_HPP

#include <boost/tuple/tuple.hpp>

#include "cocaine/common.hpp"
#include "cocaine/repository.hpp"

#include "cocaine/helpers/blob.hpp"

namespace cocaine { namespace engine {

// Sandbox I/O
// -----------

class io_t {
    public:
        virtual blob_t read(int timeout) = 0;

        virtual void write(const void * data,
                           size_t size) = 0;
};

// Sandbox interface
// -----------------

class sandbox_t:
    public boost::noncopyable
{
    public:
        virtual ~sandbox_t() {
            // Empty.
        }
        
        virtual void invoke(const std::string& event,
                            io_t& io) = 0;

    protected:
        sandbox_t(context_t& context, const manifest_t&):
            m_context(context)
        { }

    private:
        context_t& m_context;
};

}

template<>
struct category_traits<engine::sandbox_t> {
    typedef std::auto_ptr<engine::sandbox_t> ptr_type;
    typedef boost::tuple<const manifest_t&> args_type;

    template<class T>
    struct default_factory:
        public factory<engine::sandbox_t>
    {
        virtual ptr_type get(context_t& context,
                             const args_type& args)
        {
            return ptr_type(
                new T(
                    context,
                    boost::get<0>(args)
                )
            );
        }
    };
};

}

#endif
