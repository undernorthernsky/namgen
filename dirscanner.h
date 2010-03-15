#ifndef DIRSCANNER_H
#define DIRSCANNER_H

extern int module_count;
extern int program_count;
extern int library_count;

int iterate_directories(char * dirpath, int work_mode);

#endif
