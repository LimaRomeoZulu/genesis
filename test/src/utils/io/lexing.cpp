/*
    Genesis - A toolkit for working with phylogenetic data.
    Copyright (C) 2014-2016 Lucas Czech

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact:
    Lucas Czech <lucas.czech@h-its.org>
    Exelixis Lab, Heidelberg Institute for Theoretical Studies
    Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany
*/

/**
 * @brief
 *
 * @file
 * @ingroup test
 */

#include "common.hpp"

#include "lib/utils/core/std.hpp"
#include "lib/utils/io/parser.hpp"
#include "lib/utils/io/scanner.hpp"
#include "lib/utils/io/input_stream.hpp"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <sstream>

using namespace genesis::utils;

// -------------------------------------------------------------------------
//     Unsigned Int
// -------------------------------------------------------------------------

void test_uint ( std::string str, unsigned int val, size_t col )
{
    std::istringstream iss ( str );
    InputStream iit( make_unique< StreamInputSource >( iss ));

    auto res = parse_unsigned_integer<unsigned int>( iit );
    EXPECT_EQ( val, res ) << "Input string: '" << str << "'";
    EXPECT_EQ( iit.column(), col ) << "Input string: '" << str << "'";
}

TEST(Parser, UnsignedInteger)
{
    SCOPED_TRACE("Parser.UnsignedInteger");

    test_uint( "",   0, 0 );
    test_uint( " ",  0, 1 );
    test_uint( "x",  0, 1 );
    test_uint( "-",  0, 1 );
    test_uint( "+0", 0, 1 );
    test_uint( "0",  0, 2 );

    test_uint( "1",         1, 2 );
    test_uint( "12345", 12345, 6 );

    test_uint( "123 45", 123, 4 );
    test_uint( "56ab",    56, 3 );

    EXPECT_THROW( test_uint( "123456789101121314151617181920", 0, 0 ), std::overflow_error);
}

// -------------------------------------------------------------------------
//     Signed Int
// -------------------------------------------------------------------------

void test_int ( std::string str, int val, size_t col )
{
    std::istringstream iss ( str );
    InputStream iit( make_unique< StreamInputSource >( iss ));

    auto res = parse_signed_integer<int>( iit );
    EXPECT_EQ( val, res ) << "Input string: '" << str << "'";
    EXPECT_EQ( iit.column(), col ) << "Input string: '" << str << "'";
}

TEST(Parser, SignedInteger)
{
    SCOPED_TRACE("Parser.SignedInteger");

    test_int( "",   0, 0 );
    test_int( " ",  0, 1 );
    test_int( "x",  0, 1 );
    test_int( "-",  0, 2 );
    test_int( "+",  0, 2 );
    test_int( "+0", 0, 3 );
    test_int( "0",  0, 2 );

    test_int( "1",           1, 2 );
    test_int( "12345",   12345, 6 );
    test_int( "+1",          1, 3 );
    test_int( "+12345",  12345, 7 );
    test_int( "-1",         -1, 3 );
    test_int( "-12345", -12345, 7 );

    test_int( "123 45",   123, 4 );
    test_int( "56ab",      56, 3 );
    test_int( "+123 45",  123, 5 );
    test_int( "+56ab",     56, 4 );
    test_int( "-123 45", -123, 5 );
    test_int( "-56ab",    -56, 4 );

    EXPECT_THROW( test_int( "123456789101121314151617181920",  0, 0 ), std::overflow_error);
    EXPECT_THROW( test_int( "-123456789101121314151617181920", 0, 0 ), std::underflow_error);
}

// -------------------------------------------------------------------------
//     Float
// -------------------------------------------------------------------------

void test_float( std::string str, float val, size_t col )
{
    std::istringstream iss ( str );
    InputStream iit( make_unique< StreamInputSource >( iss ));

    auto res = parse_float<double>( iit );
    EXPECT_FLOAT_EQ( val, res ) << "Input string: '" << str << "'";
    EXPECT_EQ( iit.column(), col ) << "Input string: '" << str << "'";
}

TEST(Parser, Float)
{
    SCOPED_TRACE("Parser.Float");

    test_float( "",   0.0, 0);
    test_float( " ",  0.0, 1);
    test_float( "x",  0.0, 1 );
    test_float( "-",  0.0, 2 );
    test_float( "+",  0.0, 2 );
    test_float( "+0", 0.0, 3 );
    test_float( "0",  0.0, 2 );

    test_float( "1",           1.0, 2 );
    test_float( "12345",   12345.0, 6 );
    test_float( "+1",          1.0, 3 );
    test_float( "+12345",  12345.0, 7 );
    test_float( "-1",         -1.0, 3 );
    test_float( "-12345", -12345.0, 7 );

    test_float( "123 45",   123.0, 4 );
    test_float( "56ab",      56.0, 3 );
    test_float( "+123 45",  123.0, 5 );
    test_float( "+56ab",     56.0, 4 );
    test_float( "-123 45", -123.0, 5 );
    test_float( "-56ab",    -56.0, 4 );

    test_float( "123.45",    123.45, 7 );
    test_float( "123,45",    123.45, 7 );
    test_float( "-123.45",   -123.45, 8 );
    test_float( "-123,45",   -123.45, 8 );

    test_float( "123.45x",    123.45, 7 );
    test_float( "123,45x",    123.45, 7 );
    test_float( "-123.45x",   -123.45, 8 );
    test_float( "-123,45x",   -123.45, 8 );

    test_float( "123.45e",    123.45, 8 );
    test_float( "123,45E",    123.45, 8 );
    test_float( "-123.45E",   -123.45, 9 );
    test_float( "-123,45e",   -123.45, 9 );

    test_float( "123.456e2",    12345.6, 10 );
    test_float( "123,456E2",    12345.6, 10 );
    test_float( "-123.456E2",   -12345.6, 11 );
    test_float( "-123,456e2",   -12345.6, 11 );

    test_float( "123.456e-2",    1.23456, 11 );
    test_float( "123,456E-2",    1.23456, 11 );
    test_float( "-123.456E-2",   -1.23456, 12 );
    test_float( "-123,456e-2",   -1.23456, 12 );

    test_float( "123.456e-x2",    123.456, 10 );
    test_float( "123,456E-x2",    123.456, 10 );
    test_float( "-123.456E-x2",   -123.456, 11 );
    test_float( "-123,456e-x2",   -123.456, 11 );

    EXPECT_THROW( test_float( "1.0e123456789101121314151617181920",  0, 0 ), std::overflow_error);
    EXPECT_THROW( test_float( "1.0e-123456789101121314151617181920", 0, 0 ), std::underflow_error);
}

