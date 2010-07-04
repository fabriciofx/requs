/**
 * * RQDQL.com, Requirements Definition and Query Language
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
 *
 * This file is included ONLY from Logger.h
 */
 
#include "Logger.h"
#include "rqdql.h"
#include "rqdql/Exception.h"

#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/join.hpp>

/**
 * Log one line
 */
template <typename T> void rqdql::Logger::log(const T* s, const std::string& m) {
    using namespace std;
    string str = m;
    if (!hasSubject(s)) {
        str = str + " (" + typeid(*s).name() + ")"; 
    }
    log(static_cast<const void*>(s), str);
}

/**
 * Log one line, by explicit link
 */
template<> void rqdql::Logger::log(const void* s, const std::string& m) {
    using namespace std;
    vector<int> lines;
    string str = m;
    if (hasSubject(s)) {
        lines = subjects[s];
    }
    messages.push_back(Message(lines, str));
}

/**
 * Log one line, we know exact line number
 */
void rqdql::Logger::log(int lineNo, const std::string& m) {
    using namespace std;
    vector<int> lines;
    lines.push_back(lineNo);
    messages.push_back(Message(lines, m));
}

/**
 * Build summary report
 */
const std::string rqdql::Logger::getReport() const {
    using namespace std;
    vector<string> msgs;
    for (vector<Message>::const_iterator i = messages.begin(); i != messages.end(); ++i) {
        // sort them in proper order
        vector<int> lines = (*i).getLines();
        sort(lines.begin(), lines.end());
        
        // leave only unique elements
        vector<int>::const_iterator end = unique(lines.begin(), lines.end());
        
        // convert them to strings
        vector<string> lineNumbers;
        for (vector<int>::const_iterator j = lines.begin(); j != end; ++j) {
            lineNumbers.push_back((boost::format("%d") % *j).str());
        }
        
        // if no lines founds, show at least ZERO
        if (lineNumbers.empty()) {
            lineNumbers.push_back("0");
        }
        
        // create a message
        msgs.push_back((boost::format("[%s] %s") 
            % boost::algorithm::join(lineNumbers, ", ") 
            % (*i).getMessage()
        ).str());
    }
    return boost::algorithm::join(msgs, "\n");
}

/**
 * All links found between elements report to log lines
 */
const std::vector<rqdql::Logger::Link>& rqdql::Logger::getLinks() {
    using namespace std;
    for (vector<Link>::iterator i = links.begin(); i != links.end(); ++i) {
        const void* left = (*i).getLeft();
        const void* right = (*i).getRight();
        if (!hasSubject(left)) {
            throw rqdql::Exception(rqdql::_t("LEFT subject not found when reporting links"));
        }
        if (!hasSubject(right)) {
            throw rqdql::Exception(rqdql::_t("RIGHT subject not found when reporting links"));
        }
        (*i).setLeftLines(subjects[left]);
        (*i).setRightLines(subjects[right]);
    }
    return links;
}

/**
 * Add a new subject
 * @see protocol()
 */
void rqdql::Logger::addSubject(const void* s, int l) { 
    subjects[s].push_back(l);
}

/**
 * We already have this subject attached to some lines?
 * @see Logger::addClone()
 */
bool rqdql::Logger::hasSubject(const void* s) const { 
    return subjects.find(s) != subjects.end(); 
}

/**
 * Add one link between two subjects
 * Left object is the source of the link, and the right is
 * the destination
 */
void rqdql::Logger::addLink(const void* l, const void* r) { 
    // validate for duplicated links!
    links.push_back(Link(l, r)); 
}

/**
 * Add object location, which is the same as the latest location of
 * another object.
 * @see Flow::makeFormula()
 * @see Flows::setFormula()
 */
void rqdql::Logger::addClone(const void* existing, const void* n) {
    using namespace std;
    if (!hasSubject(existing)) {
        throw rqdql::Exception(rqdql::_t("EXISTING subject not found when cloning"));
    }
    if (!subjects[existing].size()) {
        throw rqdql::Exception(rqdql::_t("No lines were reported yet for EXISTING subject"));
    }
    addSubject(n, *(subjects[existing].end()-1));
}

