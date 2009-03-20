/**
@file configuration.h Defines global macros and variables
*/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */


#ifndef CONFIGURATION
#define CONFIGURATION

/**
\def EXIT_SUCCESS
\brief succesful exit return variable
*/
#define EXIT_SUCCESS 0

/**
\def EXIT_ERROR
\brief unsuccesfull exit return variable
*/
#define EXIT_ERROR 1

/**
\def BLOCK_TYPE_FREE
\brief free block type (used in KK_block->type)
*/
#define BLOCK_TYPE_FREE -1

/**
\def BLOCK_TYPE_NORMAL
\brief normal block type e.g. used by some extent (used in KK_block->type)
*/
#define BLOCK_TYPE_NORMAL 0

/**
\def BLOCK_TYPE_CHAINED
\brief chained block type e.g. used if the block is chained with another (used in KK_block->type)
*/
#define BLOCK_TYPE_CHAINED 1

/**
\def NOT_CHAINED
\brief used in KK_block->chained_with if the block isn't chained
*/
#define NOT_CHAINED -1

/**
\def FREE_INT
\brief dummy data for empty integers
*/
#define FREE_INT -1

/**
\def FREE_CHAR
\brief dummy data for empty chars
*/
#define FREE_CHAR '\0'

/**
\def DB_FILE
\brief name of the main DB file
*/
#define DB_FILE "kalashnikov.db"

/**
\def MAX_EXTENTS
\brief maximum number of extents for a given segment
*/
#define MAX_EXTENTS 20

/**
\def MAX_ATTRIBUTES
\brief maximum number of attributes per block
*/
#define MAX_ATTRIBUTES 10

/**
\def MAX_ATT_NAME
\brief maximum length of attribute name string (used in KK_header->att_name)
*/
#define MAX_ATT_NAME 255

/**
\def MAX_CONSTRAINTS
\brief maximum number of constraints per attribute
*/
#define MAX_CONSTRAINTS 5

/**
\def MAX_CONSTR_NAME
\brief maximum length of constraint name string (used in KK_header->constr_name)
*/
#define MAX_CONSTR_NAME 255

/**
\def MAX_CONSTR_CODE
\brief maximum lenght of constraint code string
*/
#define MAX_CONSTR_CODE 255

/**
\def DATA_BLOCK_SIZE
\brief length of data block size (used in in KK_block->data)
*/
#define DATA_BLOCK_SIZE 500

/**
\def DATA_ENTRY_SIZE
\brief lenght of data entry in sizeof( int )
*/
#define DATA_ENTRY_SIZE 10

/**
\def DB_FILE_SIZE
\brief size of DB file in MiB
*/
#define DB_FILE_SIZE 200

/**
\def INITIAL_EXTENT_SIZE
\brief initial extent size in blocks
*/
#define INITIAL_EXTENT_SIZE 20

/**
\def EXTENT_GROWTH_TABLE
\brief extent growth factor for tables
*/
#define EXTENT_GROWTH_TABLE 0.5

/**
\def EXTENT_GROWTH_INDEX
\brief extent growth factor for indices
*/
#define EXTENT_GROWTH_INDEX 0.2

/**
\def EXTENT_GROWTH_TRANSACTION
\brief extent growth factor for transaction segments
*/
#define EXTENT_GROWTH_TRANSACTION 0.2

/**
\def EXTENT_GROWTH_TEMP
\brief extent growth factor for temporary segments
*/
#define EXTENT_GROWTH_TEMP 0.5

/**
\def SEGMENT_TYPE_SYSTEM_TABLE
\brief system table segment type (used in system catalog)
*/
#define SEGMENT_TYPE_SYSTEM_TABLE 0

/**
\def SEGMENT_TYPE_TABLE
\brief table segment type (used in system catalog)
*/
#define SEGMENT_TYPE_TABLE 1

/**
\def SEGMENT_TYPE_INDEX
\brief index segment type (used in system catalog)
*/
#define SEGMENT_TYPE_INDEX 2

/**
\def SEGMENT_TYPE_TRANSACTION
\brief transaction segment type (used in system catalog)
*/
#define SEGMENT_TYPE_TRANSACTION 3

/**
\def SEGMENT_TYPE_TEMP
\brief temporary segment type (used in system catalog)
*/
#define SEGMENT_TYPE_TEMP 4

/**
\def TYPE_INTERNAL
\brief internal data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_INTERNAL 0

/**
\def TYPE_INT
\brief integer data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_INT 1

/**
\def TYPE_FLOAT
\brief float data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_FLOAT 2

/**
\def TYPE_NUMBER
\brief number data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_NUMBER 3

/**
\def TYPE_VARCHAR
\brief varchar data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_VARCHAR 4

/**
\def TYPE_DATE
\brief date data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_DATE 5

/**
\def TYPE_DATETIME
\brief datetime data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_DATETIME 6

/**
\def TYPE_TIME
\brief time data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_TIME 7

/**
\def TYPE_BLOB
\brief blob data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_BLOB 8

/**
\def TYPE_BOOL
\brief boolean data type (used in KK_header->type and KK_tuple_dict->type)
*/
#define TYPE_BOOL 9

#endif
 
