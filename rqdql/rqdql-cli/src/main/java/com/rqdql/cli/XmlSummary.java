/**
 * Copyright (c) 2009-2011, RQDQL.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met: 1) Redistributions of source code must retain the above
 * copyright notice, this list of conditions and the following
 * disclaimer. 2) Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution. 3) Neither the name of the RQDQL.com nor
 * the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.rqdql.cli;

import com.jcabi.log.Logger;
import com.rqdql.sol.Formula;
import com.rqdql.sol.Sud;
import com.rqdql.thesaurus.RqdqlText;
import com.rqdql.thesaurus.Type;

/**
 * Convert RQDQL grammar into XML.
 *
 * @author Yegor Bugayenko (yegor@rqdql.com)
 * @version $Id$
 */
public final class XmlSummary {

    /**
     * The document to work with.
     */
    private final transient String document;

    /**
     * Public ctor.
     * @param doc The document with grammar
     */
    public XmlSummary(final String doc) {
        this.document = doc;
    }

    /**
     * Add new reporter to the summary.
     * @param report The report
     */
    public void add(final Report report) {
        // todo
    }

    /**
     * Get XML summary.
     * @return The XML report
     */
    public String xml() {
        final RqdqlText text = new RqdqlText(this.document);
        final Type sud = text.sud();
        final Formula predicate = new Sud(sud);
        Logger.info(
            this,
            "reporting finished with %s",
            predicate
        );
        return String.format(
            "<?xml version='1.0'?><rqdql>%d</rqdql>",
            this.document.length()
        );
    }

}
