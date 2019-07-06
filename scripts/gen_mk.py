import getopt
import sys
import string
import os

verbose = 0

def debug(*msg):
    if verbose == 1:
    	print string.join(msg)

def generate_env(output):
    f = open(output, "w+")
    lib_list = []

    f.write("# Automatically generated file: DO NOT EDIT.\n")
    f.write("\n")

    top_dir = os.path.abspath("./")
    f.write("TOP_DIR = %s" %top_dir)
    f.write("\n")
    f.write("KERNEL_DIR = %s/kernel" %top_dir)
    f.write("\n")
    f.write("TARGET_DIR = %s/rootfs" %top_dir)

    f.close()


if __name__ == '__main__':
    opts = ""
    output = ""
    longopts = ["env"]
    
    try:
        optlist, args = getopt.getopt(sys.argv[1:], opts, longopts)
    except getopt.GetoptError:
    	print("parsing error")
        sys.exit()
    
    for opt, arg in optlist:
  	if opt in ("", "--env"):
            generate_env("./mk/env.mk")
	else:
            print("unexpect parameter: %s" %opt)
            sys.exit()
