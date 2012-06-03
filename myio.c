#include "myio.h"
#include "ngtemplate.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


#include "logging.h"

#ifdef USE_ZZIP
#include "zzip/lib.h"

static ZZIP_DIR *zipfile = NULL;
#endif

#ifdef USE_LIBZIP
#include "zip.h"

static struct zip *zipfile = NULL;

#endif

int io_init(const char *argv0)
{
#ifdef USE_ZZIP
    zzip_error_t err;
    zipfile = zzip_dir_open(argv0, &err);
#endif
#ifdef USE_LIBZIP
    int err;
    zipfile = zip_open(argv0, ZIP_CHECKCONS, &err);
#endif
    if (!zipfile) {
        pid_t pid = getpid();
        char b1[64];
        char b2[1024];
        int k;
        sprintf(b1, "/proc/%i/exe", pid);
        if ((k = readlink(b1, b2, 1024)) == -1) {
            fprintf(stderr, "readlink on %s failed\n", b1);
        } else {
            b2[k] = 0;
#ifdef USE_ZZIP
            zipfile = zzip_dir_open(b2, &err);
#endif
#ifdef USE_LIBZIP
            zipfile = zip_open(b2, ZIP_CHECKCONS, &err);
#endif
        }
    }
    if (!zipfile)
    {
        fprintf(stderr, "Error: failed to open zipfile");
        return 1;
    }
    else
        DEBUG("Loaded zip data from %s\n", argv0);
    return 0;
}

void io_quit(void)
{
    if (zipfile)
#ifdef USE_ZZIP
        zzip_dir_close(zipfile);
#endif
#ifdef USE_LIBZIP
        zip_close(zipfile);
#endif
    zipfile = NULL;
}

static char* load_file_from_zip(const char *filename)
{
    if (!zipfile)
    {
        fprintf(stderr, "Error: %s\n", "Zipfile is invalid");
        return NULL;
    }

#ifdef USE_ZZIP
    ZZIP_FILE *file = zzip_file_open(zipfile, filename, 0);
#endif
#ifdef USE_LIBZIP
    struct zip_file *file = zip_fopen(zipfile, filename, 0);
#endif
    if (!file)
    {
        fprintf(stderr, "Error: File not found %s\n", filename);
        return NULL;
    }
#ifdef USE_ZZIP
    ZZIP_STAT stat;
    zzip_file_stat(file, &stat);
    size_t len = stat.st_size;
#endif
#ifdef USE_LIBZIP
    struct zip_stat stat;
    zip_stat(zipfile, filename, 0, &stat);
    size_t len = stat.size;
#endif
    
    char *buff = malloc(len + 1);
    memset(buff, 0, len + 1);
#ifdef USE_ZZIP
    zzip_file_read(file, buff, len);
    zzip_file_close(file);
#endif
#ifdef USE_LIBZIP
    zip_fread(file, buff, len);
    zip_fclose(file);
#endif
    DEBUG("loaded file: %s [%i bytes]\n", filename, len);

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


char* load_template_string(const char *name)
{
    char *realname = malloc(strlen(name) + 6);
    sprintf(realname, "%s.tmpl", name);
    return load_file(realname);
}

