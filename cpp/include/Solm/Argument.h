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

#ifndef __INCLUDE_SOLM_ARGUMENT_H
#define __INCLUDE_SOLM_ARGUMENT_H

#include <string>
#include "Solm/Chain.h"
#include "Solm/Term.h"

namespace solm {

/**
 * Second order logic argument, it's an abstract class
 */
class Argument {

public:

    /**
     * Abstract method, to resolve this argument on some clauses
     * and produce a new Chain of clauses.
     * The method has to be overriden in child classes.
     */
    virtual Chain operator+(const Term&) const = 0;

    /**
     * Convert the argument to string
     */
    virtual operator std::string() const = 0;

private:
    
};

}

#endif
