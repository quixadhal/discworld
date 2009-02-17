#!/usr/bin/python

import MySQLdb
import string
import types
import SocketServer
import sys
import _mysql

class InputString:
   def __init__(self, str, pos):
      self.pos = pos
      self.str = str
   def current(self):
      return self.str[self.pos]
   def forwards(self):
      self.pos = self.pos + 1
   def backwards(self):
      self.pos = self.pos + 1
   def eos(self):
      return self.pos >= len(self.str)

class LPCRequest(SocketServer.StreamRequestHandler):
   def handle(self):
      #
      # Ok, I think what we do here is read the stream for a while...
      #
      while 1:
         fluff = self.rfile.readline()
         if len(fluff) > 5:
            fluff = python_fromlpc(fluff)
            try:
               bing = doquery(self.server.connection, fluff[3], fluff[1], fluff[2], fluff[4])
               results = bing.fetchall()
               self.wfile.write(makelpcstring_type([fluff[0], 0, results]))
               bing.close()
            except IOError, (errno, strerror):
               self.wfile.write(makelpcstring_type([fluff[0], 1, "I/O error(%s): %s" % (errno, strerror)]))
            #except SocketServer.ProgrammingError:
            #   print "Programming Error."
               #self.wfile.write(makelpcstring_type([fluff[0], 1, "Programming error: "]))
               #self.wfile.write(makelpcstring_type([fluff[0], 1, "Programming error: " + detail]))
            except TypeError:
               self.wfile.write(makelpcstring_type([fluff[0], 2, "Type error."]))
            except _mysql.OperationalError, (errno, details):
               self.wfile.write(makelpcstring_type([fluff[0], errno, details]))
            except:
               self.wfile.write(makelpcstring_type([fluff[0], 1, "Another error." + str(sys.exc_info()[0])]))

class LPCServer(SocketServer.TCPServer):
   def __init__(self, addr, type, conn):
      SocketServer.TCPServer.__init__(self, addr, type)
      self.connection = conn

class CurrentConnection:
   db = 'errors'
   user = 'atuin'
   passwd = ''
   connection = MySQLdb.connect(host='127.0.0.1', user='atuin', db='errors', port=3306, passwd='')

def connectTo(connect, db, user, passwd=''):
   if connect.db != db or connect.user != user or connect.passwd != passwd:
      connect.connection.close()
      connect.connection = MySQLdb.connect(host='127.0.0.1', user=user, db=db, port=3306, passwd=passwd)
      connect.passwd = passwd
      connect.user = user
      connect.db = db

def doquery(conn, db, user, passwd, query):
   connectTo(conn, db, user, passwd)
   fluff = MySQLdb.DictCursor(conn.connection)
   fluff.execute(query)
   return fluff

def makelpcstring_float(bing):
   return fpformat.sci(bing, 10)

def makelpcstring_int(bing):
   return str(bing)

def makelpcstring_string(bing):
   bing = string.replace(bing, '\r\n', '\r')
   return '"' + string.replace(bing, '"', '\\"') + '"'

def makelpcstring_dict(bing):
   ret = '(['
   for kw in bing.keys():
      ret = ret + makelpcstring_type(kw)
      ret = ret + ':'
      ret = ret + makelpcstring_type(bing[kw])
      ret = ret + ','
   ret = ret + '])'
   return ret;

def makelpcstring_type(row):
   if type(row) is types.IntType or type(row) is types.LongType:
      ret = makelpcstring_int(row)
   elif type(row) is types.FloatType:
      ret = makelpcstring_float(row)
   elif type(row) is types.StringType:
      ret = makelpcstring_string(row)
   elif type(row) is types.ListType:
      ret = makelpcstring_array(row)
   elif type(row) is types.DictType:
      ret = makelpcstring_dict(row)
   else:
      ret = '"broken"'
      print 'Broken type', type(row), types.IntType, 
   return ret

def makelpcstring_array(arr):
   ret = '({'
   for row in arr:
      ret = ret + makelpcstring_type(row)
      ret = ret + ','
   ret = ret + '})'
   return ret;

def python_fromlpc(input):
   "Changes the input string into a real python type."
   fluff = InputString(input, 0)
   return python_fromlpctype(fluff)

def pythonnum_fromlpc(input):
   start = input.pos
   input.forwards()
   while not input.eos() and input.current() >= '0' and input.current() <= '9' or input.current() == '.':
      input.forwards()
   input.backwards()
   return string.atoi(input.str[start:input.pos - 1])

def pythonstring_fromlpc(input):
   start = input.pos
   skip = 0
   while not input.eos() and (input.current() != '"' or skip):
      if input.current() == '\\':
         skip = 1
      else:
         skip = 0
      input.forwards()
   ret = input.str[start:input.pos]
   ret = string.replace(ret, '\\n', '\n')
   ret = string.replace(ret, '\\"', '"')
   input.forwards()
   return ret

def pythonlist_fromlpc(input):
   ret = []
   while input.current() != '}':
      ret.append(python_fromlpctype(input))
      if input.current() == ',':
         input.forwards()
   if input.current() == '}':
      input.forwards()
      input.forwards()
   return ret

def pythondict_fromlpc(input):
   ret = {}
   while input.current() != ']':
      key = python_fromlpctype(input)
      if input.current() == ':':
         input.forwards()
      ret[key] = python_fromlpctype(input)
      if input.current() == ',':
         input.forwards()
   if input.current() == ']':
      input.forwards()
      input.forwards()
   return ret

def python_fromlpctype(input):
   if (input.current() >= '0' and input.current() <= '9') or input.current() == '-':
      return pythonnum_fromlpc(input)
   elif input.current() == '(':
      input.forwards()
      if input.current() == '[':
         input.forwards()
         return pythondict_fromlpc(input)
      elif input.current() == '{':
         input.forwards()
         return pythonlist_fromlpc(input)
   elif input.current() == '\"':
      input.forwards()
      return pythonstring_fromlpc(input)
   return None

#result = doquery('errors', 'atuin', '', 'select * from comments where commenter=\'pinkfish\'');

#fluff = makelpcstring_array(result.fetchall())

#print python_fromlpc(fluff)

#
# Now we go into a loop waiting for someone to say something
#
while 1:
   fluff = LPCServer(('localhost',7865), LPCRequest, CurrentConnection())
   fluff.serve_forever()
