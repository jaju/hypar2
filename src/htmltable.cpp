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
    TagEntry(L("html"),   1,  1,    true,   true,    EMPTY,      EMPTY),
    TagEntry(L("head"),   1,  2,    true,   true,    L("html"),  EMPTY),
    TagEntry(L("body"),   1,  2,    true,   true,    L("html"),  EMPTY),
    TagEntry(L("table"),  2,  11,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("col"),    0,  12,   false,  false,   L("table"), EMPTY),
    TagEntry(L("tbody"),  0,  5,    true,   false,   L("table"), EMPTY),
    TagEntry(L("tr"),     0,  5,    true,   false,   L("table"), EMPTY),
    TagEntry(L("th"),     0,  6,    true,   false,   L("tr"),    EMPTY),
    TagEntry(L("td"),     0,  6,    true,   false,   EMPTY,      L("table")),
    TagEntry(L("title"),  0,  10,   true,   true,    L("head"),  EMPTY),
    TagEntry(L("pre"),    0,  11,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("center"), 2,  10,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("p"),      1,  12,   false,  false,   EMPTY,      L("body")),
    TagEntry(L("ol"),     1,  13,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("ul"),     1,  13,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("h1"),     1,  14,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("h2"),     1,  14,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("h3"),     1,  14,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("h4"),     1,  14,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("h5"),     1,  14,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("h6"),     1,  14,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("li"),     1,  14,   true,   false,   L("ul,ol"), EMPTY),
    TagEntry(L("form"),   2,  90,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("div"),    2,  90,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("style"),  2,  90,   true,   false,   EMPTY,      L("html")),
    TagEntry(L("script"), 2,  90,   true,   false,   EMPTY,      L("html")),
    TagEntry(L("select"), 0,  91,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("option"), 1,  92,   false,  false,   L("select"), EMPTY),
    TagEntry(L("input"),  0,  99,   false,  false,   EMPTY,      L("body")),
    TagEntry(L("map"),    0,  98,   true,   false,   EMPTY,      L("body")),
    TagEntry(L("area"),   0,  99,   false,  false,   L("map"),   EMPTY),
    TagEntry(L("textarea"), 0, 99,  true,   false,   EMPTY,  L("body")),
    TagEntry(L("noscript"), 2, 10,  true,   false,   EMPTY,      L("html")),
    TagEntry(L("address"),0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("span"),   2,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("font"),   2,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("a"),      0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("b"),      0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("u"),      0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("i"),      0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("img"),    0,  TEXT_ATTR, false, false, EMPTY,    L("body")),
    TagEntry(L("base"),   0,  TEXT_ATTR, false, true, L("head"), EMPTY),
    TagEntry(L("br"),     0,  TEXT_ATTR, false, false, EMPTY,    L("body")),
    TagEntry(L("nobr"),   0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("hr"),     0,  TEXT_ATTR, false,  false, EMPTY,   L("body")),
    TagEntry(L("strong"), 0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("em"),     0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("dd"),     0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("dl"),     2,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("dt"),     0,  TEXT_ATTR, false, false, EMPTY,    L("body")),
    TagEntry(L("code"),   0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("sup"),    0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("sub"),    0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("samp"),   0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("cite"),   0,  TEXT_ATTR, true, false, EMPTY,     L("body")),
    TagEntry(L("meta"),   0,  TEXT_ATTR, false, true, L("head") ,EMPTY),
    TagEntry(L("link"),   0,  TEXT_ATTR, false, false, EMPTY,    L("html")),
    TagEntry(EMPTY,       0,  TEXT_ATTR, false, false, EMPTY,    EMPTY)
};

END_NAMESPACE (hy);
