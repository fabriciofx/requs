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

#ifndef __INCLUDE_SOLM_ATOM_H
#define __INCLUDE_SOLM_ATOM_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "Solm/Argument.h"
#include "Solm/Chain.h"
#include "Solm/Term.h"

namespace solm {

/**
 * Second order logic atom
 */
class Atom : public Argument {

public:
    
    /**
     * Public constructor
     *
     * @param v The value of the atom, like "file.txt"
     */
    Atom(const string& v) : _value(v) { /* that's it */ }

    /**
     * To resolve this argument on some clauses
     * and produce a new Chain of clauses.
     */
    Chain operator+(const Term&) const { return Chain(); }

    /**
     * Convert the predicate to string
     */
    operator std::string() const { return _value; }

private:
    
    /**
     * Value of the atom
     */
    std::string _value;

};

}

#endif
