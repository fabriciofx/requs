<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" version="2.0" exclude-result-prefixes="xs">
    <xsl:output method="xml" cdata-section-elements="svg html"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="/spec/errors">
        <xsl:copy>
            <xsl:apply-templates select="error"/>
            <xsl:for-each select="//steps[count(step) &gt; 8]">
                <error type="semantic" pos="0">
                    <xsl:attribute name="line">
                        <xsl:value-of select="step[1]/mentioned/where[1]"/>
                    </xsl:attribute>
                    <xsl:text>There are more than eight steps, the flow is difficult to understand</xsl:text>
                </error>
            </xsl:for-each>
        </xsl:copy>
    </xsl:template>
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
