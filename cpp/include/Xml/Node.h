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

#ifndef __INCLUDE_SCOPE_XML_NODE_H
#define __INCLUDE_SCOPE_XML_NODE_H

#include <string>
#include <pugixml/pugixml.hpp>

namespace Xml {

/**
 * forward declaration
 */
class Attribute;

class Node : public pugi::xml_node {
public:
    Node(pugi::xml_node& n) : xml_node(n) { /* that's it */ }
    Node operator+(const std::string&);
    Node& operator=(const std::string&);
    Node& operator=(double);
    Node& operator=(int);
    Attribute operator[](const std::string&);
};

Node operator/(pugi::xml_node n, const std::string& s);

}

#endif
