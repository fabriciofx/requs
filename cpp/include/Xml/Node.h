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

/**
 * XML Node
 */
class Node : public pugi::xml_node {

public:

    /**
     * Public constructor
     */
    Node(pugi::xml_node& n) : xml_node(n) { /* that's it */ }

    /**
     * Add new child node
     */
    Node operator+(const std::string&);

    /**
     * Get a child
     */
    Node operator/(const std::string&);

    /**
     * Set value of a node (as text)
     */
    template <typename T> Node& operator=(const T&);

    /**
     * Retrieve an attribute
     */
    Attribute operator[](const std::string&);

};

}

#endif
