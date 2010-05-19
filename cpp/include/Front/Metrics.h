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
 * @version $Id: UseCase.h 1641 2010-04-16 07:56:07Z yegor256@yahoo.com $
 *
 * This file is included ONLY from Front.h
 */

void Metrics::fillNode(pugi::xml_node& n) {
    using namespace pugi;

    pugi::xml_node x;
    
    x = n.append_child();
    x.set_name("ambiguity");
    x.append_child(pugi::node_pcdata).set_value(
        (boost::format("%0.3f") % solm::Solm::getInstance().getAmbiguity()).str().c_str()
    );
    
    x = n.append_child();
    x.set_name("errorsCount");
    x.append_child(pugi::node_pcdata).set_value(
        (boost::format("%d") % rqdql::Logger::getInstance().size()).str().c_str()
    );

    if (getParam<bool>("lists", true)) {
        x = n.append_child();
        x.set_name("types");
        vector<string> v = proxy::Proxy::getInstance().getTypeNames();
        for (vector<string>::const_iterator i = v.begin(); i != v.end(); ++i) {
            pugi::xml_node t = x.append_child();
            t.set_name("type");
            t.append_child(pugi::node_pcdata).set_value((*i).c_str());
        }

        x = n.append_child();
        x.set_name("useCases");
        v = proxy::Proxy::getInstance().getAllUseCaseNames();
        for (vector<string>::const_iterator i = v.begin(); i != v.end(); ++i) {
            pugi::xml_node t = x.append_child();
            t.set_name("uc");
            t.append_child(pugi::node_pcdata).set_value((*i).c_str());
        }
    }
}


