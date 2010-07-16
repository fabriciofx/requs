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
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include "Solm/Term.h"
using namespace solm;
using std::string;

void testTerm(const string& t, const string& image = "") {
    Term term = Term(t);
    
    if (!image.empty()) {
        BOOST_CHECK_EQUAL(image, (string)term);
    }
    
    std::vector<string> flags;
    if (term.is(Term::T_ATOM)) {
        flags.push_back("A");
    }
    if (term.is(Term::T_TEXT)) {
        flags.push_back("T");
    }
    if (term.is(Term::T_NUMBER)) {
        flags.push_back("N");
    }
    if (term.is(Term::T_RULE)) {
        flags.push_back("R");
    }
    if (term.is(Term::T_FACT)) {
        flags.push_back("F");
    }
    if (term.is(Term::T_VARIABLE)) {
        flags.push_back("V");
    }
    if (term.is(Term::T_OBJECT)) {
        flags.push_back("O");
    }
    BOOST_TEST_MESSAGE(
        t + " --> " + (std::string)term 
        + " [" + boost::algorithm::join(flags, ",") + "]"
    );
}

BOOST_AUTO_TEST_SUITE(TermTest)

BOOST_AUTO_TEST_CASE(testEmptyTermWorksFine) {
    Term t;
    BOOST_REQUIRE((bool)t); // it's TRUE by default
}

BOOST_AUTO_TEST_CASE(testInvalidFormatsRaiseExceptions) {
    BOOST_CHECK_THROW(testTerm("true"), std::exception); // DOT missed
    BOOST_CHECK_THROW(testTerm("123true."), std::exception); // invalid name of the object
    BOOST_CHECK_THROW(testTerm("-Aksel."), std::exception); // invalid name

    // now let's make sure that we can successfuly parse terms
    // after the problems happened before
    testTerm("like(klaus, beer)."); 
}

BOOST_AUTO_TEST_CASE(testDifferentFormatsAreOK) {
    // simple objects and atoms
    testTerm("true.", "true");
    testTerm("mary.", "mary");
    testTerm("victor, peter, 44, 'works?'.", "victor, peter, 44, 'works?'");
    testTerm("45.890.");
    testTerm("'this is text'.");
    
    // simple facts
    testTerm("father_of(john, mary).");
    testTerm("age_of(smith, 45), true.", "age_of(smith, 45)");
    testTerm("child_of(emily, parents(alex, alice)).");
    testTerm("mistique_character(king, 'Persia'), king('Arthur').");
    testTerm("in_class(john, mary_smith, peter, alex_Wilson_3rd).");
    testTerm("a = 1, b = 2."/*, "a = 1, b = 2"*/);
    testTerm("a(b < 4, c =\\= 'test', mary(1, 2))."/*, "a(b < 4, c =\\= 'test', mary(1, 2))"*/);
    
    // specific formatting
    testTerm("child(X, Y) :- parent(Y, X)."); // rule definition
    testTerm("P < 67."); // number of P is less than 67
    testTerm("P is 67."); // P should be equal to 67
    testTerm("X = Y."); // X is the same as Y
    testTerm("X =:= Y.", "X =:= Y"); // X and Y stand for the same number/value
    testTerm("X =\\= Y."); // X and Y stand for the different values/numbers
    
    // simple questions
    testTerm("parent(X, mary).", "parent(X, mary)"); // who is a parent of mary?
    testTerm("prince(X), X =\\= 'Arthur'."); // who is a prince and is not "Arthur"?
}

BOOST_AUTO_TEST_CASE(testDoubleTrueIsIgnored) {
    // this trailing TRUE should be ignored
    Term t("king(john), true.");
    BOOST_CHECK_EQUAL("king(john)", (string)t);

    // leading and trailing true will be ignored
    t = Term("true, true, woman(alex), true.");
    BOOST_CHECK_EQUAL("woman(alex)", (string)t);
}

BOOST_AUTO_TEST_CASE(testWeCanResolveSimpleFactsAndRules) {
    // here we add simple prolog-style facts
    Term t("kid(john, mary).");
    
    // now we're asking for a list of X that satisfy this rule
    Term a = t / Term("kid(john, X).");
    
    BOOST_REQUIRE((bool)a); // the term should be positive
    // BOOST_REQUIRE_EQUAL("X = mary", (string)a);
}

BOOST_AUTO_TEST_CASE(testWeCanResolveMoreComplexRules) {
    // here we add simple prolog-style facts
    Term t("kid(john, mary), kid(john, peter).");
    
    // now we're asking for a list of X that satisfy this rule
    Term a = t / Term("kid(john, X).");
    
    BOOST_REQUIRE((bool)a); // the term should be positive
    // BOOST_REQUIRE_EQUAL("X = mary", (string)a);
}

BOOST_AUTO_TEST_SUITE_END()
