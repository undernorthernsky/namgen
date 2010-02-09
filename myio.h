#ifndef MYIO_H
#define MYIO_H

struct ngt_template_tag;

extern char* load_file(const char *filename);
extern int   load_template(struct ngt_template_tag *tpl, const char *filename);

extern char* load_template_string(const char* name);

extern int  io_init(const char *argv0);
extern void io_quit(void);

#endif


