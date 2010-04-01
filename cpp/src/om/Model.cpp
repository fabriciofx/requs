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
 * @version $Id$
 */

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "om/Model.h"

void rqdql::om::Model::setScope(const std::vector<rqdql::scope::Statement>& items) {
    this->scope = items;

    // convert ::scope into XML
    this->_buildXml();
    
    // validate that all XPointer links are valid
    this->_validateXPointer();
}

std::string rqdql::om::Model::query(const std::string& q) {
    // convert existing XML into string and return it
    std::stringstream stringXml(std::stringstream::out);
    pugi::xml_writer_stream writer(stringXml);
    this->xml.save(writer);
    return stringXml.str();
}

/**
 * To build XML from the presented list of statements, in this->scope
 */
void rqdql::om::Model::_buildXml() {
    this->xml.append_child().set_name("scope");
    // for (this->scope::iterator)
}

/**
 * To validate XPointer links inside XML and remove
 * elements that have invalid links, adding log messages
 * about this into XML
 */
void rqdql::om::Model::_validateXPointer() {
}
