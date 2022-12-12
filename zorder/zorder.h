#ifndef ZORDER_H
#define ZORDER_H

#include "fmgr.h"

#define SAMPLE_HINT 60
#define MAX_SAMPLE_SIZE 1000000

/*
 * ReservoirSamplingState are varlena objects, so must meet the varlena convention
 * that the first int32 of the object contains the total object size in bytes.
 * Be sure to use VARSIZE() and SET_VARSIZE() to access it, though!
 */
typedef struct ReservoirSamplingState
{
    int32 vl_len;        /* varlena header (do not touch directly!) */
    int32 data_len;      /* num of elems in data[] */
    int32 sample_size;   /* sample_size = SAMPLE_HINT * partition_num */
    int32 partition_num; /* the number of partitions */
    int32 input_size;    /* cursor of input data */
    Oid element_type;    /* the type of Datum */
    int16 typlen;        /* sizeof element_type,-1 indicates a "varlena" type */
    bool typbyval;       /* whether internal routines pass a value of this type by value or by reference */
    bool is_flattened;   /* when has been sended the struct will be flattened, and the data will store offset*/
    char padding[4];
    Datum data[];        /* datas of elements */
} ReservoirSamplingState;

#define RS_HEADER_SIZE sizeof(ReservoirSamplingState)
#define RS_SIZE(r) VARSIZE(r)
#define RS_SET_SIZE(r, len) SET_VARSIZE(r, len)
#define RS_ELEMTYPE(r) ((r)->element_type)
#define RS_ELEM(r, n) ((r)->data[(n)])

/* make sure the isFlattened is true */
#define RS_DATA_OFFSET(r) (RS_HEADER_SIZE + ((r)->data_len) * sizeof(Datum))
#define RS_DATA_PTR(r) (((char *)(r)) + RS_DATA_OFFSET(r))
#define RS_FLAT_ELEM_OFFSET(r, n) ((uint64_t)RS_ELEM(r, n))
#define RS_FLAT_ELEM(r, n) ((Datum)(RS_DATA_PTR(r) + RS_FLAT_ELEM_OFFSET(r, n)))

#define RS_ELEM_ANY(r, n) (((r)->is_flattened) ? RS_FLAT_ELEM(r, n) : RS_ELEM(r, n))

/* zorder functions */

extern Datum reservoir_sampling_transfn(PG_FUNCTION_ARGS);
extern Datum reservoir_sampling_collecfn(PG_FUNCTION_ARGS);
extern Datum reservoir_sampling_finalfn(PG_FUNCTION_ARGS);
extern Datum reservoir_sampling_state_in(PG_FUNCTION_ARGS);
extern Datum reservoir_sampling_state_out(PG_FUNCTION_ARGS);
extern Datum reservoir_sampling_state_recv(PG_FUNCTION_ARGS);
extern Datum reservoir_sampling_state_send(PG_FUNCTION_ARGS);
extern Datum reservoir_sampling_state_print(PG_FUNCTION_ARGS);
extern Datum get_partitionid(PG_FUNCTION_ARGS);

/* translate rs_state to flattened rs_state*/
extern ReservoirSamplingState *rs_flatten(ReservoirSamplingState *state);

#endif /* ZORDER_H */




#ifndef ZORDER_H
#define ZORDER_H

#include "array.h"
/*
 * working state for accumReservoirSampling and friends
 */
typedef struct ReservoirSamplingState
{
	ArrayBuildState astate;
	FunctionCallInfoData locfcinfo;  // for cmp in array_qsort
	int partition_num;
	int index;
}ReservoirSamplingState;

/* TODO zorder functions */
extern Datum reservoir_sampling_transfn(PG_FUNCTION_ARGS);
extern Datum reservoir_sampling_finalfn(PG_FUNCTION_ARGS);
extern Datum get_partitionid(PG_FUNCTION_ARGS);
extern Datum interleave_bits(PG_FUNCTION_ARGS);
extern Datum get_bounds_addr(PG_FUNCTION_ARGS);

#endif /* ZORDER_H */

