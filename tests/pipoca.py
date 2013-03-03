#!/usr/bin/env python

import os, sys
import subprocess
from optparse import OptionParser

#
# CONFIG
#

home = os.path.expanduser("~")

compiler = "../pipoca"
assembler = "/usr/bin/yasm"
linker = "/usr/bin/ld"

rts_dirs = [
        '/usr/lib/',
        home + '/compiladores/root/usr/lib/',
        home + '/ist/com/root/usr/lib/',
        ]

lrts = "rts"

showErrors = True

#
# CODE
#

class term:
    GREEN 	= '\x1b[32m'
    YELLOW 	= '\x1b[33m'
    RED 	= '\x1b[31m'
    BLUE 	= '\x1b[34m'
    RESET 	= '\x1b[0m'

class CompileException(Exception):
	pass
	
class AssembleException(Exception):
	pass
	
class LinkException(Exception):
	pass	

def error(s):
	print term.RED + "error:" + term.RESET + " %s" % (s)

def remove_file(file):
	try:
		os.remove(file)
	except OSError:
		pass # raise Exception("could not remove '%s'" % (file))

def check_args():
	usage = "usage: %prog [options] example.pip -o example"
	parser = OptionParser(usage=usage, version="%prog 0.1")

	parser.add_option("-a", "--asm", dest="asm",
					  action="store_true", help="keeps the assembly code")

	parser.add_option("-o", "--output", dest="filename",
					  help="writes the program to prog", metavar="prog")

	parser.add_option("-r", "--run", dest="run", action="store_true",
					  help="runs the compiled program")

	global options, args
	(options, args) = parser.parse_args()

	if len(sys.argv) < 2 or len(args) < 1:
		parser.print_help()
		sys.exit()

def exec_wait(args):
	try:
		err = None if showErrors else subprocess.PIPE
		ret = subprocess.check_call(args, stderr=err, stdout=err)
	except subprocess.CalledProcessError:
		raise Exception("got an error running process '%s'" % (args[0]))

def compile(file):	
	# check for the program and the compiler
	if not os.path.isfile(file):
		raise Exception("file '%s' not found or does not exist" % (file))
	
	if not os.path.isfile(compiler):
		raise Exception("compiler '%s' not found or does not exist" % (compiler))
	
	# compile the program
	try:
		out = exec_wait([compiler, file])
	except Exception:
		raise CompileException("could not compile '%s'" % (file))

def assembly(asm):
	# check for the assembler
	if not os.path.isfile(assembler):
		raise Exception("assembler '%s' not found or does not exist" % (assembler))

	# check for assembly file
	if not os.path.isfile(asm):
		raise Exception("assembly file '%s' not found or does not exist" % (asm))
	
	# assembly it
	try:
		exec_wait([assembler, "-f", "elf", asm])
	except Exception:
		raise AssembleException("could not assemble '%s'" % (asm))

def link(obj, out):
	# check for the linker
	if not os.path.isfile(linker):
		raise Exception("linker '%s' not found or does not exist" % (linker))
	
	# check for the runtime lib
	for dir in rts_dirs:
		if os.path.isfile(dir + '/lib' + lrts + '.a'):
			rts = dir
			break
	else:
		raise LinkException("runtime lib '%s' not found or does not exist" % (lrts))
		
	# link it
	try:
		try:
			obj = os.path.split(obj)[1]
		except:
			pass
		
		exec_wait([linker, obj, "-L", rts, "-l", lrts, "-o", out])
	except:
		raise LinkException("could not link '%s'" % (obj))
	finally:
		return obj

def compile_and_link(file, syntaxOnly):
	path = os.path.splitext(file)[0]
	asm = path+".asm"
	obj = path+".o"
	
	try:
		compile(file)
		assembly(asm)
		obj = link(obj, path)
	except LinkException, msg:
		error(msg)
		sys.exit(1)
	finally:
		try:
			if not options.asm:
				remove_file(asm)
		except Exception:
			remove_file(asm)
			pass
		remove_file(obj)

	return path

def main():
	check_args()
	
	file = args[0]
	
	try:
		path = compile_and_link(file, False)
		
		if options.run:
			try:
				exec_wait(["./"+path])
			except KeyboardInterrupt:
				raise Exception("user interruped process")
			
	except Exception, msg:
		error(msg)
		sys.exit(1)		

if __name__ == '__main__':
	main()
