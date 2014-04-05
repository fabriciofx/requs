/**
 * Copyright (c) 2009-2014, requs.org
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met: 1) Redistributions of source code must retain the above
 * copyright notice, this list of conditions and the following
 * disclaimer. 2) Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution. 3) Neither the name of the requs.org nor
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
package org.requs.facet.decor;

import com.jcabi.xml.XMLDocument;
import com.jcabi.xml.XSLDocument;
import com.rexsl.test.XhtmlMatchers;
import org.hamcrest.MatcherAssert;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import org.phandom.Phandom;
import org.requs.Docs;

/**
 * Test case for {@link Scaffolding}.
 * @author Yegor Bugayenko (yegor@tpc2.com)
 * @version $Id$
 * @since 1.2
 * @checkstyle MultipleStringLiteralsCheck (500 lines)
 */
public final class ScaffoldingTest {

    /**
     * Temporary folder.
     * @checkstyle VisibilityModifier (3 lines)
     */
    @Rule
    public transient TemporaryFolder temp = new TemporaryFolder();

    /**
     * Scaffolding can create an index file.
     * @throws Exception When necessary
     */
    @Test
    public void createsIndexFile() throws Exception {
        final Docs docs = new Docs.InDir(this.temp.newFolder());
        new Scaffolding().touch(docs);
        MatcherAssert.assertThat(
            XhtmlMatchers.xhtml(docs.get("index.xml").read()),
            XhtmlMatchers.hasXPaths("/index")
        );
    }

    /**
     * Scaffolding can build a renderable XML+XSL.
     * @throws Exception When necessary
     */
    @Test
    public void rendersXslt() throws Exception {
        final Docs docs = new Docs.InDir(this.temp.newFolder());
        new Scaffolding().touch(docs);
        final String html = XSLDocument
            .make(this.getClass().getResourceAsStream("index.xsl"))
            .transform(new XMLDocument(docs.get("index.xml").read()))
            .nodes("/*").get(0).toString();
        MatcherAssert.assertThat(
            XhtmlMatchers.xhtml(new Phandom(html).dom()),
            XhtmlMatchers.hasXPaths("//table")
        );
    }

}
