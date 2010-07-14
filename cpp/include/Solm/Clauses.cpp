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
 * @version $Id: Context.h 2304 2010-07-13 11:58:31Z yegor256@yahoo.com $
 */

#include <string>
#include "Solm/Clauses.h"

solm::Clauses::Clauses() {
}

bool solm::Clauses::operator==(const solm::Clauses&) const {
    return true;
}

solm::Clauses& solm::Clauses::operator<<(const solm::Clauses&) {
    return *this;
}

