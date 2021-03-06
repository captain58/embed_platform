#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import sys, os, json, glob, re
import shutil
from itertools import chain

def move_2boot_file(bin_output_path):
    tmp_path = os.path.join(bin_output_path, '2nd_boot')
    flag     = 0
    #mkdir temp
    if os.path.exists(tmp_path):
        shutil.rmtree(tmp_path)
    os.makedirs(tmp_path)
    #copy file to temp
    list = os.listdir(bin_output_path)
    for i in range(0,len(list)):
        filename = os.path.join(bin_output_path, list[i])
        if filename.find(".2boot") > 0  and filename.find("link.2boot.opts") < 0:
            shutil.move(filename, tmp_path)
            flag = 1

    if flag == 0 :
        shutil.rmtree(tmp_path)


def find_factory_bin(bin_output_path):
    bin_output_name = os.path.dirname(bin_output_path)
    fpath , fname = os.path.split(bin_output_name)
    bin_name = fname + ".all.bin"
    filename  = os.path.join(bin_output_path, bin_name)

    if os.path.exists(filename):
        return bin_name

    bin_name = fname + ".bin"
    filename  = os.path.join(bin_output_path, bin_name)

    if os.path.exists(filename):
        return bin_name

    return "null"

def find_ota_bin(bin_output_path):
    list = os.listdir(bin_output_path)
    for i in range(0,len(list)):
        filename = os.path.join(bin_output_path, list[i])
        if filename.find(".ota.bin") > 0:
            fpath , fname = os.path.split(filename)
            return fname

    for i in range(0,len(list)):
        filename = os.path.join(bin_output_path, list[i])
        if filename.find(".bin") > 0:
            fpath , fname = os.path.split(filename)
            return fname

    return "null"

def gen_readme_file(bin_output_path, ota_file, factory_file, version):
    readmename = os.path.join(bin_output_path, "readme.txt")
    ota_blank = " "
    fac_blank = " "
    for i in range(0, len(ota_file)):
        ota_blank = ota_blank + " "
    for i in range(0, len(factory_file)):
        fac_blank = fac_blank + " "

    if os.path.exists(readmename):
        os.remove(readmename)
    f=file(readmename, "w")
    f.write("version : " + version + " \r\n")
    f.write(ota_file + fac_blank +" : for ota update. ")
    f.write(" ??????OTA????????? \r\n")
    f.write(factory_file + ota_blank + " : for direct burning into flash memory. ")
    f.write(" ??????flash?????????\r\n")
    f.close()

def find_version(config_file):
    with open(config_file, "r") as f:
        config_mk_str = f.read()
        find_str = re.search( r'DSYSINFO_APP_VERSION=.*', config_mk_str, re.M|re.I)
        version = find_str.group()
        version = version.replace('DSYSINFO_APP_VERSION=', '')
        version = version.replace('\\"', '')
    f.close()
    return version

def main():
    bin_output_path = sys.argv[1]
    config_file     = sys.argv[2]
    move_2boot_file(bin_output_path)
    factory_file = find_factory_bin(bin_output_path)
    ota_file = find_ota_bin(bin_output_path)

    version = find_version(config_file)
    gen_readme_file(bin_output_path, ota_file, factory_file, version)

if __name__ == "__main__":
    main()

