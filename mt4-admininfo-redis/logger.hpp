#pragma once

#include <boost/log/core.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#define loginfo BOOST_LOG_TRIVIAL(info)
#define logerror BOOST_LOG_TRIVIAL(error)
#define logdebug BOOST_LOG_TRIVIAL(debug)

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

//using namespace boost::log::trivial;