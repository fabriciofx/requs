<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xs="http://www.w3.org/2001/XMLSchema"
    xmlns:re="http://www.requs.org"
    xmlns:r="org.requs.facet.uml.Plant" version="2.0"
    exclude-result-prefixes="xs r re">
    <xsl:import href="uml/functions.xsl"/>
    <xsl:output method="xml" cdata-section-elements="svg html"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="method">
        <xsl:copy>
            <xsl:apply-templates select="(node() except diagrams)|@*"/>
            <xsl:variable name="uml">
                <xsl:text>left to right direction&#10;</xsl:text>
                <xsl:text>title </xsl:text>
                <xsl:value-of select="id"/>
                <xsl:text>: Use Case Diagram&#10;</xsl:text>
                <xsl:apply-templates select="." mode="uml"/>
            </xsl:variable>
            <diagrams>
                <xsl:apply-templates select="diagrams/*"/>
                <diagram>
                    <uml>
                        <xsl:value-of select="$uml"/>
                    </uml>
                    <svg>
                        <xsl:value-of select="r:svg(re:plant($uml))"/>
                    </svg>
                </diagram>
            </diagrams>
        </xsl:copy>
    </xsl:template>
    <xsl:template match="method" mode="uml">
        <xsl:variable name="primary" select="bindings/binding[name='_self']/type"/>
        <xsl:text>actor </xsl:text>
        <xsl:value-of select="$primary"/>
        <xsl:text> as primary&#10;</xsl:text>
        <xsl:text>usecase main as</xsl:text>
        <xsl:text>&quot;</xsl:text>
        <xsl:value-of select="id"/>
        <xsl:text>&#10;</xsl:text>
        <xsl:value-of select="re:signature(signature)"/>
        <xsl:text>&quot;&#10;</xsl:text>
        <xsl:text>primary -> main&#10;</xsl:text>
        <xsl:for-each select=".//signature[@ref]">
            <xsl:text>usecase </xsl:text>
            <xsl:value-of select="@ref"/>
            <xsl:text>as &quot;</xsl:text>
            <xsl:value-of select="@ref"/>
            <xsl:text>&#10;</xsl:text>
            <xsl:value-of select="re:signature(/spec/methods/method[id=current()/@ref]/signature)"/>
            <xsl:text>&quot;&#10;</xsl:text>
            <xsl:text>main ---> </xsl:text>
            <xsl:value-of select="@ref"/>
            <xsl:text>&#10;</xsl:text>
        </xsl:for-each>
    </xsl:template>
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
