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
 * @version $Id: UseCase.h 1641 2010-04-16 07:56:07Z yegor256@yahoo.com $
 *
 * This file is included ONLY from Front.h
 */

#include <string>
#include <vector>
#include "rqdql.h"
#include "Logger.h"
#include "Front/Errors.h"
#include "Front/Errors/Error.h"
#include "Xml/Node.h"
#include "Xml/Attribute.h"

/**
 * Convert errors into XML node
 */
void front::Errors::fill(Xml::Node& n) {
    using namespace std;
    
    int max = getParam<int>("max", 50);

    vector<errors::Error> errors;
    typedef vector<rqdql::Logger::Message> Msgs;
    Msgs v = rqdql::get<rqdql::Logger>().getMessages();
    for (Msgs::const_iterator i = v.begin(); i != v.end(); ++i) {
        for (vector<int>::const_iterator k = (*i).getLines().begin(); k != (*i).getLines().end(); ++k) {
            errors.push_back(errors::Error(*k, (*i).getMessage()));
        }
    }
    
    if (getParam<bool>("unique", true)) {
        errors.resize(unique(errors.begin(), errors.end()) - errors.begin());
        sort(errors.begin(), errors.end());
    }
    
    for (vector<errors::Error>::const_iterator i = errors.begin(); i != errors.end(); ++i) {
        (n / "errors" + "error" = (*i).message) ["line"] = (*i).line;
        if (!--max) {
            break;
        }
    }
}


