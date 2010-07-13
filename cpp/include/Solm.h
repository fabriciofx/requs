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

#ifndef __INCLUDE_SOLM_H
#define __INCLUDE_SOLM_H

#include "Solm/Predicate.h"

namespace solm {
    
/**
 * Second-Order Logic Model (SOLM)
 *
 * This is a huge AND predicate that includes all other predicates
 */
class Solm : public Predicate {
    
public:
    
    /**
     * Public constructor
     */
    Solm();

    /**
     * To calculate ambiguity of the SOLM, as a relation between
     * total number of silent elements and data manipulators, which
     * include CREATED(), DELETED() and READ(). This list of manipulators
     * is fixed and won't be changed ever.
     */
    const double ambiguity() const;

private:

};

}

#endif
