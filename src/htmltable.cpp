/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "html.hpp"

BEGIN_NAMESPACE (hy);

#define TEXT_ATTR TagEntry::TEXT_ATTR
#define EMPTY 0

/* 
 *  _char         int           int        bool      bool     _char    _char
 *   name    contextSwitch  contextLevel  closure  occurOnce  parent   rparent
 */

const TagEntry HTMLDoc::s_defaultHTMLTable[] =
{
    TagEntry("html",   1,  1,    true,   true,    EMPTY,      EMPTY),
    TagEntry("head",   1,  2,    true,   true,    "html",  EMPTY),
    TagEntry("body",   1,  2,    true,   true,    "html",  EMPTY),
    TagEntry("table",  2,  11,   true,   false,   EMPTY,      "body"),
    TagEntry("col",    0,  12,   false,  false,   "table", EMPTY),
    TagEntry("tbody",  0,  5,    true,   false,   "table", EMPTY),
    TagEntry("tr",     0,  5,    true,   false,   "table", EMPTY),
    TagEntry("th",     0,  6,    true,   false,   "tr",    EMPTY),
    TagEntry("td",     0,  6,    true,   false,   EMPTY,      "table"),
    TagEntry("title",  0,  10,   true,   true,    "head",  EMPTY),
    TagEntry("pre",    0,  11,   true,   false,   EMPTY,      "body"),
    TagEntry("center", 2,  10,   true,   false,   EMPTY,      "body"),
    TagEntry("p",      1,  12,   false,  false,   EMPTY,      "body"),
    TagEntry("ol",     1,  13,   true,   false,   EMPTY,      "body"),
    TagEntry("ul",     1,  13,   true,   false,   EMPTY,      "body"),
    TagEntry("h1",     1,  14,   true,   false,   EMPTY,      "body"),
    TagEntry("h2",     1,  14,   true,   false,   EMPTY,      "body"),
    TagEntry("h3",     1,  14,   true,   false,   EMPTY,      "body"),
    TagEntry("h4",     1,  14,   true,   false,   EMPTY,      "body"),
    TagEntry("h5",     1,  14,   true,   false,   EMPTY,      "body"),
    TagEntry("h6",     1,  14,   true,   false,   EMPTY,      "body"),
    TagEntry("li",     1,  14,   true,   false,   "ul,ol", EMPTY),
    TagEntry("form",   2,  90,   true,   false,   EMPTY,      "body"),
    TagEntry("div",    2,  90,   true,   false,   EMPTY,      "body"),
    TagEntry("style",  2,  90,   true,   false,   EMPTY,      "html"),
    TagEntry("script", 2,  90,   true,   false,   EMPTY,      "html"),
    TagEntry("select", 0,  91,   true,   false,   EMPTY,      "body"),
    TagEntry("option", 1,  92,   false,  false,   "select", EMPTY),
    TagEntry("input",  0,  99,   false,  false,   EMPTY,      "body"),
    TagEntry("map",    0,  98,   true,   false,   EMPTY,      "body"),
    TagEntry("area",   0,  99,   false,  false,   "map",   EMPTY),
    TagEntry("textarea", 0, 99,  true,   false,   EMPTY,  "body"),
    TagEntry("noscript", 2, 10,  true,   false,   EMPTY,      "html"),
    TagEntry("address",0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("span",   2,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("font",   2,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("a",      0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("b",      0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("u",      0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("i",      0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("img",    0,  TEXT_ATTR, false, false, EMPTY,    "body"),
    TagEntry("base",   0,  TEXT_ATTR, false, true, "head", EMPTY),
    TagEntry("br",     0,  TEXT_ATTR, false, false, EMPTY,    "body"),
    TagEntry("nobr",   0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("hr",     0,  TEXT_ATTR, false,  false, EMPTY,   "body"),
    TagEntry("strong", 0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("em",     0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("dd",     0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("dl",     2,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("dt",     0,  TEXT_ATTR, false, false, EMPTY,    "body"),
    TagEntry("code",   0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("sup",    0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("sub",    0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("samp",   0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("cite",   0,  TEXT_ATTR, true, false, EMPTY,     "body"),
    TagEntry("meta",   0,  TEXT_ATTR, false, true, "head" ,EMPTY),
    TagEntry("link",   0,  TEXT_ATTR, false, false, EMPTY,    "html"),
    TagEntry(EMPTY,       0,  TEXT_ATTR, false, false, EMPTY,    EMPTY)
};

END_NAMESPACE (hy);
