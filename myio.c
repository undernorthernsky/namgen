#include "myio.h"
#include "ngtemplate.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "logging.h"

#ifdef USE_ZIP
#include "zzip.h"

static ZZIP_DIR *zipfile = NULL;

extern int io_init(const char *argv0)
{
    zzip_error_t err;
    zipfile = zzip_dir_open(argv0, &err);
    if (!zipfile)
    {
        fprintf(stderr, "Error: failed to open zipfile");
        return 1;
    }
    else
        DEBUG("Loaded zip data from %s\n", argv0);
    return 0;
}

extern void io_quit(void)
{
    if (zipfile)
        zzip_dir_close(zipfile);
    zipfile = NULL;
}

static char* load_file_from_zip(const char *filename)
{
    if (!zipfile)
    {
        fprintf(stderr, "Error: %s\n", "Zipfile is invalid");
        return NULL;
    }

    ZZIP_FILE *file = zzip_file_open(zipfile, filename, 0);
    if (!file)
    {
        fprintf(stderr, "Error: File not found %s\n", filename);
        return NULL;
    }
    ZZIP_STAT stat;
    zzip_file_stat(file, &stat);
    size_t len = stat.st_size;
    
    char *buff = malloc(len + 1);
    memset(buff, 0, len + 1);
    zzip_file_read(file, buff, len);
    zzip_file_close(file);
    DEBUG("loaded file: %s [%i bytes]\n", filename, stat.st_size);

    return buff;
}

char* load_file(const char *filename)
{
    return load_file_from_zip(filename);
}

int load_template(ngt_template* tpl, const char *filename)
{
    char *buff = load_file_from_zip(filename);
    if (!buff)
        return -1;
    if (tpl->template)
        free(tpl->template);
    tpl->template = buff;
    return 0;
}

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern int io_init(const char *argv0)
{
    return 0;
}

extern void io_quit(void)
{
}

char* load_file(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "Error: File not found %s\n", filename);
        return NULL;
    }
    struct stat st;
    stat(filename, &st);
    
    size_t len = st.st_size;
    char *buff = malloc(len + 1);
    memset(buff, 0, len + 1);
    size_t got;
    if ((got = fread(buff, 1, len, f)) != len)
    {
        fprintf(stderr, "Error read %i bytes when expecting %i\n", got, len);
        free(buff);
        buff = NULL;
    }
    fclose(f);

    return buff;
}

int load_template(ngt_template* tpl, const char *filename)
{
    return ngt_load_from_filename(tpl, filename);
}

#endif

char* load_template_string(const char *name)
{
    char *realname = malloc(strlen(name) + 6);
    sprintf(realname, "%s.tmpl", name);
    return load_file(realname);
}


