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

#ifndef __INCLUDE_SCOPE_LOGGER_H
#define __INCLUDE_SCOPE_LOGGER_H

#include <vector>
#include <map>
#include <string>
#include "Logger/Message.h"
#include "Logger/Link.h"

namespace rqdql {

/**
 * Universal logging component
 */
class Logger {

public:

    /**
     * Public constructor
     */
    Logger();

    /**
     * Add a new subject
     * @see protocol()
     */
    void addSubject(const void*, int);

    /**
     * Add object location, which is the same as the latest location of
     * another object.
     * @see Flow::makeFormula()
     * @see Flows::setFormula()
     */
    void addClone(const void*, const void*);

    /**
     * We already have this subject attached to some lines?
     * @see Logger::addClone()
     */
    bool hasSubject(const void*) const;

    /**
     * Add one link between two subjects
     * Left object is the source of the link, and the right is
     * the destination
     */
    void addLink(const void*, const void*);

    /**
     * Log one line, by explicit link
     */
    template <typename T> void log(const T*, const std::string&);

    /**
     * Log one line, we know exact line number
     */
    void log(int, const std::string&);

    /**
     * The log is empty?
     */
    bool empty() const { return _messages.empty(); }

    /**
     * Build summary report
     */
    const std::string getReport() const;

    /**
     * Get full list of lines for the given subject
     */
    const std::vector<int>& getLinesFor(const void* s) { return _subjects[s]; }

    /**
     * Return full list of messages
     */
    const std::vector<logger::Message>& getMessages() const { return _messages; }

    /**
     * All links found between elements report to log lines
     */
    const std::vector<logger::Link>& getLinks();

    /**
     * Remove all log lines, used for testing
     */
    void clear() { _messages.clear(); }

    /**
     * Get total size of log archive
     */
    size_t size() const { return _messages.size(); }

private:

    /**
     * Collection of known subject in scope, with links to their lines
     * (where they were found)
     */
    typedef std::map<const void*, std::vector<int> > Subjects;

    /**
     * List of subjects
     */
    Subjects _subjects;

    /**
     * List of links between subjects
     */
    std::vector<logger::Link> _links;

    /**
     * Messages attached to subjects
     */
    std::vector<logger::Message> _messages;

};

}

#endif
