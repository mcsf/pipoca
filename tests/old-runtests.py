#!/usr/bin/env python

import os, sys
import glob, re
import subprocess
from optparse import OptionParser

import pipoca
from pipoca import term

files = []

def exec_out(args, input):
	try:
		err = subprocess.PIPE
		p = subprocess.Popen(args, stdin=err, stdout=err, stderr=err)
		return p.communicate(input)
	except subprocess.CalledProcessError, err:
		raise err
		
class TestFail(Exception):
	pass

def test(bin, src):	
	bl = re.findall("{\s*(in|out)\s*([^}]*)\s*}", src)	
	m = {}
	
	for i in bl:
		m[i[0]] = i[1]
	
	try:
		input = m["in"]
		if not input.endswith("\n"):
			input += "\n"
	except KeyError:
		input = ""
		#raise Exception("test input not found")

	try:
		out = m["out"]
	except KeyError:
		out = ""
		#raise Exception("test output not found")
	
	p_out = exec_out([os.path.join(os.getcwd(), bin)], input)
	
	if options.output:
		print "out:", p_out[0].strip(), "exp:", out.strip(),
	
	if p_out[0].strip() != out.strip():
		raise TestFail
	
def test_pass():
	if options.verbose:
		print term.GREEN + "pass" + term.RESET
	
def test_fail():
	global n_fail
	n_fail += 1
	if options.verbose:
		print term.RED + "fail" + term.RESET

def check_args():
	usage = "usage: %prog [options] [pattern]"
	parser = OptionParser(usage=usage, version="%prog 0.1")

	parser.add_option("-o", "--output", dest="output",
	 action="store_true", help="show exampls output")

	parser.add_option("-e", "--errors", dest="showErrors",
	 action="store_true", help="outputs compile diagnostics")

	parser.add_option("-s", "--syntax", dest="syntaxOnly",
	 action="store_true", help="checks the syntax only")
	
	parser.add_option("-q", "--quiet", dest="verbose",
	 action="store_false", help="only outputs the summary")

	global options, args
	(options, args) = parser.parse_args()
	
	if options.verbose is None:
		options.verbose = True
	
	pipoca.showErrors = options.showErrors


def get_tests():
	pattern = "**.pip"
	
	if os.getcwd() == sys.path[0]:
		pattern = "**/*.pip"
	
	if len(args) > 0:
		pattern = args[0]
		
	pattern = os.path.join(os.getcwd(), pattern)
	files = glob.iglob(pattern)
	return sorted(files)
	
def run_tests(files):
	global n_fail
	n_fail = 0
	
	for file in files:
	
		if options.verbose:
			filter = os.path.split(file)[1]
			print "%s/%s" % (files.index(file)+1, len(files)) + term.RESET + ":",
			print "'" + term.YELLOW + "%s" % (filter) + term.RESET + "',",
					
		try:					
			run_test(file)

		except TestFail:
			test_fail()

		except Exception, e:
			pipoca.error(e)

	num = len(files)
	print term.YELLOW + "summary:" + term.RESET,
	print str(num-n_fail) + " tests" + term.GREEN + " passed" + term.RESET,
	print term.YELLOW + "/" + term.RESET,
	print str(n_fail) + " tests" + term.RED + " failed" + term.RESET,

def run_test(file):
	try:
		src = open(file).read()
		ex_fail = re.search("{\s*fail\s*}", src)
		
		bin = pipoca.compile_and_link(file, options.syntaxOnly)
		
		if options.syntaxOnly:
			test_pass()
			return
		
		test(bin, src)
		
		if ex_fail:
			test_fail()
		else:
			test_pass()
	
	except pipoca.CompileException:
		if ex_fail:
			test_pass()
		else:
			test_fail()

	except pipoca.AssembleException:
		if options.syntaxOnly:
			test_pass()
		else:
			test_fail()
			
	finally:
		path = os.path.splitext(file)[0]
		pipoca.remove_file(path)

def main():
	check_args()
	files = get_tests()
	run_tests(files)

if __name__ == '__main__':
	main()