python
import sys
sys.path.insert(0, '/ucrt64/share/gcc-13.2.0/python')
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)
end