// $Id: Symbol.cpp,v 1.1 2010-03-27 17:00:15 ist162462 Exp $
/*
 * $Log: Symbol.cpp,v $
 * Revision 1.1  2010-03-27 17:00:15  ist162462
 * Initial commit.
 *
 * Revision 1.2  2009/03/15 19:21:15  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.1  2009/03/02 17:40:21  david
 * Major changes: in addition to compiling with CDK4, Pipoca now has
 * its own namespace (pipoca). All classes are defined in it or in its
 * child namespaces. Added automatic node processing: the "nodes"
 * directory is scanned and nodes/all.h is built (contains both forward
 * declarations and include directives for all nodes -- in the appropriate
 * namespaces).
 *
 * Revision 1.1  2009/03/01 20:40:34  david
 * Class Symbol is now a part of Pipoca.
 *
 */
#include "semantics/Symbol.h"
