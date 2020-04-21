import os
import sipconfig

build_file = "corelib.sbf"

config = sipconfig.Configuration()

os.system("%s -c %s -b %s/%s %s" % (config.sip_bin, "src", "src", build_file, "corelib2.sip"))

makefile = sipconfig.SIPModuleMakefile(config, build_file, dir="src")
makefile.extra_include_dirs = ["../../inc", "../../../sfw/include"]
makefile.extra_lib_dirs = ["../../lib", "../../../sfw/lib"]
makefile.extra_libs = ["core2++", "gmp", "mpfr"]
makefile.generate()
