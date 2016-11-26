#!/usr/bin/env python3
import os

root_dir = '../..'

source_dir = root_dir + 'source'
def list_prj():
    return [d for d in os.listdir(source_dir) if os.path.isdir(source_dir + '/' + d)]

def list_src(prj_dir):
    src_files = []
    for i in os.walk(prj_dir):
        src_files.extend([prj_dir + '/' + f for f in i[2] if f.endswith('.c') or f.endswith('.cpp')])

    return src_files

def generate_makefile(makefile_dir, source_files):
    if not os.path.exists(makefile_dir):
        os.mkdir(makefile_dir)
    elif not os.path.isdir(makefile_dir):
        print(makefile_dir + ' exists, but is not a directory')
        return

if __name__ == '__main__':
    projects = list_prj()
    for prj in projects:
        source_files = list_src(source_dir + prj)
        print(source_files)
        generate_makefile(prj, source_files)


