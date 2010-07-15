/**
 * RQDQL.com, Requirements Definition and Query Language
 *
 * Redistribution and use in source and binary forms, with or 
 * without modification, are PROHIBITED without prior written 
 * permission from the author. This product may NOT be used 
 * anywhere and on any computer except the server platform of 
 * rqdql.com. located at www.rqdql.com. If you received this 
 * code occasionally and without intent to use it, please report 
 * this incident to the author by email: team@rqdql.com
 *
 * @author Yegor Bugayenko <egor@tpc2.com>
 * @copyright Copyright (c) rqdql.com, 2010
 * @version $Id: DataTest.cpp 2314 2010-07-15 09:21:38Z yegor256@yahoo.com $
 */

#include <boost/test/unit_test.hpp>
#include <string>
#include "Solm/Term.h"
using namespace solm;
using std::string;

BOOST_AUTO_TEST_SUITE(TermTest)

BOOST_AUTO_TEST_CASE(testDifferentFormatsAreOK) {
    Term t("123.");

    // simple objects and atoms
    t = Term("mary.");
    BOOST_CHECK(t.is(Term::T_ATOM) && !t.is(Term::T_NUMBER));
    t = Term("45.890.");
    BOOST_CHECK(t.is(Term::T_NUMBER));
    t = Term("'this is text'.");
    BOOST_CHECK(t.is(Term::T_TEXT));
    
    // simple facts
    t = Term("father_of(john, mary).");
    BOOST_CHECK(t.is(Term::T_FACT));
    t = Term("age_of(smith, 45).");
    BOOST_CHECK(t.is(Term::T_FACT));
    t = Term("child_of(emily, parents(alex, alice)).");
    BOOST_CHECK(t.is(Term::T_FACT));
    t = Term("mistique_character(king, 'Persia'), king('Arthur').");
    BOOST_CHECK(t.is(Term::T_FACT));
    t = Term("in_class(john, mary_smith, peter, alex_Wilson_3rd).");
    BOOST_CHECK(t.is(Term::T_FACT));
    
    // specific formatting
    t = Term("child(X, Y) :- parent(Y, X)."); // rule definition
    BOOST_CHECK(!t.is(Term::T_FACT));
    t = Term("P < 67."); // number of P is less than 67
    BOOST_CHECK(!t.is(Term::T_FACT));
    t = Term("P is 67."); // P should be equal to 67
    BOOST_CHECK(!t.is(Term::T_FACT));
    t = Term("X = Y."); // X is the same as Y
    BOOST_CHECK(!t.is(Term::T_FACT));
    t = Term("X =:= Y."); // X and Y stand for the same number/value
    BOOST_CHECK(!t.is(Term::T_FACT));
    BOOST_LOG_MESSAGE((std::string)t);
    t = Term("X =\\= Y."); // X and Y stand for the different values/numbers
    BOOST_CHECK(!t.is(Term::T_FACT));
    
    // simple questions
    t = Term("parent(X, mary)."); // who is a parent of mary?
    BOOST_CHECK(!t.is(Term::T_FACT));
    t = Term("price(X), X =\\= 'Arthur'."); // who is a prince and is not "Arthur"?
    BOOST_CHECK(!t.is(Term::T_FACT));
}

BOOST_AUTO_TEST_SUITE_END()
