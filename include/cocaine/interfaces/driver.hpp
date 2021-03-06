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

#ifndef COCAINE_DRIVER_INTERFACE_HPP
#define COCAINE_DRIVER_INTERFACE_HPP

#include <boost/tuple/tuple.hpp>

#include "cocaine/common.hpp"
#include "cocaine/repository.hpp"

#include "cocaine/helpers/json.hpp"

namespace cocaine { namespace engine { namespace drivers {

class driver_t:
    public boost::noncopyable
{
    public:
        virtual ~driver_t() {
            // Empty.
        }

        virtual Json::Value info() const = 0;

    public:
        engine_t& engine() {
            return m_engine;
        }

    protected:
        driver_t(context_t& context, engine_t& engine, const std::string& , const Json::Value& ):
            m_context(context),
            m_engine(engine)
        { }
        
    private:
        context_t& m_context;
        engine_t& m_engine;
};

}}

template<>
struct category_traits<engine::drivers::driver_t> {
    typedef std::auto_ptr<engine::drivers::driver_t> ptr_type;

    typedef boost::tuple<
        engine::engine_t&,
        const std::string&,
        const Json::Value&
    > args_type;

    template<class T>
    struct default_factory:
        public factory<engine::drivers::driver_t>
    {
        virtual ptr_type get(context_t& context,
                             const args_type& args)
        {
            return ptr_type(
                new T(
                    context,
                    boost::get<0>(args),
                    boost::get<1>(args),
                    boost::get<2>(args)
                )
            );
        }
    };
};

}

#endif
