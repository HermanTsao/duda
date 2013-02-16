/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Duda I/O
 *  --------
 *  Copyright (C) 2012-2013, Eduardo Silva P. <edsiper@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "duda_utils.h"

#include "MKPlugin.h"
#include "duda_param.h"
#include "duda.h"

/*
 * @OBJ_NAME: param
 * @OBJ_DESC: The param object provides a set of methods to manipulate parameters
 * that comes in the URI per the webservice spec
 */

struct duda_api_param *duda_param_object()
{
    struct duda_api_param *p;

    p = mk_api->mem_alloc(sizeof(struct duda_api_param));
    p->count      = duda_param_count;
    p->get        = duda_param_get;
    p->get_number = duda_param_get_number;
    p->len        = duda_param_len;

    return p;
};

/*
 * @METHOD_NAME: get
 * @METHOD_DESC: Return a new buffer with the value of the given parameter index in
 * string format.
 * @METHOD_PARAM: dr the request context information hold by a duda_request_t type
 * @METHOD_PARAM: idx numeric parameter position starting from zero
 * @METHOD_RETURN: Upon successful completion it returns the new memory buffer with
 * the parameter value specified, on error returns NULL.
 */
char *duda_param_get(duda_request_t *dr, short int idx)
{
    if (idx >= dr->n_params) {
        return NULL;
    }

    return mk_api->str_copy_substr(dr->params[idx].data, 0,
                                   (int) dr->params[idx].len);
}

/*
 * @METHOD_NAME: get_number
 * @METHOD_DESC: Return a the value of the given parameter index in
 * integer format. Use only when expecting a numeric value.
 * @METHOD_PARAM: dr the request context information hold by a duda_request_t type
 * @METHOD_PARAM: idx numeric parameter position starting from zero
 * @METHOD_PARAM: res stores the parameter value
 * @METHOD_RETURN: Upon successful completion it returns 0, on error it returns -1.
 */
int duda_param_get_number(duda_request_t *dr, short int idx, long *res)
{
    int ret;
    long number;

    if (idx >= dr->n_params) {
        return -1;
    }

    ret = duda_utils_strtol(dr->params[idx].data, dr->params[idx].len, &number);
    if (ret == -1) {
        return -1;
    }

    *res = number;
    return 0;
}

/*
 * @METHOD_NAME: count
 * @METHOD_DESC: Returns the total number of parameters
 * @METHOD_PARAM: dr the request context information hold by a duda_request_t type
 * @METHOD_RETURN: Upon successful completion it returns the number of parameters,
 * on error it returns -1.
 */
short int duda_param_count(duda_request_t *dr)
{
    if (!dr) {
        return -1;
    }
    return dr->n_params;
}

/*
 * @METHOD_NAME: len
 * @METHOD_DESC: Returns the string length of a given parameter by index
 * @METHOD_PARAM: dr the request context information hold by a duda_request_t type
 * @METHOD_PARAM: idx numeric parameter position starting from zero
 * @METHOD_RETURN: Upon successful completion it returns the parameter string length,
 * on error it returns -1.
 */
/* Return the length of the parameter */
short int duda_param_len(duda_request_t *dr, short int idx)
{
    if (!dr) {
        return -1;
    }

    return dr->params[idx].len;
}
