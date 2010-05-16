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
 *
 * This file is included ONLY from Logger.h
 */

/**
 * This is a singleton pattern. In order to get an instance
 * of this class you should call getInstance()
 */
Logger& Logger::getInstance() {
    static Logger* logger;
    if (!logger) {
        logger = new Logger();
    }
    return *logger;
}

/**
 * Log one line
 */
template <typename T> void Logger::log(const T* s, const string& m) {
    string str = m;
    if (!hasSubject(s)) {
        str = str + " (" + typeid(*s).name() + ")"; 
    }
    log(static_cast<const void*>(s), str);
}

/**
 * Log one line, by explicit link
 */
template<> void Logger::log(const void* s, const string& m) {
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
void Logger::log(int lineNo, const string& m) {
    vector<int> lines;
    lines.push_back(lineNo);
    messages.push_back(Message(lines, m));
}

/**
 * Build summary report
 */
const string Logger::getReport() const {
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
 * @see rqdql.cpp
 */
void Logger::reportLinks() {
    for (vector<Link>::const_iterator i = links.begin(); i != links.end(); ++i) {
        const void* left = (*i).getLeft();
        const void* right = (*i).getRight();
        if (!hasSubject(left)) {
            throw "LEFT subject not found when reporting links";
        }
        if (!hasSubject(right)) {
            throw "RIGHT subject not found when reporting links";
        }
        vector<string> lineNumbers;
        for (vector<int>::const_iterator j = subjects[right].begin(); j != subjects[right].end(); ++j) {
            lineNumbers.push_back((boost::format("%d") % *j).str());
        }
        log(left, "[LINK] " + boost::algorithm::join(lineNumbers, ", "));
    }
}

/**
 * Add one link between two subjects
 */
void Logger::addLink(const void* l, const void* r) { 
    // validate for duplicated links!
    links.push_back(Link(l, r)); 
}

/**
 * Log report has scope errors?
 */
bool Logger::hasErrors() const {
    for (vector<Message>::const_iterator i = messages.begin(); i != messages.end(); ++i) {
        if ((*i).isError()) {
            return true;
        }
    }
    return false;
}

/**
 * This message is a scope error?
 */
bool Logger::Message::isError() const {
    // it's a system message
    if (boost::regex_match(message, boost::regex("^\\[[A-Z]+\\]"))) {
        return false;
    }
    if (lines.empty()) {
        return false;
    }
    return true;
}

