#ifndef COCAINE_DRIVERS_ABSTRACT_HPP
#define COCAINE_DRIVERS_ABSTRACT_HPP

#include "cocaine/common.hpp"
#include "cocaine/engine.hpp"

namespace cocaine { namespace engine {

class driver_t:
    public boost::noncopyable
{
    public:
        driver_t(engine_t* engine, const std::string& method):
            m_engine(engine),
            m_method(method)
        {
            syslog(LOG_DEBUG, "driver [%s:%s]: constructing", 
                m_engine->name().c_str(), m_method.c_str());
        }
        
        virtual ~driver_t() {
            syslog(LOG_DEBUG, "driver [%s:%s]: destructing",
                m_engine->name().c_str(), m_method.c_str());
        }

        virtual void suspend() = 0;
        virtual void resume() = 0;

        virtual Json::Value info() const = 0;

    protected:
        engine_t* m_engine;
        const std::string m_method;
};

}}

#endif
