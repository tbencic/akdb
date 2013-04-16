#ifndef CONFIGURATION
#define CONFIGURATION

#include "ini_parser/iniparser.h"
/**
 * @def DB_NAME
 * @brief Constant declaring the name oof database file
*/
#define DB_FILE (iniparser_getstring(AK_config,"general:db_file","kalashnikov.db"))
/**
 * @def MAX_NUM_OF_BLOCKS
 * @brief Constant declaring maximum number of blocks in a segment 
*/
#define MAX_NUM_OF_BLOCKS (iniparser_getint(AK_config,"segments:max_num_of_blocks",200))
/**
  * @def MAX_EXTENTS_IN_SEGMENT
  * @brief Constant declaring maximum number of extents in segment
*/
#define MAX_EXTENTS_IN_SEGMENT 200
/**
  * @def MAX_FREE_SPACE_SIZE
  * @brief Constant declaring maximum free space in block
*/
#define MAX_FREE_SPACE_SIZE (iniparser_getint(AK_config,"blocks:max_free_space_size",4000)) 
/**
  * @def MAX_LAST_TUPLE_DICT_SIZE_TO_USE
  * @brief Constant declaring maximum size od last tuple in dictionary
*/
#define MAX_LAST_TUPLE_DICT_SIZE_TO_USE (iniparser_getint(AK_config,"dictionary:max_last_tuple_dict_size_to_use",470))
/**
  * @def DB_FILE_SIZE
  * @brief Constant declaring size of DB file in MB
 */
#define DB_FILE_SIZE (iniparser_getint(AK_config,"general:db_file_size",40))
/**
  * @def MAX_DB_FILE_BLOCKS
  * @brief Constant declaring total blocks in DB file (for the given DB_FILE size)
 */
#define DB_FILE_BLOCKS_NUM (1024 * 1024 * DB_FILE_SIZE / sizeof(AK_block))
/**
  * @def INITIAL_EXTENT_SIZE
  * @brief Constant declaring initial extent size in blocks
 */
#define INITIAL_EXTENT_SIZE (iniparser_getint(AK_config,"extents:initial_extent_size",15))
/**
  * @def EXTENT_GROWTH_TABLE
  * @brief Constant declaring extent growth factor for tables
 */
#define EXTENT_GROWTH_TABLE (iniparser_getdouble(AK_config,"extents:extent_growth_table",0.5))
/**
  * @def EXTENT_GROWTH_INDEX
  * @brief Constant declaring extent growth factor for indices
 */
#define EXTENT_GROWTH_INDEX (iniparser_getdouble(AK_config,"extents:extent_growth_index",0.2))
/**
  * @def EXTENT_GROWTH_TRANSACTION
  * @brief Constant declaring extent growth factor for transaction segments
 */
#define EXTENT_GROWTH_TRANSACTION (iniparser_getdouble(AK_config,"extents:extent_growth_transaction",0.2))
/**
  * @def EXTENT_GROWTH_TEMP
  * @brief Constant declaring extent growth factor for temporary segments
 */
#define EXTENT_GROWTH_TEMP (iniparser_getdouble(AK_config,"extents:extent_growth_temp",0.5))
/**
 * @def MAX_REDO_LOG_MEMORY
 * @brief maximum size of REDO log memory
 */
#define MAX_REDO_LOG_MEMORY 255
/**
 * @def NUMBER_OF_THREADS
 * @brief Constant declaring maximum number of threads that an application can acquire
*/
#define NUMBER_OF_THREADS (iniparser_getint(AK_config,"general:number_of_threads",42))
/**
  * @def MAX_EXTENTS
  * @brief Constant declaring maximum number of extents for a given segment
 */
//#define MAX_EXTENTS 20


#endif
