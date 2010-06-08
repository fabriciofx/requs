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

/**
 * Create an outcome of this formula, list of facts
 */
Outcome Declaration::getOutcome(const Fact& f, const Snapshot::Mapping& m = Snapshot::NullMapping) const { 
    Fact fact;
    fact.setFormula(this);
    
    Snapshot s = f.getSnapshot();
    for (Vars::const_iterator i = getVars().begin(); i != getVars().end(); ++i) {
        Snapshot::Object& obj = s.create("");
        s.assignName(obj, *i);
        s.assignId(obj);
    }
    fact.setSnapshot(s);
    
    return (Outcome() << fact) + getFormula()->getOutcome(fact); 
}

/**
 * Convert this declaration to the LaTeX text
 */
const string Declaration::toString() const { 
    string f;
    if (Unary<Declaration>::getFormulas().size() != 1) {
        rqdql::get<rqdql::Logger>().log(
            this, 
            (boost::format("Declaration '%s' shall have exactly one formula inside") % name).str()
        );
        f = Err("'missed formula").toString();
    } else {
        f = getFormula()->toString();
    }
    
    Vars v = getVars();
    if (!getVars().size()) {
        rqdql::get<rqdql::Logger>().log(
            this, 
            (boost::format("Declaration '%s' shall have at least one argument") % name).str()
        );
        v.push_back("x");
    }
    
    return name + "(" + boost::algorithm::join(v, ", ") + "): " + f;
}
