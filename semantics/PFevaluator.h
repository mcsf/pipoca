// $Id: PFevaluator.h,v 1.1 2010-03-27 17:00:15 ist162462 Exp $
/*
 * $Log: PFevaluator.h,v $
 * Revision 1.1  2010-03-27 17:00:15  ist162462
 * Initial commit.
 *
 * Revision 1.4  2009/03/15 19:21:15  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.3  2009/03/02 17:40:21  david
 * Major changes: in addition to compiling with CDK4, Pipoca now has
 * its own namespace (pipoca). All classes are defined in it or in its
 * child namespaces. Added automatic node processing: the "nodes"
 * directory is scanned and nodes/all.h is built (contains both forward
 * declarations and include directives for all nodes -- in the appropriate
 * namespaces).
 *
 * Revision 1.1  2007/03/14 01:20:29  david
 * Changed compiler implementation to use evaluation strategy objects
 * instead of functions. These strategies are selected according to the
 * target selected from the command line options or from clues such as
 * the output file extension.
 */
#ifndef __PIPOCA_SEMANTICS_PFEVALUATOR_H__
#define __PIPOCA_SEMANTICS_PFEVALUATOR_H__

#include <iostream>
#include <cdk/semantics/Evaluator.h>

namespace cdk {
  class Compiler;
  namespace node {
    class Node;
  }
}

namespace pipoca {
  namespace semantics {

    class PFevaluator: public cdk::semantics::Evaluator {
      static PFevaluator _thisEvaluator;

    protected:
      PFevaluator(const char *target = "asm") :
        cdk::semantics::Evaluator(target) {
      }

    public:
      bool evaluate(cdk::Compiler *compiler);

    }; // class PFevaluator

  } // namespace semantics
} // namespace pipoca

#endif
