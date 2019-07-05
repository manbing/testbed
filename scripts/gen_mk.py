import getopt
import sys
import string

cross_compile = ""
openssl_path = ""
libnl_path = ""
sysroot_path = ""
include_path = ""
include_library = ""
install_prefix = ""
verbose = 0

def debug(*msg):
    if verbose == 1:
    	print string.join(msg)

def generate_tool_chain_cmake(output):
    f = open(output, "w+")
    lib_list = []

    f.write("# Automatically generated file: DO NOT EDIT.\n")
    f.write("\n")
    f.write("# this one is important\n")
    f.write("SET(CMAKE_SYSTEM_NAME Linux)\n")
    f.write("\n")
    f.write("# this one not so much\n")
    f.write("SET(CMAKE_SYSTEM_VERSION 1)\n")
    f.write("\n")
    f.write("# specify the cross compiler\n")
    f.write("SET(CMAKE_C_COMPILER %sgcc)\n" %cross_compile)
    f.write("SET(CMAKE_CXX_COMPILER %sg++)\n" %cross_compile)
    f.write("\n")
    f.write("SET(OPENSSL_SSL_LIBRARY %s/libssl.so)\n" %openssl_path)
    f.write("SET(OPENSSL_CRYPTO_LIBRARY %s/libcrypto.so)\n" %openssl_path)
    f.write("SET(OPENSSL_INCLUDE_DIR %s/include)\n" %openssl_path)
    f.write("\n")
    f.write("SET(ENV{PKG_CONFIG_PATH} %s)\n" %libnl_path)
    f.write("\n")
    f.write("# where is the target environment\n")
    f.write("SET(CMAKE_FIND_ROOT_PATH  %s)\n" %sysroot_path)
    f.write("\n") 
    f.write("# Included headers path\n")
    f.write("SET(INC_DIR ")
    f.write("%s/include " %libnl_path)
    for x in include_path:
    	f.write("%s " %x)
    f.write(")\n")
    f.write("include_directories(BEFORE ${INC_DIR})\n")
    f.write("\n")
    f.write("SET(CMAKE_INSTALL_PREFIX %s)\n" %install_prefix)
    f.write("\n")
    f.write("# Included libraries path\n")
    f.write("add_library(nl-genl-3 SHARED IMPORTED)\n")
    f.write("set_property(TARGET nl-genl-3 PROPERTY IMPORTED_LOCATION %s/lib/.libs/libnl-genl-3.so)\n" %libnl_path)
    f.write("add_library(nl-3 SHARED IMPORTED)\n")
    f.write("set_property(TARGET nl-3 PROPERTY IMPORTED_LOCATION %s/lib/.libs/libnl-3.so)\n" %libnl_path)
    for x in include_library:
    	name = x.split("/")[len(x.split("/"))-1]
        lib_list.append(name)
        f.write("add_library(%s SHARED IMPORTED)\n" %name)
        f.write("set_property(TARGET %s PROPERTY IMPORTED_LOCATION %s)\n" %(name, x))
    f.write("\n")
    f.write("SET(CMAKE_INNDA_EXTERNAL_LIB ")
    for x in lib_list:
        f.write("%s " %(x))
    f.write(")\n")
    f.write("\n")
    f.write("# search for programs in the build host directories\n")
    f.write("SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)\n")
    f.write("\n")
    f.write("# for libraries and headers in the target directories\n")
    f.write("SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)\n")
    f.write("SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)\n")

    f.close()


if __name__ == '__main__':
    opts = "o:vi:l:"
    output = ""
    longopts = ["output=", "cross-compile=", "openssl-path=", "libnl-path=", "sysroot-path=", "prefix=", "verbose", "include-path=", "include-library="]
    
    try:
        optlist, args = getopt.getopt(sys.argv[1:], opts, longopts)
    except getopt.GetoptError:
    	print("parsing error")
        sys.exit()
    
    for opt, arg in optlist:
  	if opt in ("-o", "--output"):
            output = arg 
  	elif opt in ("", "--cross-compile"):
            cross_compile = arg 
  	elif opt in ("", "--openssl-path"):
            openssl_path = arg 
  	elif opt in ("", "--libnl-path"):
            libnl_path = arg 
  	elif opt in ("", "--sysroot-path"):
            sysroot_path = arg 
  	elif opt in ("", "--prefix"):
            install_prefix = arg 
  	elif opt in ("-i", "--include-path"):
            include_path = arg.split(",") 
  	elif opt in ("-l", "--include-library"):
            include_library = arg.split(",") 
  	elif opt in ("-v", "--verbose"):
            verbose += 1
	else:
            print("unexpect parameter: %s" %opt)
            sys.exit()

    debug("output = %s" %output)
    debug("cross_compile = %s" %cross_compile)
    debug("openssl_path = %s" %openssl_path)
    debug("libnl_path = %s" %libnl_path)
    debug("sysroot_path = %s" %sysroot_path)
    debug("install_prefix = %s" %install_prefix)
    debug("include_path = %s" %include_path)
    debug("include_library = %s" %include_library)
    
    generate_tool_chain_cmake(output)
