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
 * @version $Id: rqdql.cpp 1746 2010-05-04 11:47:08Z yegor256@yahoo.com $
 */

#include "rqdql.h"
#include "Scanner.h"
#include "Logger.h"
#include <stdarg.h>
#include <iostream>

using namespace std;

rqdql::LogLevel rqdql::level = L_ERROR;

int main(int argc, char** argv) {
    string s;
    cin >> s;
    rqdql::Scanner::getInstance().scan(s);

    // everything OK?
    if (rqdql::Logger::getInstance().empty()) {
        return 0;
    }
    
    // no, we should display all errors found
    cout << rqdql::Logger::getInstance().getReport() << endl;
    return -1;
}

