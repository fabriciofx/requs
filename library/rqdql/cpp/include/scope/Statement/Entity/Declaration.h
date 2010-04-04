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

#ifndef __INCLUDE_SCOPE_STATEMENT_ENTITY_DECLARATION_H
#define __INCLUDE_SCOPE_STATEMENT_ENTITY_DECLARATION_H

#include <string>
#include "pugixml/pugixml.hpp"
#include "funcs.h"
#include "scope/Statement/Entity.h"

class rqdql::scope::EntityDeclarationStatement : public rqdql::scope::EntityStatement {
private:
    std::string _declaration; // informal description of the name

public:
    EntityDeclarationStatement(const LeftName& name, const std::string& declaration) :
        EntityStatement(name), _declaration(declaration) {
        trim(this->_declaration);
    };
    void derive(pugi::xml_node&);
};

#endif
