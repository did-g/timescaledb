/*
 * Copyright (c) 2016-2018  Timescale, Inc. All Rights Reserved.
 *
 * This file is licensed under the Apache License,
 * see LICENSE-APACHE at the top level directory.
 */
#ifndef TIMESCALEDB_COMPAT_H
#define TIMESCALEDB_COMPAT_H

#include <postgres.h>
#include <pgstat.h>

#include "export.h"

#define is_supported_pg_version_96(version) ((version >= 90603) && (version < 100000))
#define is_supported_pg_version_10(version) ((version >= 100002) && (version < 110000))
#define is_supported_pg_version(version) (is_supported_pg_version_96(version) || is_supported_pg_version_10(version))

#define PG96 is_supported_pg_version_96(PG_VERSION_NUM)
#define PG10 is_supported_pg_version_10(PG_VERSION_NUM)

/* TupleDescAttr was only backpatched to 9.6.5. Make it work under 9.6.3 and 9.6.4 */
#if ((PG_VERSION_NUM >= 90603) && PG_VERSION_NUM < 90605)
#define TupleDescAttrCompat(tupdesc, i) ((tupdesc)->attrs[(i)])
#else
#define TupleDescAttrCompat(tupdesc, i) TupleDescAttr(tupdesc, i)
#endif

#ifndef TupleDescAttr
/*
   backport in PG10 commit d34a74dd
*/
#define TupleDescAttr(a, i) ((a)->attrs[(i)])
#endif

#if PG10

#define ExecARInsertTriggersCompat(estate, result_rel_info, tuple, recheck_indexes) \
	ExecARInsertTriggers(estate, result_rel_info, tuple, recheck_indexes, NULL)
#define ExecASInsertTriggersCompat(estate, result_rel_info) \
	ExecASInsertTriggers(estate, result_rel_info, NULL)
#define InitResultRelInfoCompat(result_rel_info, result_rel_desc, result_rel_index, instrument_options) \
	InitResultRelInfo(result_rel_info, result_rel_desc, result_rel_index, NULL, instrument_options)
#define CheckValidResultRelCompat(relinfo, operation)	\
	CheckValidResultRel(relinfo, operation)
#define ParseFuncOrColumnCompat(pstate, funcname, fargs, fn, location) \
	ParseFuncOrColumn(pstate, funcname, fargs, (pstate)->p_last_srf, fn, location)
#define make_op_compat(pstate, opname, ltree, rtree, location)	\
	make_op(pstate, opname, ltree, rtree, (pstate)->p_last_srf, location)
#define get_projection_info_slot_compat(pinfo) \
	(pinfo->pi_state.resultslot)
#define map_variable_attnos_compat(returning_clauses, varno, sublevels_up, map, map_size, rowtype, found_whole_row) \
	map_variable_attnos(returning_clauses, varno, sublevels_up, map, map_size, rowtype, found_whole_row);
#define ExecBuildProjectionInfoCompat(tl, exprContext, slot, parent, inputdesc) \
	 ExecBuildProjectionInfo(tl, exprContext, slot, parent, inputdesc)
#define WaitLatchCompat(latch, wakeEvents, timeout) \
	WaitLatch(latch, wakeEvents, timeout, PG_WAIT_EXTENSION)

#elif PG96

#define ExecARInsertTriggersCompat(estate, result_rel_info, tuple, recheck_indexes) \
	ExecARInsertTriggers(estate, result_rel_info, tuple, recheck_indexes)
#define ExecASInsertTriggersCompat(estate, result_rel_info) \
	ExecASInsertTriggers(estate, result_rel_info)
#define InitResultRelInfoCompat(result_rel_info, result_rel_desc, result_rel_index, instrument_options) \
	InitResultRelInfo(result_rel_info, result_rel_desc, result_rel_index, instrument_options)
#define CheckValidResultRelCompat(relinfo, operation) \
	CheckValidResultRel((relinfo)->ri_RelationDesc, operation)
#define ParseFuncOrColumnCompat(pstate, funcname, fargs, fn, location) \
	ParseFuncOrColumn(pstate, funcname, fargs, fn, location)
#define make_op_compat(pstate, opname, ltree, rtree, location)	\
	make_op(pstate, opname, ltree, rtree, location)
#define get_projection_info_slot_compat(pinfo) \
	(pinfo->pi_slot)
#define map_variable_attnos_compat(expr, varno, sublevels_up, map, map_size, rowtype, found_whole_row) \
	map_variable_attnos(expr, varno, sublevels_up, map, map_size, found_whole_row)
#define ExecBuildProjectionInfoCompat(tl, exprContext, slot, parent, inputdesc) \
	 ExecBuildProjectionInfo((List *)ExecInitExpr((Expr *) tl, NULL), exprContext, slot, inputdesc)
#define WaitLatchCompat(latch, wakeEvents, timeout) \
	WaitLatch(latch, wakeEvents, timeout)
#else

#error "Unsupported PostgreSQL version"

#endif							/* PG_VERSION_NUM */

#endif							/* TIMESCALEDB_COMPAT_H */
