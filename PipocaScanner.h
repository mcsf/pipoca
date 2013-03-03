// $Id: PipocaScanner.h,v 1.1 2010-03-27 17:00:04 ist162462 Exp $ -*- c++ -*-
/*
 * $Log: PipocaScanner.h,v $
 * Revision 1.1  2010-03-27 17:00:04  ist162462
 * Initial commit.
 *
 * Revision 1.10  2009/03/15 19:21:14  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.8  2009/03/02 20:16:14  david
 * Corrected stupid recursion bug.
 *
 * Revision 1.7  2009/02/20 06:04:35  david
 * Pipoca in new version. The new CDK is independent from particular
 * tools (such as Flex and BYACC). Must adapt. This is the first version
 * and will probably change.
 *
 */
#ifndef __PIPOCASCANNER_H__
#define __PIPOCASCANNER_H__

#undef yyFlexLexer
#define yyFlexLexer PipocaScanner
#include <FlexLexer.h>

#endif
