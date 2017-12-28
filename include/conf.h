#ifndef	_CONF_H
#define	_CONF_H

#include <stdlib.h>
#include <string.h>
#include "log.h"

extern "C" {
#include "ini_config.h"
}

int conf_init(const char *fpath, const char *fname, 
        struct collection_item **config);

int conf_free(struct collection_item *config);

int conf_int(struct collection_item *config,
        const char* section, const char *name, 
        int def, int *value);

int conf_uint(struct collection_item *config,
        const char* section, const char *name, 
        uint32_t def, uint32_t *value);

int conf_double(struct collection_item *config,
        const char* section, const char *name, 
        double def, double *value);

int conf_string(struct collection_item *config,
        const char* section, const char *name, 
        const char *def, char *s, uint32_t len);

int conf_long_array(struct collection_item *config,
        const char* section, const char *name, 
        long **value, int *size);

int conf_string_array(struct collection_item *config,
        const char* section, const char *name, 
        char ***value, int *size);
#endif
