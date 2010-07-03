/**
 *
 * FaZend.com, Fully Automated Zend Framework
 * RQDQL.com, Requirements Definition and Query Language
 *
 * Redistribution and use in source and binary forms, with or 
 * without modification, are PROHIBITED without prior written 
 * permission from the author. This product may NOT be used 
 * anywhere and on any computer except the server platform of 
 * FaZend.com. located at www.fazend.com. If you received this 
 * code occasionally and without intent to use it, please report 
 * this incident to the author by email: team@rqdql.com
 *
 * @author Yegor Bugayenko <egor@tpc2.com>
 * @copyright Copyright (c) rqdql.com, 2010
 * @version $Id: Silent.h 2095 2010-05-28 07:26:19Z yegor256@yahoo.com $
 */

#ifndef __INCLUDE_SOLM_FORMULA_PREDICATE_H
#define __INCLUDE_SOLM_FORMULA_PREDICATE_H

#include <vector>

#include "Solm/Formula.h"
#include "Solm/Formula/Parametrized.h"
#include "Solm/Fact.h"
#include "Solm/Snapshot.h"

namespace solm {

class Predicate : public Formula, public Parametrized<Predicate> {
public:
    Predicate(const std::string& n) : Parametrized<Predicate>(), name(n) { /* that's it */ }
    const std::string toString() const;
    virtual Outcome getOutcome(const Fact&, const Snapshot::Mapping&) const;
    const std::string& getName() const { return name; }
private:
    std::string name;
};

}

#endif

