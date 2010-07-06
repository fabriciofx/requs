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
 * @version $Id$
 */

#include <boost/test/unit_test.hpp>
#include <string>
#include "Proxy/Signature.h"

BOOST_AUTO_TEST_SUITE(SignatureTest)

BOOST_AUTO_TEST_CASE(testSignatureCanBeCreatedEmpty) {
    proxy::Signature sig;
    BOOST_CHECK_EQUAL("", sig.text());
}

BOOST_AUTO_TEST_CASE(testSignatureCanBeCreatedWithText) {
    const std::string s = "{alpha} works with {beta}";
    proxy::Signature sig = proxy::Signature(s);
    BOOST_CHECK_EQUAL(s, sig.text());
}

BOOST_AUTO_TEST_CASE(testSignatureTextCanBeChanged) {
    proxy::Signature sig; // signature text is empty
    const std::string s = "{alpha} sends email to {beta} with pleasure";
    sig.text(s);
    BOOST_CHECK_EQUAL(s, sig.text());
}

BOOST_AUTO_TEST_CASE(testPlaceCanBeExplained) {
    proxy::Signature sig;
    sig.text("{alpha} asks {beta}");
    sig.explain("alpha", new Signature::Explanation())
    sig.explain("beta", new Signature::Explanation("document", "User"))
    BOOST_CHECK_EQUAL(s, sig.text());
}

BOOST_AUTO_TEST_SUITE_END()
