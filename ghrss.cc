#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <leveldb/options.h>
#include <map>
#include <stdlib.h>
#include "header/ghrss.h"
#include <curl/curl.h>
#include <string>
#include <fmt/core.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cassert>
#include "leveldb/db.h"

typedef  struct memory_db_cache _str_int_t;

#define DEBUG(x) printf("%s\n", x.c_str())
// #define DEBUG_MAP()

static void google_leveldb_open(const char* filename, _str_int_t *memdb, file_opcode op)
{
        leveldb::DB* db;
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, filename, &db);
        assert(status.ok());
        

        if (op == F_LOAD) {

                // db->Put(leveldb::WriteOptions(), "a", "Aa");
                leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());

                for (it->SeekToFirst(); it->Valid(); it->Next()) {
                        memdb->db.insert(
                                {
                                        it->key().ToString(),
                                        (std::string)it->value().data()
                                }
                        );
                }
                assert(it->status().ok());

                delete it;
        }

        if (op == F_STORE) {
           
                for (auto i = memdb->db.begin(); i != memdb->db.end(); ++i) {
                        
                        db->Put(leveldb::WriteOptions(), i->first, i->second);
                        // printf("saved %s %s\n", i->first.c_str(), i->second.c_str());

                }
        }
        

        delete db;

}

static void _init_curl(gh_rss_ctx_t *ctx)
{
        ctx->curl_ctx = curl_easy_init();
}


static size_t _curl_write_mem_cb(void* contents, size_t size, size_t n, void *userp)
{
        size_t realsize = size * n;
        
        struct memory_struct *memstruct = (struct memory_struct*)userp;

        char* ptr = (char*)realloc(memstruct->mem_ptr, memstruct->memsize + realsize + 1);

        if (!ptr) {
                perror("realloc");
                return ENOMEM;
        }

        memstruct->mem_ptr = ptr;
        memcpy(&(memstruct->mem_ptr[memstruct->memsize]), contents, realsize);
        memstruct->memsize = memstruct->memsize + realsize;
        memstruct->mem_ptr[memstruct->memsize] = '\0';
        return realsize;
}

static inline void _peform(gh_rss_ctx_t *ctx, std::string builded_url)
{
        CURLcode res;
        curl_easy_setopt(ctx->curl_ctx, CURLOPT_URL, builded_url.c_str());
        curl_easy_setopt(ctx->curl_ctx, CURLOPT_WRITEFUNCTION, _curl_write_mem_cb);
        curl_easy_setopt(ctx->curl_ctx, CURLOPT_WRITEDATA, (void*)&ctx->memory_struct);


        res = curl_easy_perform(ctx->curl_ctx);

        if (res != CURLE_OK) {
                fprintf(stderr, "%s\n", curl_easy_strerror(res));
        }
}

static inline std::string build_github_query(const char* username, const char* reponame)
{
        std::string res = fmt::format("https://github.com/{0}/{1}/releases.atom", 
                                                username, reponame);

        return res;
}

static void _xml_internal_parse_entry(xmlDocPtr xml, xmlNodePtr cur, _str_int_t* memdb)
{
        xmlChar *id;
        cur = cur->xmlChildrenNode;
        while (cur != NULL) {
                if (!xmlStrcmp(cur->name, (const xmlChar*)"id")) {
                        id = xmlNodeListGetString(xml, cur->xmlChildrenNode, 1);

                        memdb->db.insert({std::string((char*)id), "not_seen_yet"});
                        xmlFree(id);
                }
                cur = cur->next;
        }
}

static void _xml_parse(struct memory_struct *memstruct, _str_int_t* memdb)
{
        xmlDocPtr xml;
        xml = xmlReadMemory(memstruct->mem_ptr, memstruct->memsize, "random.xml", "UTF-8", 0);

        if (xml == NULL) {
                fprintf(stderr, "XML FAILED PARSE");
                return;
        }

        xmlNodePtr cur;
        cur = xmlDocGetRootElement(xml);

        if (xmlStrcmp(cur->name, (const xmlChar*)"feed")) {
                printf("xml root invalid");
                goto xml_free;
        }

        cur = cur->xmlChildrenNode;
        while(cur != NULL) {
                if (!xmlStrcmp(cur->name, (const xmlChar*)"entry")) {
                        _xml_internal_parse_entry(xml, cur, memdb);
                }
                
                cur = cur->next;

        }

xml_free:
        xmlFreeDoc(xml);
}

void gh_rss_init(gh_rss_ctx_t *ctx) 
{
        /* init heap */
        // ctx->mem_ptr = (char*)malloc(1);
        // ctx->memsize =   0;
        ctx->memory_struct.mem_ptr = (char*)malloc(1);
        ctx->memory_struct.memsize = 0;

        ctx->memdb.db = std::map<std::string, std::string>();
        ctx->memdb.size = 0;

        google_leveldb_open("test.db", &ctx->memdb, F_LOAD);

        /* init curl */
        curl_global_init(CURL_GLOBAL_ALL);
        _init_curl(ctx);

        /* test purpose */
}

static int _reset_memory(struct memory_struct *memstruct)
{
        memset(memstruct->mem_ptr, '\0', memstruct->memsize);
        char* ptr = (char*)realloc(memstruct->mem_ptr, 1);
        if (!ptr) {
                perror("reset memory");
                return ENOMEM;
        }

        memstruct->mem_ptr = ptr;
        memstruct->memsize = 0;

        return 0;
}

void gh_rss_get_updates(gh_rss_ctx_t *ctx, const char* username, const char* repo)
{
        std::string url = build_github_query(username, repo);

        #ifdef ENABLE_DEBUG_SERVER
        _peform(ctx, "http://127.0.0.1:9000/releases.atom");
        #else
        _peform(ctx, url);
        #endif

        // printf("%s\n", ctx->memory_struct.mem_ptr);
        _xml_parse(&ctx->memory_struct, &ctx->memdb);
        _reset_memory(&ctx->memory_struct);
}

char* gh_rss_list(gh_rss_ctx_t *ctx)
{
        for (auto itr = ctx->memdb.db.begin(); itr != ctx->memdb.db.end(); ++itr) { 
                if (!itr->second.compare("not_seen_yet")) {
                        // char* ret = (char*)malloc(strlen(itr->first.c_str()));
                        // ret = itr->first.c_str();
                        return strdup(itr->first.c_str());
                }
        } 
        return 0; /* no updates */
}

void gh_rss_seen(gh_rss_ctx_t *ctx, char* special)
{
        ctx->memdb.db[special] = "seen";
}

void gh_rss_free(gh_rss_ctx_t *ctx) 
{

        

        free(ctx->memory_struct.mem_ptr);

        curl_easy_cleanup(ctx->curl_ctx);
        curl_global_cleanup();

        google_leveldb_open("test.db", &ctx->memdb, F_STORE);
}