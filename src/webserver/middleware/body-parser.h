//
// Created by roteklaue on 10/9/24.
//

#ifndef BODY_PARSER_H
#define BODY_PARSER_H

#include "../ContentType.h"

typedef enum {
    Array,
    Object,
    Int,
    Float,
    String
} DataType;

typedef struct {
    void *data;
    DataType data_type;
} BodyData;

BodyData *parse_body_data(char *body, enum ContentType type);
void free_body_data(BodyData *data);

#endif //BODY_PARSER_H