// -------------------------------------------------------------------------
//     String
// -------------------------------------------------------------------------

void test_string(
    std::string str,
    std::string val,
    size_t col,
    bool use_escapes,
    bool use_twin_quotes,
    bool include_qmarks
) {
    std::istringstream iss ( str );
    InputStream iit( make_unique< StreamInputSource >( iss ));

    auto res = parse_quoted_string( iit, use_escapes, use_twin_quotes, include_qmarks );
    EXPECT_EQ( val, res )
        << "Input string: '" << str << "', "
        << "use_escapes: "   << (use_escapes ? "true" : "false") << ", "
        << "include_qmarks:" << (include_qmarks ? "true" : "false");

    EXPECT_EQ( iit.column(), col )
        << "Input string: '"  << str << "', "
        << "use_escapes: "    << (use_escapes ? "true" : "false") << ", "
        << "include_qmarks: " << (include_qmarks ? "true" : "false");
}


TEST(Parser, String)
{
    SCOPED_TRACE("Parser.String");

    // empty string
    test_string( "", "", 0, true, true, true);
    test_string( "", "", 0, true, false, true);
    test_string( "", "", 0, false, true, true);
    test_string( "", "", 0, false, false, true);

    // Escape

    // no closing qmark
    EXPECT_THROW( test_string( "\"",    "", 1, true, false, true), std::runtime_error);
    EXPECT_THROW( test_string( "\"xyz", "", 1, true, false, true), std::runtime_error);
    EXPECT_THROW( test_string( "xyz",   "", 1, true, false, true), std::runtime_error);

    // end after escape sequence
    EXPECT_THROW( test_string( "\"bla\\", "", 1, true, false, true), std::runtime_error);

    // no escape sequences
    test_string( "\"bla\"", "bla",     6, true, false,  false );
    test_string( "\"bla\"", "\"bla\"", 6, true, false,  true  );
    test_string( "\"bla\"", "bla",     6, false, false, false );
    test_string( "\"bla\"", "\"bla\"", 6, false, false, true  );

    // other qmarks
    test_string( "-bla-", "bla",   6, true, false,  false );
    test_string( ".bla.", ".bla.", 6, true, false,  true  );
    test_string( "\nbla\n", "bla", 1, false, false, false );
    test_string( "xblax", "xblax", 6, false, false, true  );

    // with escape sequences
    test_string( "'bl\\\\a'", "bl\\a",     8, true, false,  false );
    test_string( "'bl\\\\a'", "'bl\\a'",   8, true, false,  true  );
    test_string( "'bl\\\\a'", "bl\\\\a",   8, false, false, false );
    test_string( "'bl\\\\a'", "'bl\\\\a'", 8, false, false, true  );

    // other escape sequences
    test_string( "'bl\\ra'", "bl\ra",     8, true, false,  false );
    test_string( "'bl\\na'", "bl\na",     8, true, false,  false );
    test_string( "'bl\\ta'", "bl\ta",     8, true, false,  false );
    test_string( "'bl\\:a'", "bl:a",      8, true, false,  false );
    test_string( "'bl\\\"a'", "bl\"a",    8, true, false,  false );

    // Double quotes

    // no closing qmark
    EXPECT_THROW( test_string( "\"",     "", 1, false, true, true), std::runtime_error);
    EXPECT_THROW( test_string( "\"xyz",  "", 1, false, true, true), std::runtime_error);
    EXPECT_THROW( test_string( "xyz",    "", 1, false, true, true), std::runtime_error);
    EXPECT_THROW( test_string( "'xy''z", "", 1, false, true, true), std::runtime_error);

    // end after escape sequence
    EXPECT_THROW( test_string( "\"bla\\", "", 1, false, true, true), std::runtime_error);

    // no escape sequences
    test_string( "\"bla\"", "bla",     6, false, true,  false );
    test_string( "\"bla\"", "\"bla\"", 6, false, true,  true  );

    // with double qmarks
    test_string( "''", "",              3, false, true, false );
    test_string( "''''", "'",           5, false, true, false );
    test_string( "''''''", "''",        7, false, true, false );
    test_string( "'bla'", "bla",        6, false, true, false );
    test_string( "'bl''a'", "bl'a",     8, false, true, false );
    test_string( "'''bla'", "'bla",     8, false, true, false );
    test_string( "'bla'''", "bla'",     8, false, true, false );
    test_string( "'a''''b'", "a''b",    9, false, true, false );
}
