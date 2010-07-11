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

#ifndef __INCLUDE_SCOPE_SCANNER_H
#define __INCLUDE_SCOPE_SCANNER_H

#include <string>

typedef union {
    std::string* name;
    int num;
    void* ptr;
} YYSTYPE;

// bison/flex file
// this define will protect us against BISON default YYSTYPE
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL 1
#include "Scanner/symbols.h"

extern int yyparse();
extern int yylex(void);
extern int yylineno;

namespace rqdql {

/**
 * Wrapper around BISON/FLEX functionality
 */
class Scanner {

public:
    
    /**
     * Scan input string and inject results into proxy::Proxy
     */
    void scan(const std::string&);

};

}

#endif
