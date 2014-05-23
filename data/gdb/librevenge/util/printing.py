# -*- tab-width: 4; indent-tabs-mode: nil; py-indent-offset: 4 -*-
# librevenge
# Version: MPL 2.0 / LGPLv2.1+
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Alternatively, the contents of this file may be used under the terms
# of the GNU Lesser General Public License Version 2.1 or later
# (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
# applicable instead of those above.
#

# Printer interface adaptor.

from collections import Mapping
import gdb
import re

from compatibility import use_gdb_printing

class SimplePrinter(object):

    def __init__(self, name, function):
        self.name = name
        self.function = function
        self.enabled = True

    def invoke(self, val):
        if not self.enabled:
            return None
        return self.function(self.name, val)

class NameLookup(Mapping):

    def __init__(self):
        self.map = {}
        self.name_regex = re.compile('^([\w:]+)(<.*>)?')

    def add(self, name, printer):
        self.map[name] = printer

    def __len__(self):
        return len(self.map)

    def __getitem__(self, type):
        typename = self._basic_type(type)
        if typename and typename in self.map:
            return self.map[typename]
        return None

    def __iter__(self):
        return self.map

    def _basic_type(self, type):
        basic_type = self.basic_type(type)
        if basic_type:
            match = self.name_regex.match(basic_type)
            if match:
                return match.group(1)
        return None

    @staticmethod
    def basic_type(type):
        if type.code == gdb.TYPE_CODE_REF:
            type = type.target()
        type = type.unqualified().strip_typedefs()
        return type.tag

class FunctionLookup(Mapping):

    def __init__(self):
        self.map = {}

    def add(self, test, printer):
        self.map[test] = printer

    def __len__(self):
        return len(self.map)

    def __getitem__(self, type):
        for (test, printer) in self.map.iteritems():
            if test(type):
                return printer
        return None

    def __iter__(self):
        return self.map

class Printer(object):

    def __init__(self, name):
        self.name = name
        self.subprinters = []
        self.name_lookup = NameLookup()
        self.func_lookup = FunctionLookup()
        self.enabled = True

    def add(self, name, function, lookup = None):
        printer = SimplePrinter(name, function)
        self.subprinters.append(printer)
        if not lookup:
            self.name_lookup.add(name, printer)
        else:
            self.func_lookup.add(lookup, printer)


    def __call__(self, val):
        printer = self.name_lookup[val.type]
        if not printer:
            printer = self.func_lookup[val.type]

        if printer:
            return printer.invoke(val)
        return None

def register_pretty_printer(printer, obj):
    '''Registers printer with objfile'''

    if use_gdb_printing:
        gdb.printing.register_pretty_printer(obj, printer)
    else:
        if obj is None:
            obj = gdb
        obj.pretty_printers.append(printer)

# vim:set filetype=python shiftwidth=4 softtabstop=4 expandtab:
