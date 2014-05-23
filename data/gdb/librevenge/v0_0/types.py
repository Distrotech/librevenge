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

import gdb

import librevenge.util.compatibility as compatibility
import librevenge.util.printing as printing

class RVNGBinaryDataPrinter:

    def __init__(self, typename, value):
        self.typename = typename
        self.value = value

        pimpl = self.value['m_binaryDataImpl']
        assert pimpl
        ptr = pimpl.dereference()['m_ptr']['px']
        assert ptr
        self.delegate = gdb.default_visualizer(ptr.dereference()['m_buf'])

    def children(self):
        return self.delegate.children()

    def to_string(self):
        return self.typename

    def display_hint(self):
        if hasattr(self.delegate, 'display_hint'):
            return self.delegate.display_hint()
        return None

class RVNGPropertyPrinter:

    def __init__(self, typename, value):
        self.typename = typename
        self.value = value
        self.delegate = None
        if self.value:
            self._find_delegate()

    def to_string(self):
        if self.delegate:
            return self.delegate.to_string()
        elif not self.value:
            return 'empty %s' % self.typename
        else: # let gdb show the pointer
            return None

    def _find_delegate(self):
        if compatibility.has_dynamic_type:
            dtype = self.value.dynamic_type
            if dtype != self.value.type:
                self.delegate = gdb.default_visualizer(self.value.cast(dtype))

class RVNGPropertyListPrinter:

    def __init__(self, typename, value):
        self.typename = typename
        self.value = value

        pimpl = self.value['m_impl']
        assert pimpl
        self.delegate = gdb.default_visualizer(pimpl.dereference()['m_map'])

    def children(self):
        return self.delegate.children()

    def to_string(self):
        return self.typename

    def display_hint(self):
        if hasattr(self.delegate, 'display_hint'):
            return self.delegate.display_hint()
        return None

class RVNGPropertyListVectorPrinter:

    def __init__(self, typename, value):
        self.typename = typename
        self.value = value

        pimpl = self.value['m_impl']
        assert pimpl
        self.delegate = gdb.default_visualizer(pimpl.dereference()['m_vector'])

    def children(self):
        return self.delegate.children()

    def to_string(self):
        return self.typename

    def display_hint(self):
        if hasattr(self.delegate, 'display_hint'):
            return self.delegate.display_hint()
        return None

class RVNGStringPrinter:

    def __init__(self, typename, value):
        self.typename = typename
        self.value = value

    def to_string(self):
        pimpl = self.value['m_stringImpl']
        assert pimpl
        return pimpl.dereference()['m_buf']

    def display_hint(self):
        return 'string'

class RVNGStringVectorPrinter:

    def __init__(self, typename, value):
        self.typename = typename
        self.value = value

        pimpl = self.value['m_pImpl']
        assert pimpl
        self.delegate = gdb.default_visualizer(pimpl.dereference()['m_strings'])

    def children(self):
        return self.delegate.children()

    def to_string(self):
        return self.typename

    def display_hint(self):
        if hasattr(self.delegate, 'display_hint'):
            return self.delegate.display_hint()
        return None

# helper printers

class RVNGPropertyListElementPrinter:

    def __init__(self, typename, value):
        self.typename = typename
        self.value = value

        prop = self.value['m_prop']
        if prop:
            assert prop
            self.delegate = gdb.default_visualizer(prop.dereference())
        else:
            vec = self.value['m_vec']
            assert vec
            self.delegate = gdb.default_visualizer(vec.dereference())

    def children(self):
        if hasattr(self.delegate, 'children'):
            return self.delegate.children()
        return iter([])

    def to_string(self):
        return self.delegate.to_string()

    def display_hint(self):
        if hasattr(self.delegate, 'display_hint'):
            return self.delegate.display_hint()
        return None

class RVNGStringPropertyPrinter:

    def __init__(self, typename, value):
        self.value = value

    def to_string(self):
        return self.value['m_str']

class RVNGBinaryDataPropertyPrinter:

    def __init__(self, typename, value):
        self.value = value

    def to_string(self):
        return self.value['m_data']

class RVNGBoolPropertyPrinter:

    def __init__(self, typename, value):
        self.value = value

    def to_string(self):
        if self.value['m_val'] == 0:
            return False
        return True

class RVNGIntPropertyPrinter:

    def __init__(self, typename, value):
        self.value = value

    def to_string(self):
        return self.value['m_val']

class RVNGDoublePropertyPrinter:

    def __init__(self, typename, value, unit=None):
        self.value = value
        self.unit = unit

    def to_string(self):
        if self.unit():
            return '%s %s' % (self.value['m_val'], self.unit())
        else:
            return self.value['m_val']

printer = None

def build_pretty_printers():
    global printer

    if printer != None:
        return

    printer = printing.Printer('librevenge')

    printer.add('librevenge::RVNGBinaryData', RVNGBinaryDataPrinter)
    printer.add('librevenge::RVNGProperty', RVNGPropertyPrinter)
    printer.add('librevenge::RVNGPropertyList', RVNGPropertyListPrinter)
    printer.add('librevenge::RVNGPropertyListVector', RVNGPropertyListVectorPrinter)
    printer.add('librevenge::RVNGString', RVNGStringPrinter)
    printer.add('librevenge::RVNGStringVector', RVNGStringVectorPrinter)

    # helpers
    printer.add('librevenge::RVNGPropertyListElement', RVNGPropertyListElementPrinter)
    printer.add('librevenge::RVNGStringProperty', RVNGStringPropertyPrinter)
    printer.add('librevenge::RVNGBinaryDataProperty', RVNGBinaryDataPropertyPrinter)
    printer.add('librevenge::RVNGBoolProperty', RVNGBoolPropertyPrinter)
    printer.add('librevenge::RVNGIntProperty', RVNGIntPropertyPrinter)
    printer.add('librevenge::RVNGDoubleProperty', RVNGDoublePropertyPrinter)
    printer.add('librevenge::RVNGGenericProperty', RVNGDoublePropertyPrinter)
    printer.add('librevenge::RVNGInchProperty', lambda t, v: RVNGDoublePropertyPrinter(t, v, 'in'))
    printer.add('librevenge::RVNGPercentProperty', lambda t, v: RVNGDoublePropertyPrinter(t, v, '%'))
    printer.add('librevenge::RVNGPointProperty', lambda t, v: RVNGDoublePropertyPrinter(t, v, 'pt'))
    printer.add('librevenge::RVNGTwipProperty', lambda t, v: RVNGDoublePropertyPrinter(t, w, '*'))

def register_pretty_printers(obj):
    printing.register_pretty_printer(printer, obj)

build_pretty_printers()

# vim:set filetype=python shiftwidth=4 softtabstop=4 expandtab:
