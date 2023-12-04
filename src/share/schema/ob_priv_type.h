/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef OCEABASE_SHARE_SCHEMA_OB_PRIV_TYPE_H_
#define OCEABASE_SHARE_SCHEMA_OB_PRIV_TYPE_H_

#include <stdint.h>
#include "share/schema/ob_sys_priv_type.h"
#include "share/schema/ob_obj_priv_type.h"

typedef int64_t ObPrivSet;
typedef int64_t ObPrivType;

#define OB_PRIV_SET_EMPTY 0

#define OB_TEST_PRIVS(privs_a, privs_b) (((privs_a) & (privs_b)) == (privs_b))
#define OB_PRIV_HAS_ANY(privs_a, privs_b) (((privs_a) & (privs_b)) != 0)
#define OB_PRIV_HAS_OTHER(privs_a, privs_b) (((privs_a) & (~privs_b)) != 0)

enum OB_PRIV_SHIFT
{
  OB_PRIV_INVALID_SHIFT = 0,
  OB_PRIV_ALTER_SHIFT,
  OB_PRIV_CREATE_SHIFT,
  OB_PRIV_CREATE_USER_SHIFT,
  OB_PRIV_DELETE_SHIFT,
  OB_PRIV_DROP_SHIFT,
  OB_PRIV_GRANT_SHIFT,
  OB_PRIV_INSERT_SHIFT,
  OB_PRIV_UPDATE_SHIFT,
  OB_PRIV_SELECT_SHIFT,
  OB_PRIV_INDEX_SHIFT,
  OB_PRIV_CREATE_VIEW_SHIFT,
  OB_PRIV_SHOW_VIEW_SHIFT,
  OB_PRIV_SHOW_DB_SHIFT,
  OB_PRIV_SUPER_SHIFT,
  OB_PRIV_PROCESS_SHIFT,
  OB_PRIV_BOOTSTRAP_SHIFT,
  OB_PRIV_CREATE_SYNONYM_SHIFT,
  OB_PRIV_AUDIT_SHIFT,
  OB_PRIV_COMMENT_SHIFT,
  OB_PRIV_LOCK_SHIFT,
  OB_PRIV_RENAME_SHIFT,
  OB_PRIV_REFERENCES_SHIFT,
  OB_PRIV_EXECUTE_SHIFT,
  OB_PRIV_FLASHBACK_SHIFT,
  OB_PRIV_READ_SHIFT,
  OB_PRIV_WRITE_SHIFT,
  OB_PRIV_FILE_SHIFT,
  OB_PRIV_ALTER_TENANT_SHIFT,
  OB_PRIV_ALTER_SYSTEM_SHIFT,
  OB_PRIV_CREATE_RESOURCE_POOL_SHIFT,
  OB_PRIV_CREATE_RESOURCE_UNIT_SHIFT,
  OB_PRIV_DEBUG_SHIFT,
  OB_PRIV_REPL_SLAVE_SHIFT,
  OB_PRIV_REPL_CLIENT_SHIFT,
  OB_PRIV_DROP_DATABASE_LINK_SHIFT,
  OB_PRIV_CREATE_DATABASE_LINK_SHIFT,
  OB_PRIV_ALTER_ROUTINE_SHIFT,
  OB_PRIV_CREATE_ROUTINE_SHIFT,
  OB_PRIV_MAX_SHIFT_PLUS_ONE,
  OB_PRIV_MAX_SHIFT_LIMIT = 65
};

#define OB_PRIV_MAX_SHIFT (OB_PRIV_MAX_SHIFT_PLUS_ONE - 1)

#define OB_PRIV_GET_TYPE(i) (1LL << i)

