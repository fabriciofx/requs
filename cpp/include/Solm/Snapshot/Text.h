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
 * @version $Id: Snapshot.h 2256 2010-07-08 08:30:01Z yegor256@yahoo.com $
 */

#ifndef __INCLUDE_SOLM_SNAPSHOT_VALUE_TEXT_H
#define __INCLUDE_SOLM_SNAPSHOT_VALUE_TEXT_H

#include <string>
#include "Solm/Snapshot/Value.h"

namespace solm {
namespace snapshot {

/**
 * Simple value in form of text
 */
class Text : public Value {
public:
    Text(const string& s) : _text(s) { /* that's it */ }
    operator std::string() const { return _text; }
private:
    std::string _text;
};

}
}

#endif
