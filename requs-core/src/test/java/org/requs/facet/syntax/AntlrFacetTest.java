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
package org.requs.facet.syntax;

import com.rexsl.test.XhtmlMatchers;
import java.io.IOException;
import javax.xml.transform.Source;
import org.apache.commons.io.IOUtils;
import org.hamcrest.MatcherAssert;
import org.junit.Ignore;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import org.requs.Docs;
import org.requs.facet.syntax.ontology.Flow;

/**
 * Test case for {@link AntlrFacet}.
 * @author Yegor Bugayenko (yegor@tpc2.com)
 * @version $Id$
 */
public final class AntlrFacetTest {

    /**
     * Temporary folder.
     * @checkstyle VisibilityModifier (3 lines)
     */
    @Rule
    public transient TemporaryFolder temp = new TemporaryFolder();

    /**
     * AntlrFacet can parse input text and produce clauses.
     * @throws Exception When necessary
     */
    @Test
    public void parsesInputAndProducesTypes() throws Exception {
        MatcherAssert.assertThat(
            this.parse("Sud includes: test."),
            XhtmlMatchers.hasXPaths("/spec/types")
        );
    }

    /**
     * AntlrFacet can report syntax/grammar errors found.
     * @throws Exception When necessary
     */
    @Test
    public void reportsErrorsFound() throws Exception {
        MatcherAssert.assertThat(
            this.parse(
                "User is ?. Site is ?. Employee is a User."
            ),
            XhtmlMatchers.hasXPaths(
                "/spec/types[count(type) = 3]",
                "//type[name='User']",
                "//type[name='Employee' and parents/type='User']",
                "/spec/errors[count(error)=2]",
                "/spec/errors/error[@type='syntax' and @line='1']"
            )
        );
    }

    /**
     * AntlrFacet can compile a complex document.
     * @throws Exception When necessary
     */
    @Test
    public void compilesComplexSpec() throws Exception {
        MatcherAssert.assertThat(
            this.parse(
                IOUtils.toString(
                    this.getClass().getResourceAsStream("example.req")
                )
            ),
            XhtmlMatchers.hasXPaths(
                "/spec/errors[not(error)]",
                "/spec/types[count(type)=2]",
                "/spec/types/type[name='User']",
                "/spec/types/type[name='Fraction']",
                "//type[name='Fraction']/slots/slot[name='numerator']",
                "//type[name='Fraction']/slots/slot[name='denominator']",
                "//type[name='Fraction']/info[informal='a math calculator']",
                "/spec[count(//method)=1]",
                "//method[id='UC1']/attributes[attribute='must']",
                String.format(
                    "//method[id='UC1']/bindings/binding[name='%s']",
                    Flow.SELF
                ),
                String.format(
                    "//method[id='UC1']/bindings/binding[ name='%s']",
                    Flow.SUD
                )
            )
        );
    }

    /**
     * AntlrFacet can parse all possible cases.
     * @throws Exception When necessary
     */
    @Test
    public void parsesAllPossibleCases() throws Exception {
        MatcherAssert.assertThat(
            this.parse(
                IOUtils.toString(
                    this.getClass().getResourceAsStream("all-cases.req")
                )
            ),
            XhtmlMatchers.hasXPath("/spec[not(errors/error)]")
        );
    }

    /**
     * AntlrFacet can parse all possible errors.
     * @throws Exception When necessary
     */
    @Test
    @Ignore
    public void parsesAllPossibleErrors() throws Exception {
        final String[] specs = {
            "\"alpha",
            "User is a",
            "User needs: a as",
            "UC1 where",
        };
        for (final String spec : specs) {
            MatcherAssert.assertThat(
                this.parse(spec),
                XhtmlMatchers.hasXPath("/spec/errors/error")
            );
        }
    }

    /**
     * Parse input.
     * @param input Input syntax
     * @return XML output
     * @throws IOException If fails
     */
    private Source parse(final String input) throws IOException {
        final Docs docs = new Docs.InDir(this.temp.newFolder());
        docs.get("input.req").write(input);
        new AntlrFacet().touch(docs);
        return XhtmlMatchers.xhtml(docs.get("main.xml").read());
    }

}
