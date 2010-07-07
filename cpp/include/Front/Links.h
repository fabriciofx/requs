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
 * @version $Id: Front.h 2240 2010-07-04 09:20:34Z yegor256@yahoo.com $
 */

#ifndef __INCLUDE_SCOPE_FRONT_LINKS_H
#define __INCLUDE_SCOPE_FRONT_LINKS_H

#include <string>
#include "Front/Reporter.h"
#include "Xml/Node.h"

namespace front {
    
/**
 * All links between objects in scope
 */
class Links : public Reporter {
public:
    Links(const Params& p) : Reporter(p) { /* that's it */ }
    void fill(Xml::Node&);
private:
    template<typename T> void addLocations(Xml::Node&, const std::string&) const;
};
    
}

#endif
