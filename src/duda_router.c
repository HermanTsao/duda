 /* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Duda I/O
 *  --------
 *  Copyright (C) 2012-2014, Eduardo Silva P. <eduardo@monkey.io>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <monkey/mk_api.h>

#include "duda.h"
#include "duda_router.h"

/*
 * @OBJ_NAME: router
 * @OBJ_MENU: Router
 * @OBJ_DESC: The Router object exposes methods to handle routing for requests
 * that arrive. It register certain patterns as rules and associate specific
 * callback functions to be invoked once the core finds a match.
 */

/*
 * The STATIC 'way', aims to replace the old map->static_add() method used
 * to route fixed and static URL patterns to specific callbacks, it do not
 * support dynamic parameters.
 */
static int router_add_static(char *pattern,
                             void (*callback)(duda_request_t *),
                             struct mk_list *list)
{
    struct duda_router_rule *rule;

    rule           = mk_api->mem_alloc(sizeof(struct duda_router_rule));
    rule->type     = DUDA_ROUTER_STATIC;
    rule->pattern  = pattern;
    rule->callback = callback;
    mk_list_add(&rule->_head, list);

    return 0;
}

static int router_add_dynamic(char *pattern,
                              void (*callback)(duda_request_t *))
{
    (void) pattern;
    (void) callback;

    return 0;
}


/*
 * @METHOD_NAME: map
 * @METHOD_DESC: It register a Router interface and pattern associating it
 * to a service callback.
 * @METHOD_PROTO: int set_path(const char *dir)
 * @METHOD_PARAM: dir directory path where the configuration files are located.
 * @METHOD_RETURN: Upon successful completion it returns 0, on error returns -1.
 */
int duda_router_map(char *pattern,
                    void (*callback)(duda_request_t *),
                    struct mk_list *list)
{
    int ret;
    char *tmp;


    tmp = strstr(pattern, ":");
    if (!tmp) {
        ret = router_add_static(pattern, callback, list);
    }
    else {
        ret = router_add_dynamic(pattern, callback);
    }

    return ret;
}

struct duda_api_router *duda_router_object()
{
    struct duda_api_router *r;

    r = mk_api->mem_alloc(sizeof(struct duda_api_router));
    r->_map = duda_router_map;

    return r;
}