#define OB_PRIV_ALTER         OB_PRIV_GET_TYPE(OB_PRIV_ALTER_SHIFT)
#define OB_PRIV_CREATE        OB_PRIV_GET_TYPE(OB_PRIV_CREATE_SHIFT)
#define OB_PRIV_CREATE_USER   OB_PRIV_GET_TYPE(OB_PRIV_CREATE_USER_SHIFT)
#define OB_PRIV_DELETE        OB_PRIV_GET_TYPE(OB_PRIV_DELETE_SHIFT)
#define OB_PRIV_DROP          OB_PRIV_GET_TYPE(OB_PRIV_DROP_SHIFT)
#define OB_PRIV_GRANT         OB_PRIV_GET_TYPE(OB_PRIV_GRANT_SHIFT)
#define OB_PRIV_INSERT        OB_PRIV_GET_TYPE(OB_PRIV_INSERT_SHIFT)
#define OB_PRIV_UPDATE        OB_PRIV_GET_TYPE(OB_PRIV_UPDATE_SHIFT)
#define OB_PRIV_SELECT        OB_PRIV_GET_TYPE(OB_PRIV_SELECT_SHIFT)
#define OB_PRIV_INDEX         OB_PRIV_GET_TYPE(OB_PRIV_INDEX_SHIFT)
#define OB_PRIV_CREATE_VIEW   OB_PRIV_GET_TYPE(OB_PRIV_CREATE_VIEW_SHIFT)
#define OB_PRIV_SHOW_VIEW     OB_PRIV_GET_TYPE(OB_PRIV_SHOW_VIEW_SHIFT)
#define OB_PRIV_SHOW_DB       OB_PRIV_GET_TYPE(OB_PRIV_SHOW_DB_SHIFT)
#define OB_PRIV_SUPER         OB_PRIV_GET_TYPE(OB_PRIV_SUPER_SHIFT)
#define OB_PRIV_PROCESS       OB_PRIV_GET_TYPE(OB_PRIV_PROCESS_SHIFT)
#define OB_PRIV_BOOTSTRAP     OB_PRIV_GET_TYPE(OB_PRIV_BOOTSTRAP_SHIFT) // only grant to root@sys
#define OB_PRIV_CREATE_SYNONYM OB_PRIV_GET_TYPE(OB_PRIV_CREATE_SYNONYM_SHIFT)
#define OB_PRIV_AUDIT         OB_PRIV_GET_TYPE(OB_PRIV_AUDIT_SHIFT)
#define OB_PRIV_COMMENT       OB_PRIV_GET_TYPE(OB_PRIV_COMMENT_SHIFT)
#define OB_PRIV_LOCK          OB_PRIV_GET_TYPE(OB_PRIV_LOCK_SHIFT)
#define OB_PRIV_RENAME        OB_PRIV_GET_TYPE(OB_PRIV_RENAME_SHIFT)
#define OB_PRIV_REFERENCES    OB_PRIV_GET_TYPE(OB_PRIV_REFERENCES_SHIFT)
#define OB_PRIV_FLASHBACK     OB_PRIV_GET_TYPE(OB_PRIV_FLASHBACK_SHIFT) 
#define OB_PRIV_READ          OB_PRIV_GET_TYPE(OB_PRIV_READ_SHIFT)  
#define OB_PRIV_WRITE         OB_PRIV_GET_TYPE(OB_PRIV_WRITE_SHIFT)  
#define OB_PRIV_FILE          OB_PRIV_GET_TYPE(OB_PRIV_FILE_SHIFT)
#define OB_PRIV_ALTER_TENANT  OB_PRIV_GET_TYPE(OB_PRIV_ALTER_TENANT_SHIFT)  
#define OB_PRIV_ALTER_SYSTEM  OB_PRIV_GET_TYPE(OB_PRIV_ALTER_SYSTEM_SHIFT)  
#define OB_PRIV_CREATE_RESOURCE_POOL  OB_PRIV_GET_TYPE(OB_PRIV_CREATE_RESOURCE_POOL_SHIFT)  
#define OB_PRIV_CREATE_RESOURCE_UNIT  OB_PRIV_GET_TYPE(OB_PRIV_CREATE_RESOURCE_UNIT_SHIFT)  
#define OB_PRIV_DEBUG         OB_PRIV_GET_TYPE(OB_PRIV_DEBUG_SHIFT)
#define OB_PRIV_REPL_SLAVE    OB_PRIV_GET_TYPE(OB_PRIV_REPL_SLAVE_SHIFT)
#define OB_PRIV_REPL_CLIENT   OB_PRIV_GET_TYPE(OB_PRIV_REPL_CLIENT_SHIFT)
#define OB_PRIV_DROP_DATABASE_LINK    OB_PRIV_GET_TYPE(OB_PRIV_DROP_DATABASE_LINK_SHIFT)
#define OB_PRIV_CREATE_DATABASE_LINK  OB_PRIV_GET_TYPE(OB_PRIV_CREATE_DATABASE_LINK_SHIFT)
#define OB_PRIV_EXECUTE       OB_PRIV_GET_TYPE(OB_PRIV_EXECUTE_SHIFT)
#define OB_PRIV_ALTER_ROUTINE  OB_PRIV_GET_TYPE(OB_PRIV_ALTER_ROUTINE_SHIFT)
#define OB_PRIV_CREATE_ROUTINE  OB_PRIV_GET_TYPE(OB_PRIV_CREATE_ROUTINE_SHIFT)

#define OB_PRIV_ALL                                                             \
  (OB_PRIV_ALTER | OB_PRIV_CREATE | OB_PRIV_CREATE_USER | OB_PRIV_DELETE |      \
   OB_PRIV_DROP | OB_PRIV_INSERT | OB_PRIV_UPDATE | OB_PRIV_SELECT |            \
   OB_PRIV_INDEX | OB_PRIV_CREATE_VIEW | OB_PRIV_SHOW_VIEW | OB_PRIV_SHOW_DB |  \
   OB_PRIV_SUPER | OB_PRIV_PROCESS | OB_PRIV_CREATE_SYNONYM | OB_PRIV_FILE |    \
   OB_PRIV_ALTER_TENANT | OB_PRIV_ALTER_SYSTEM |                                \
   OB_PRIV_CREATE_RESOURCE_POOL | OB_PRIV_CREATE_RESOURCE_UNIT|                 \
   OB_PRIV_REPL_SLAVE | OB_PRIV_REPL_CLIENT |                                   \
   OB_PRIV_DROP_DATABASE_LINK | OB_PRIV_CREATE_DATABASE_LINK |                  \
   OB_PRIV_EXECUTE | OB_PRIV_ALTER_ROUTINE | OB_PRIV_CREATE_ROUTINE)

#define OB_PRIV_DB_ACC                                                        \
  (OB_PRIV_ALTER | OB_PRIV_CREATE | OB_PRIV_DELETE |                          \
   OB_PRIV_DROP | OB_PRIV_INSERT | OB_PRIV_UPDATE | OB_PRIV_SELECT |          \
   OB_PRIV_INDEX | OB_PRIV_CREATE_VIEW | OB_PRIV_SHOW_VIEW |                  \
   OB_PRIV_EXECUTE | OB_PRIV_ALTER_ROUTINE | OB_PRIV_CREATE_ROUTINE)

#define OB_PRIV_TABLE_ACC                                                     \
  (OB_PRIV_ALTER | OB_PRIV_CREATE | OB_PRIV_DELETE |                          \
   OB_PRIV_DROP | OB_PRIV_INSERT | OB_PRIV_UPDATE | OB_PRIV_SELECT |          \
   OB_PRIV_INDEX | OB_PRIV_CREATE_VIEW | OB_PRIV_SHOW_VIEW)

#define OB_PRIV_ROUTINE_ACC                                                     \
  (OB_PRIV_ALTER_ROUTINE | OB_PRIV_EXECUTE)

#endif //ifndef OCEABASE_SHARE_SCHEMA_OB_PRIV_TYPE_H_
