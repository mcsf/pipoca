// $Id: ProgramNode.h,v 1.1 2010-03-27 17:00:11 ist162462 Exp $ -*- c++ -*-
/*
 * $Log: ProgramNode.h,v $
 * Revision 1.1  2010-03-27 17:00:11  ist162462
 * Initial commit.
 *
 * Revision 1.5  2009/03/15 19:21:15  david
 * First public revision of the CDK4-based Pipoca compiler.
 * Revision logs were cleaned.
 *
 * Revision 1.4  2009/03/02 17:40:22  david
 * Major changes: in addition to compiling with CDK4, Pipoca now has
 * its own namespace (pipoca). All classes are defined in it or in its
 * child namespaces. Added automatic node processing: the "nodes"
 * directory is scanned and nodes/all.h is built (contains both forward
 * declarations and include directives for all nodes -- in the appropriate
 * namespaces).
 *
 * Revision 1.3  2008/03/01 18:32:50  david
 * Pipoca for CDK3 (first version).
 *
 * Revision 1.2  2007/03/13 00:26:28  david
 * This version of the Pipoca compiler uses the new CDK2 library.
 * All nodes and visitors now use non-overloaded visitor hooks. The Pipoca
 * compiler now implements the abstract factory for scanners and compilers.
 * A separate evaluator is no longer needed since the compiler implementation
 * (CompilerImpl) now provides a better encapsulation.
 *
 */
#ifndef __PIPOCA_PROGRAMNODE_H__
#define __PIPOCA_PROGRAMNODE_H__

#include <cdk/nodes/Node.h>

namespace pipoca {
  namespace node {

    //!
    //! Class for describing program nodes.
    //!
    class ProgramNode: public cdk::node::Node {
      cdk::node::Node *_statements;

    public:
      inline ProgramNode(int lineno, cdk::node::Node *statements) :
        cdk::node::Node(lineno), _statements(statements) {
      }

    public:
      inline cdk::node::Node *statements() {
        return _statements;
      }

    public:
      const char *name() const {
        return "ProgramNode";
      }

      void accept(SemanticProcessor *sp, int level) {
        sp->processProgramNode(this, level);
      }

    };

  } // namespace node
} // namespace pipoca

#endif
