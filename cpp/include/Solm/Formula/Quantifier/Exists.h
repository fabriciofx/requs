/**
 * * RQDQL.com, Requirements Definition and Query Language
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
 * @version $Id: UseCase.h 1641 2010-04-16 07:56:07Z yegor256@yahoo.com $
 */

#ifndef __INCLUDE_SOLM_FORMULA_QUANTIFIER_EXISTS_H
#define __INCLUDE_SOLM_FORMULA_QUANTIFIER_EXISTS_H

#include <string>

#include "Solm/Formula/Quantifier.h"
#include "Solm/Outcome.h"
#include "Solm/Fact.h"
#include "Solm/Snapshot.h"

namespace solm {

class Exists : public Quantifier<Exists> {
public:
    virtual const std::string toString() const { return _toString("\\exists"); }
    virtual Outcome getOutcome(const Fact&, const Snapshot::Mapping&) const;
};

}

#endif
