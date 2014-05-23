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

import librevenge.util.printing as printing

printer = None

def build_pretty_printers():
    global printer

    if printer != None:
        return

    printer = printing.Printer('librevenge-stream')

def register_pretty_printers(obj):
    printing.register_pretty_printer(printer, obj)

build_pretty_printers()

# vim:set filetype=python shiftwidth=4 softtabstop=4 expandtab:
