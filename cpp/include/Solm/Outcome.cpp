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
 *
 * This file is included ONLY from Solm.h
 */
 
#include <string>
#include <vector>
#include "Solm/Outcome.h"

solm::Outcome::Outcome(const Context& ctx) : std::vector<Fact>(), _context(ctx) { 
    /* that's it */ 
}

solm::Outcome solm::Outcome::operator+(const solm::Outcome& out) const {
    // this outcome is empty - just return the given one
    if (!size()) {
        return out;
    }
    Outcome n = *this;
    if (!n) {
        throw rqdql::Exception(
            boost::format("Outcome (%d facts) is negative, we can't append next step to it") % size()
        );
    }
    n.getPositiveEnd().setOutcome(out);
    return n;
}

solm::Outcome& solm::Outcome::operator+=(const solm::Outcome& out) {
    getPositiveEnd().setOutcome(out);
    return *this;
}

solm::Outcome& solm::Outcome::operator<<(const solm::Outcome& out) {
    for (Outcome::const_iterator i = out.begin(); i != out.end(); ++i) {
        push_back(*i);
    }
    return *this;
}

solm::Outcome& solm::Outcome::operator<<(const solm::Fact& f) {
    push_back(f);
    return *this;
}

const std::vector<solm::FactPath> Outcome::paths() const {
    std::vector<FactPath> v;
    for (const_iterator i = begin(); i != end(); ++i) {
        FactPath p;
        p.push_back(*i);
        if ((*i).hasOutcome()) {
            vector<FactPath> paths = (*i).getOutcome().getPaths();
            for (vector<FactPath>::const_iterator j = paths.begin(); j != paths.end(); ++j) {
                FactPath n = p + *j;
                v.push_back(n);
            }
        } else {
            v.push_back(p);
        }
    }
    return v;
}

/**
 * This outcome has a positive end?
 */
Outcome::operator bool() const {
    try {
        const_cast<Outcome*>(this)->getPositiveEnd();
        return true;
    } catch (AbsentPositiveEndException e) {
        return false;
    }
}

/**
 * Get positive end if it exists
 */
Fact& Outcome::getPositiveEnd() {
    for (iterator i = begin(); i != end(); ++i) {
        if (!(*i).hasOutcome()) {
            if (*i) {
                return *i;
            } else {
                continue;
            }
        }
        return (*i).getOutcome().getPositiveEnd();
    }
    throw AbsentPositiveEndException();
}

