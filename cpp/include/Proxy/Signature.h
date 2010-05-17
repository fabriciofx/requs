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
 * This file is included ONLY from Proxy.h
 */

Signature* Signature::explain(const string& n, Explanation* e) { 
    if (!hasPlace(n)) {
        throw (boost::format("There is no place '%s' in '%s', can't explain") % n % text).str();
    }
    explanations[n] = e;
    return this; 
}

/**
 * Simplify the signature before matching
 * @see match()
 */
const string Signature::simplify(const string& s) const {
    typedef map<string, string> Reps;
    Reps reps;
    reps["\\{.*?\\}"] = "{...}";
    reps["[ \\t\\n\\r]+"] = " ";

    string n = boost::algorithm::to_lower_copy(s);
    for (Reps::const_iterator i = reps.begin(); i != reps.end(); ++i) {
        n = boost::algorithm::replace_all_regex_copy(
            n, // source string
            boost::regex((*i).first), // what to find
            (*i).second // what to replace to
        );
    }
    return n;
}

/**
 * Compare two signatures and match them. Returns TRUE if
 * two signatures look identical or very similar.
 */
bool Signature::match(const Signature* s) const {
    return simplify(text) == simplify(s->text);
}

/**
 * Convert one explanation to string
 */
const string Signature::Explanation::toString() const {
    if (type) {
        return type->getName();
    }
    if (!slot.empty() && !object.empty()) {
        return slot + " (the " + object + ")"; 
    }
    return "itself"; 
}

/**
 * This signature is already a formula ready-to-go into SOLM?
 */
bool Signature::isFormula() const {
    try {
        makeFormula();
    } catch (...) {
        return false;
    }
    return true;
}

/**
 * Get the formula ready-to-go into SOLM
 */
solm::Formula* Signature::makeFormula() const {
    using namespace solm;
    string t = simplify(text);
    if (boost::regex_match(t, boost::regex("\\{...\\} reads? \\{...\\}"))) {
        return (new Read())->arg(getPlaceName(0))->arg(getPlaceName(1));
    }
    if (boost::regex_match(t, boost::regex("\\{...\\} creates? \\{...\\}"))) {
        return (new Created())->arg(getPlaceName(0))->arg(getPlaceName(1));
    }
    if (boost::regex_match(t, boost::regex("\\{...\\} deletes? \\{...\\}"))) {
        return (new Deleted())->arg(getPlaceName(0))->arg(getPlaceName(1));
    }
    if (boost::regex_match(t, boost::regex("failure"))) {
        return (new Throw())->arg("'not implemented yet");
    }
    if (boost::regex_match(t, boost::regex("if failure"))) {
        return (new Caught())->arg("'not implemented yet");
    }
    throw (boost::format("Signature '%s' is not a formula") % text).str();
}

/**
 * Get name of the element located at i-th position
 */
string Signature::getPlaceName(size_t i) const {
    if (!hasPlaces()) {
        throw (boost::format("No places found in signature '%s'") % text).str();
    }
    if (i >= getPlaces().size()) {
        throw (boost::format("Places no.%d is absent in signature '%s'") % i % text).str();
    }
    return getPlaces().at(i);
}

/**
 * This signature has any places to explain?
 */
bool Signature::hasPlaces() const {
    return !getPlaces().empty();
}

/**
 * This signature has this place?
 */
bool Signature::hasPlace(const string& n) const {
    vector<string> places = getPlaces();
    return (places.end() != find(places.begin(), places.end(), n));
}

/**
 * Get full list of places
 */
vector<string> Signature::getPlaces() const {
    vector<string> places;
    string::const_iterator begin = text.begin();
    string::const_iterator end = text.end();
    boost::match_results<string::const_iterator> what;
    while (regex_search(begin, end, what, boost::regex("\\{(.*?)\\}"))) {
        places.push_back(string(what[1].first, what[2].second-1));
        begin = what[0].second;
    }
    
    // for (vector<string>::const_iterator i = places.begin(); i != places.end(); ++i) {
    //     cout << *i << endl;
    // }
    // terminate();
    return places;
}

