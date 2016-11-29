#!/usr/bin/env python3
import os

root_dir = '../..'

source_dir = root_dir + 'source'
def list_projects():
    candidates = [d for d in os.listdir('.') if os.path.isdir(d)]
    return [d for d in candidates if os.path.isfile(d + '/project.cfg')]


def load_project_cfg(project):
    cfg_file = project + '/project.cfg'
    prj_cfg = {}
    with open(cfg_file) as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith('#'):
                k, v = line.split('=')
                prj_cfg[k.strip()] = v.strip()

    if not prj_cfg['PRJ_NAME']:
        prj_cfg['PRJ_NAME'] = project

    if not prj_cfg['SRC_FILE_TYPES']:
        prj_cfg['SRC_FILE_TYPES'] = 'c,cpp'

    if not prj_cfg['OBJ_DIR']:
        prj_cfg['OBJ_DIR'] = './Debug'

    prj_cfg['MAKEFILE_DIR'] = project
    return prj_cfg


def generate_makefile(prj_cfg, code_bit):
    prj_name = prj_cfg['PRJ_NAME']
    prj_root = prj_cfg['PRJ_ROOT_DIR']
    defines = [d for d in prj_cfg['DEFINES'].split(':') if d]
    inc_paths = [p for p in prj_cfg['INC_PATHS'].split(':') if p]
    depend_libs = [lib for lib in prj_cfg['DEPEND_LIBS'].split(':') if lib]

    type_filter = tuple(prj_cfg['SRC_FILE_TYPES'].split(','))
    if not prj_name:
        print('no PRJ_NAME in the project configuration')
        exit(-1)

    make_file = '%s/Makefile.%d' %(prj_name, code_bit)
    prj_dir = '/'.join((prj_cfg['MAKEFILE_DIR'], prj_root, prj_name))
    obj_dir = '%s-%d' %(prj_cfg['OBJ_DIR'], code_bit)
    output_dir = '%s/%d' %(prj_cfg['OUTPUT_DIR'], code_bit)

    sources = []
    for entry in os.walk(prj_dir):
        if entry[2]:
            sources.extend(['/'.join((entry[0].lstrip(prj_cfg['MAKEFILE_DIR'] + '/'), f)).replace('\\', '/') for f in entry[2] if f.endswith(type_filter)])
    
    if not sources:
        print('no source codes found in %s' %os.path.abspath(prj_dir))
        exit(-1)

    with open(make_file, 'w') as f:
        if prj_cfg['PRJ_TYPE'] == 'bin':
            output_bin = True
            f.write('OUTPUT_FILENAME = %s\n' %prj_name)
        else:
            f.write('OUTPUT_FILENAME = lib%s.so\n' %prj_name)
            output_bin = False

        f.write('PRJ_ROOT_DIR = %s\n' %prj_root)
        f.write('OBJ_DIR = %s\n' %obj_dir)
        f.write('OUTPUT_DIR = %s\n' %output_dir)
        f.write('\n')

        if defines:
            f.write('DEFINES = \\\n')
            for d in defines[:-1]:
                f.write('\t-D%s\\\n' %d)
            f.write('\t-D%s\n' %defines[-1])
            f.write('\n')

        if inc_paths:
            f.write('INCLUDE_PATH = \\\n')
            for p in inc_paths[:-1]:
                f.write('\t-I%s\\\n' %p)
            f.write('\t-I%s\n' %inc_paths[-1])
            f.write('\n')

        f.write('COMMON_CX_FLAGS = -g -pipe -fpic -fPIC -fpie -ffunction-sections -funwind-tables\n')
        f.write('COMMON_CX_FLAGS += -fstack-protector -fno-short-enums -Wall -m%d -fdiagnostics-color=auto\n' %(code_bit))
        f.write('COMMON_CX_FLAGS += -O2 -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300\n')
        f.write('COMMON_CX_FLAGS += -fvisibility=hidden\n')
        f.write('\n')

        f.write('CC = gcc\n')
        f.write('CC_FLAGS = $(COMMON_CX_FLAGS) $(DEFINES)\n')
        f.write('\n')

        f.write('CXX = g++\n')
        f.write('CXX_FLAGS = $(COMMON_CX_FLAGS) -fno-exceptions -fno-rtti -Wno-psabi -std=c++11 -Wno-deprecated $(DEFINES)\n')
        f.write('\n')

        f.write('LINK = g++\n')
        if output_bin:
            f.write('LINK_FLAGS = -m%d -Xlinker -rpath-link $(OUTPUT_DIR)\n' %(code_bit))
        else:
            f.write('LINK_FLAGS = -m%d -shared -Wl,-soname,$(OUTPUT_FILENAME)\n' %(code_bit))
        f.write('\n')

        f.write('LIBS = -ldl -lm -lpthread\n')
        if depend_libs:
            f.write('LIBS += \\\n')
            for lib in depend_libs[:-1]:
                f.write('\t-L%s\\\n' %lib)
            f.write('\t-L%s\\\n' %depend_libs[-1])
            f.write('\n')

        f.write('TARGET = $(OUTPUT_DIR)/$(OUTPUT_FILENAME)\n')
        f.write('\n')
        f.write('DEL_FILE = rm -f\n')
        f.write('MKDIR = mkdir -p\n')
        f.write('\n')
        f.write('first: all\n')
        f.write('\n')

        objects = 'OBJECTS = \\\n'
        obj_files = []
        for src in sources[:-1]:
            obj_name = src.rsplit('/', 1)[1].split('.')[0] + '.o'
            obj_files.append(obj_name)
            objects += '\t$(OBJ_DIR)/%s\\\n' %obj_name

        src = sources[-1]
        obj_name = src.rsplit('/', 1)[1].split('.')[0] + '.o'
        obj_files.append(obj_name)
        objects += '\t$(OBJ_DIR)/%s\n' %obj_name

        f.write(objects)
        f.write('\n')
        f.write('$(OBJ_DIR) :\n\t$(MKDIR) $(OBJ_DIR)\n')
        f.write('\n')
        f.write('$(OUTPUT_DIR) :\n\t$(MKDIR) $(OUTPUT_DIR)\n')
        f.write('\n')
        f.write('all : $(OBJ_DIR) $(OUTPUT_DIR) $(TARGET)\n')
        f.write('$(TARGET) : $(OBJECTS)\n\t$(LINK) $(LINK_FLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)\n')
        f.write('\n')
        f.write('clean : \n\t-$(DEL_FILE) $(OBJECTS)\n\t-$(DEL_FILE) $(TARGET)\n')
        f.write('\n')
        f.write('rebuild : clean all\n')
        f.write('\n')

        for i in range(len(sources)):
            obj_file = obj_files[i]
            src = sources[i]
            if src.endswith('cpp'):
                compiler = 'CXX' 
                flags = 'CXX_FLAGS'
            else:
                compiler = 'CC'
                flags = 'CC_FLAGS'

            f.write('$(OBJ_DIR)/%s : %s\n\t$(%s) -c $(%s) $(INCLUDE_PATH) -o $(OBJ_DIR)/%s %s\n' 
                    %(obj_file, src, compiler, flags, obj_file, src))


if __name__ == '__main__':
    projects = list_projects()
    for project in projects:
        prj_cfg = load_project_cfg(project)
        generate_makefile(prj_cfg, 32)
        generate_makefile(prj_cfg, 64)

