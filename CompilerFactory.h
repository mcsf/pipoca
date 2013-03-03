// $Id: CompilerFactory.h,v 1.1 2010-03-27 17:00:04 ist162462 Exp $ -*- c++ -*-
/*
 * $Log: CompilerFactory.h,v $
 * Revision 1.1  2010-03-27 17:00:04  ist162462
 * Initial commit.
 *
 * Revision 1.2  2009/03/15 19:21:14  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.1  2009/03/02 17:40:20  david
 * Major changes: in addition to compiling with CDK4, Pipoca now has
 * its own namespace (pipoca). All classes are defined in it or in its
 * child namespaces. Added automatic node processing: the "nodes"
 * directory is scanned and nodes/all.h is built (contains both forward
 * declarations and include directives for all nodes -- in the appropriate
 * namespaces).
 *
 * Revision 1.3  2009/02/20 06:04:35  david
 * Pipoca in new version. The new CDK is independent from particular
 * tools (such as Flex and BYACC). Must adapt. This is the first version
 * and will probably change.
 *
 * Revision 1.1  2007/03/14 01:20:20  david
 * Changed compiler implementation to use evaluation strategy objects
 * instead of functions. These strategies are selected according to the
 * target selected from the command line options or from clues such as
 * the output file extension.
 *
 * Revision 1.1  2007/03/13 00:26:20  david
 * This version of the Pipoca compiler uses the new CDK2 library.
 * All nodes and visitors now use non-overloaded visitor hooks. The Pipoca
 * compiler now implements the abstract factory for scanners and compilers.
 *
 */
#ifndef __PIPOCA_PIPOCACOMPILERFACTORY_H__
#define __PIPOCA_PIPOCACOMPILERFACTORY_H__

#include <cdk/CompilerFactory.h>

class FlexLexer;
namespace cdk {
  class Compiler;
}

namespace pipoca {

  /**
   * This class implements the compiler factory for the Pipoca compiler.
   */
  class CompilerFactory: public cdk::CompilerFactory {
    /**
     * This object is automatically registered by the constructor in the
     * superclass' language registry.
     */
    static CompilerFactory _thisFactory;

  protected:
    /**
     * @param language name of the language handled by this factory (see .cpp file)
     */
CompilerFactory  (const char *language) :
  cdk::CompilerFactory(language) {
  }

protected:
  /**
   * Create a scanner object for the Pipoca language.
   * This method is now private and is called only from the compiler creation
   * method.
   * @param name name of the input file (for debug only)
   * @return scanner object pointer
   * @see createCompiler
   */
  virtual cdk::syntax::Parser *createParser(const char *name);

public:
  /**
   * Create a compiler object for a given language.
   * @param name name of the language handled by the compiler
   * @return compiler object pointer
   */
  virtual cdk::Compiler *createCompiler(const char *name);

}; // class CompilerFactory

} // namespace pipoca

#endif
