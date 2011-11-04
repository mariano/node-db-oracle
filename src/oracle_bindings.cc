// Copyright 2011 Mariano Iglesias <mgiglesias@gmail.com>
#include "./node-db/binding.h"
#include "./oracle.h"
#include "./query.h"

extern "C" {
    void init(v8::Handle<v8::Object> target) {
        node_db::EventEmitter::Init();
        node_db_oracle::Oracle::Init(target);
        node_db_oracle::Query::Init(target);
    }

    NODE_MODULE(oracle_bindings, init);
}
