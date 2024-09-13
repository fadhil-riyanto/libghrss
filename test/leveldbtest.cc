#include <cassert>
#include <cstdio>
#include <cstdlib>
#include "leveldb/db.h"

int main(void)
{
        leveldb::DB* db;
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, "/tmp/test.db", &db);
        assert(status.ok());

        int intKey = 2332;
        leveldb::Slice key((char*)&intKey, sizeof(int));

        db->Put(leveldb::WriteOptions(), "key", key);


        leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());

        for (it->SeekToFirst(); it->Valid(); it->Next()) {
                printf("%s | %s\n", it->key().ToString().c_str(), (key)it->value().data());
        }
        assert(it->status().ok());
        delete  it;

        delete db;
}