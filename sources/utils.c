/**
 * (C)2024 aks
 * https://github.com/akscf/
 **/
#include "mod_openai_tts.h"

extern globals_t globals;

tts_model_info_t *tts_model_lookup(const char *lang) {
    tts_model_info_t *model = NULL;

    if(!lang) { return NULL; }

    switch_mutex_lock(globals.mutex);
    model = switch_core_hash_find(globals.models, lang);
    switch_mutex_unlock(globals.mutex);

    return model;
}

char *enc2ext(const char *fmt) {
    if(strcasecmp(fmt, "mp3") == 0) { return "mp3"; }
    return (char *)fmt;
}

/*
 * based on switch_utils.c
 */
char *escape_dquotes(const char *string) {
    size_t string_len = strlen(string);
    size_t i;
    size_t n = 0;
    size_t dest_len = 0;
    char *dest;

    dest_len = strlen(string) + 1;
    for (i = 0; i < string_len; i++) {
        switch (string[i]) {
            case '\"': dest_len += 1; break;
        }
    }

    dest = (char *) malloc(sizeof(char) * dest_len);
    switch_assert(dest);

    for (i = 0; i < string_len; i++) {
        switch (string[i]) {
            case '\"':
                dest[n++] = '\\';
                dest[n++] = '\"';
            break;
            default:
                dest[n++] = string[i];
        }
    }
    dest[n++] = '\0';

    switch_assert(n == dest_len);
    return dest;
}

switch_status_t write_file(char *file_name, switch_byte_t *buf, uint32_t buf_len) {
    switch_status_t status = SWITCH_STATUS_SUCCESS;
    switch_memory_pool_t *pool = NULL;
    switch_size_t len = buf_len;
    switch_file_t *fd = NULL;

    if(switch_core_new_memory_pool(&pool) != SWITCH_STATUS_SUCCESS) {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "switch_core_new_memory_pool() fail\n");
        switch_goto_status(SWITCH_STATUS_GENERR, out);
    }
    if((status = switch_file_open(&fd, file_name, (SWITCH_FOPEN_WRITE | SWITCH_FOPEN_TRUNCATE | SWITCH_FOPEN_CREATE), SWITCH_FPROT_OS_DEFAULT, pool)) != SWITCH_STATUS_SUCCESS) {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Open fail: %s\n", file_name);
        goto out;
    }
    if((status = switch_file_write(fd, buf, &len)) != SWITCH_STATUS_SUCCESS) {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Write fail (%s)\n", file_name);
    }
    switch_file_close(fd);
out:
    if(pool) {
        switch_core_destroy_memory_pool(&pool);
    }
    return status;
}
