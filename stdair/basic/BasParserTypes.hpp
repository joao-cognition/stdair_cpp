#ifndef STDAIR_BAS_BASCOMPARSERTYPES_HPP
#define STDAIR_BAS_BASCOMPARSERTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost Spirit (Parsing)
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
// STDAIR
#include <stdair/basic/BasParserHelperTypes.hpp>

namespace stdair {

  // ////////////////////////////////////////////////////////////////////
  //
  //  Definition of Basic Types
  //
  // ////////////////////////////////////////////////////////////////////
  // The types of iterator, scanner and rule are then derived from
  // the parsing unit.
  using base_iterator_t = std::istreambuf_iterator<char>;
  using iterator_t = boost::spirit::multi_pass<base_iterator_t>;

  // ////////////////////////////////////////////////////////////////////
  //
  //  Parser related types
  //
  // ////////////////////////////////////////////////////////////////////
  /** 1-digit-integer parser */
  using int1_p_t = boost::spirit::qi::int_parser<unsigned int, 10, 1, 1>;
    
  /** 2-digit-integer parser */
  using uint2_p_t = boost::spirit::qi::uint_parser<int, 10, 2, 2>;
    
  /** 4-digit-integer parser */
  using uint4_p_t = boost::spirit::qi::uint_parser<int, 10, 4, 4>;

  /** Up-to-4-digit-integer parser */
  using uint1_4_p_t = boost::spirit::qi::uint_parser<int, 10, 1, 4>;

  /** Date & time element parsers. */
  using hour_p_t = boost::spirit::qi::uint_parser<hour_t, 10, 2, 2>;
  using minute_p_t = boost::spirit::qi::uint_parser<minute_t, 10, 2, 2>;
  using second_p_t = boost::spirit::qi::uint_parser<second_t, 10, 2, 2>;
  using year_p_t = boost::spirit::qi::uint_parser<year_t, 10, 4, 4>;
  using month_p_t = boost::spirit::qi::uint_parser<month_t, 10, 2, 2>;
  using day_p_t = boost::spirit::qi::uint_parser<day_t, 10, 2, 2>;
}
#endif // STDAIR_BAS_BASCOMPARSERTYPES_HPP
