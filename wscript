####
# Copyright by Mariano Iglesias
# See contributors list in README.md
#
# See license text in LICENSE file
####

import Options, Utils
from os import unlink, symlink, chdir, environ
from os.path import exists

srcdir = "."
blddir = "build"
VERSION = "0.1.3"

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.add_option('--debug', action='store_true', help='Run tests with nodeunit_g')
  opt.add_option('--warn', action='store_true', help='Enable extra -W* compiler flags')

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  
  # Enables all the warnings that are easy to avoid
  conf.env.append_unique('CXXFLAGS', ["-Wall"])
  if Options.options.warn:
    # Extra warnings
    conf.env.append_unique('CXXFLAGS', ["-Wextra"])
    # Extra warnings, gcc 4.4
    conf.env.append_unique('CXXFLAGS', ["-Wconversion", "-Wshadow", "-Wsign-conversion", "-Wunreachable-code", "-Wredundant-decls", "-Wcast-qual"])

  oci_include = environ.get("OCI_INCLUDE_DIR", "/opt/instantclient/sdk/include")
  if oci_include:
      conf.env.append_unique('CXXFLAGS', [ '-I' + oci_include ])

  oci_lib = environ.get("OCI_LIB_DIR", "/opt/instantclient")
  if oci_lib:
      conf.env.append_unique('LINKFLAGS', [ '-L' + oci_lib ])

  conf.env.append_unique('LINKFLAGS', ['-locci', '-lclntsh', '-lnnz11'])
  conf.check(header_name="occi.h", errmsg="Missing include files for OCI", mandatory=True)
  conf.check_cxx(lib="occi", errmsg="Missing libocci", mandatory=True)

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "oracle_bindings"
  obj.source = "lib/node-db/binding.cc lib/node-db/connection.cc lib/node-db/exception.cc lib/node-db/query.cc lib/node-db/result.cc src/connection.cc src/oracle.cc src/query.cc src/result.cc src/oracle_bindings.cc"
  obj.includes = "lib/"

def test(tst):
  test_binary = 'nodeunit'
  if Options.options.debug:
    test_binary = 'nodeunit_g'
  
  Utils.exec_command(test_binary + ' tests.js')

def lint(lnt):
  # Bindings C++ source code
  print("Run CPPLint:")
  Utils.exec_command('cpplint --filter=-whitespace/line_length ./lib/node-db/*.h ./lib/node-db/*.cc ./src/*.h ./src/*.cc')
  # Bindings javascript code, and tools
  print("Run Nodelint for sources:")
  Utils.exec_command('nodelint ./package.json ./db-drizzle.js')
