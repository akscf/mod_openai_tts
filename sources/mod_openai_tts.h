/**
 * (C)2024 aks
 * https://github.com/akscf/
 **/
#ifndef MOD_OPENAI_TTS_H
#define MOD_OPENAI_TTS_H

#include <switch.h>
#include <switch_stun.h>
#include <switch_curl.h>
#include <stdint.h>
#include <string.h>

#ifndef true
#define true SWITCH_TRUE
#endif
#ifndef false
#define false SWITCH_FALSE
#endif

#define VERSION             "1.0-v1_http"
#define FILE_SIZE_MAX       (2*1024*1024)

// #define CURL_DEBUG_REQUESTS 1

typedef struct {
    switch_mutex_t          *mutex;
    switch_hash_t           *models;
    const char              *cache_path;
    const char              *tmp_path;
    const char              *opt_encoding;
    const char              *user_agent;
    const char              *api_url;
    const char              *api_key;
    const char              *proxy;
    const char              *proxy_credentials;
    uint32_t                file_size_max;
    uint32_t                request_timeout;            // seconds
    uint32_t                connect_timeout;            // seconds
    uint8_t                 fl_voice_name_as_language;
    uint8_t                 fl_log_http_error;
    uint8_t                 fl_cache_enabled;
} globals_t;

typedef struct {
    char        *lang;
    char        *voice;
    char        *model;
} tts_model_info_t;

typedef struct {
    switch_memory_pool_t    *pool;
    switch_file_handle_t    *fhnd;
    switch_buffer_t         *curl_recv_buffer;
    tts_model_info_t        *model_info;
    char                    *curl_send_buffer_ref;
    char                    *language;
    char                    *alt_voice;
    char                    *alt_model;
    char                    *dst_file;
    uint32_t                samplerate;
    uint32_t                channels;
    size_t                  curl_send_buffer_len;
} tts_ctx_t;

char *enc2ext(const char *fmt);
char *escape_dquotes(const char *string);

tts_model_info_t *tts_model_lookup(const char *lang);
switch_status_t write_file(char *file_name, switch_byte_t *buf, uint32_t buf_len);

#endif
