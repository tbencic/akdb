/**
@file theta_join.c Provides functions for relational theta join operation
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

#include "theta_join.h"

/**
 * @brief  Function for creating the header of the new table for theta join
 * @author Tomislav Mikulček
 * @param srcTable1 - name of the first table
 * @param srcTable2 - name of the second table
 * @param new_table - name of the destination table
 * @return int - returns EXIT_SUCCESS if the header was successfully created and EXIT_ERROR if the renamed headers are too long
 */
int create_theta_join_header(char *srcTable1, char * srcTable2, char *new_table) {

	table_addresses *src_addr1 = (table_addresses *) get_table_addresses(srcTable1);
	table_addresses *src_addr2 = (table_addresses *) get_table_addresses(srcTable2);

	int table_address1 = src_addr1->address_from[0];
	int table_address2 = src_addr2->address_from[0];

    AK_block *temp_block_tbl1 = (AK_block *) AK_read_block(table_address1);
    AK_block *temp_block_tbl2 = (AK_block *) AK_read_block(table_address2);

	//Currently it works with headers no longer than MAX_ATTRIBUTES. The same header is written in all allocated table blocks.
	//This is wrong and need to be corrected.
	//If header doesn't fit in the first block than system must write the remain attributes from header to the new block.
	//Correction must be handled in all functions that write, read or count header attributes.
	AK_header header[MAX_ATTRIBUTES];
    memset(header, 0, sizeof( AK_header ) * MAX_ATTRIBUTES);

    int head1 = 0; 		//counter of the heads
    int head2 = 0; 		//counter of the heads
    int new_head = 0; 	//counter of heads to write
    int rename = 0;		//indicate if we copy these header or not
    int length_tbl1 = strlen(srcTable1);
    int length_tbl2 = strlen(srcTable2);
    char *renamed_att;

    AK_list_elem list_elem;

    //first we copy all of the column names from the first table
    while (strcmp(temp_block_tbl1->header[head1].att_name, "") != 0) {

    	dbg_messg(HIGH, REL_OP, "Theta join: Copying attribute header: %s from first table", temp_block_tbl1->header[head1].att_name);
    	memcpy(&header[head1], &temp_block_tbl1->header[head1], sizeof (temp_block_tbl1->header[head1]));
    	head1++;
    }

    new_head = head1;

    //we start copying column names from the second table
    while (strcmp(temp_block_tbl2->header[head2].att_name, "") != 0) {

    	head1 = 0;

    	//we need to check if the names of the columns from the second table overlap with the names from the first table
    	while (strcmp(temp_block_tbl1->header[head1].att_name, "") != 0) {

			if (strcmp(temp_block_tbl1->header[head1].att_name, temp_block_tbl2->header[head2].att_name) == 0){
				rename = 1;
				break;
			}

			head1++;
    	}

    	dbg_messg(HIGH, REL_OP, "Theta join: Copying attribute header: %s from second table", temp_block_tbl2->header[head2].att_name);
    	memcpy(&header[new_head], &temp_block_tbl2->header[head2], sizeof(temp_block_tbl2->header[head2]));


    	//if an overlap is found, the column is renamed by prepending its table name and a dot.
    	if (rename){

    		dbg_messg(HIGH, REL_OP, "Theta join: renaming header: %s", temp_block_tbl2->header[head2].att_name);

    		renamed_att = malloc(length_tbl1 + strlen(temp_block_tbl1->header[head1].att_name) + 2);
    		memcpy(renamed_att, srcTable1, length_tbl1);
    		memcpy(renamed_att + length_tbl1, ".", strlen("."));
    		memcpy(renamed_att + length_tbl1 + 1, temp_block_tbl1->header[head1].att_name, strlen(temp_block_tbl1->header[head1].att_name) + 1);

    		if (strlen(renamed_att) > MAX_ATT_NAME)
    			return EXIT_ERROR;
    		else
    			memcpy(header[head1].att_name, renamed_att, strlen(renamed_att) + 1);

    		free(renamed_att);

    		renamed_att = malloc(length_tbl2 + strlen(temp_block_tbl2->header[head2].att_name) + 2);
    		memcpy(renamed_att, srcTable2, length_tbl2);
			memcpy(renamed_att + length_tbl2, ".", strlen("."));
			memcpy(renamed_att + length_tbl2 + 1, temp_block_tbl2->header[head2].att_name, strlen(temp_block_tbl2->header[head2].att_name) + 1);

			if (strlen(renamed_att) > MAX_ATT_NAME)
				return EXIT_ERROR;
			else
				memcpy(header[new_head].att_name, renamed_att, strlen(renamed_att) + 1);

			free(renamed_att);
			rename = 0;
    	}

    	new_head++;
		head2++;
    }

    free(temp_block_tbl1);
    free(temp_block_tbl2);

    AK_temp_create_table(new_table, header, SEGMENT_TYPE_TABLE);

    return EXIT_SUCCESS;
}

/**
 * @brief  Value comparison according to data type
 * @author Tomislav Mikulček
 * @param el - list element
 * @param *op - comparison operator
 * @param *a - left operand
 * @param *b - right operand
 * @return int - 0 if false, 1 if true
 */
int inequality_check(AK_list_elem el, const char *op, const void *a, const void *b) {
    switch (el->type) {
        case TYPE_INT:
            //printf("a:%i , b:%i\n", *((int *)a), *((int *)b));
            if ((strcmp(op, "<") && *(int *) a < *(int *) b) == 0)
                return 1;
            else if ((strcmp(op, ">") && *((int *) a) > *((int *) b)) == 0)
                return 1;
            else if ((strcmp(op, "<=") && *((int *) a) <= *((int *) b)) == 0)
                return 1;
            else if ((strcmp(op, ">=") && *((int *) a) >= *((int *) b)) == 0)
                return 1;
            else
                return 0;
            break;

        case TYPE_FLOAT:
            //printf("a:%f , b:%f\n", *((float *)a), *((float *)b));
            if ((strcmp(op, "<") && *((float *) a) < *((float *) b)) == 0)
                return 1;
            else if ((strcmp(op, ">") && *((float *) a) > *((float *) b)) == 0)
                return 1;
            else if ((strcmp(op, "<=") && *((float *) a) <= *((float *) b)) == 0)
                return 1;
            else if ((strcmp(op, ">=") && *((float *) a) >= *((float *) b)) == 0)
                return 1;
            else
                return 0;
            break;

        case TYPE_NUMBER:
            //printf("a:%f , b:%f\n", *((float *)a), *((float *)b));
            if ((strcmp(op, "<") && *((double *) a) < *((double *) b)) == 0)
                return 1;
            else if ((strcmp(op, ">") && *((double *) a) > *((double *) b)) == 0)
                return 1;
            else if ((strcmp(op, "<=") && *((double *) a) <= *((double *) b)) == 0)
                return 1;
            else if ((strcmp(op, ">=") && *((double *) a) >= *((double *) b)) == 0)
                return 1;
            else
                return 0;
            break;

        case TYPE_VARCHAR:
            //printf("a:%s , b:%s\n", (const char *)a, (const char *)b);
            if ((strcmp(op, "<") && strcmp((const char *) a, (const char *) b) < 0) == 0)
                return 1;
            else if ((strcmp(op, ">") && strcmp((const char *) a, (const char *) b) > 0) == 0)
                return 1;
            else if ((strcmp(op, "<=") && strcmp((const char *) a, (const char *) b) <= 0) == 0)
                return 1;
            else if ((strcmp(op, ">=") && strcmp((const char *) a, (const char *) b) >= 0) == 0)
                return 1;
            else
                return 0;
    }
}

/**
 * @brief Function iterates through blocks of the two tables and copies the rows which pass the constraint check into the new table
 * @author Tomislav Mikulček
 * @param tbl1_temp_block - block of the first table
 * @param tbl2_temp_block - block of the second join table
 * @param tbl1_num_att - number of attributes in the first table
 * @param tbl2_num_att - number of attributes in the second table
 * @param constraints - list of attributes, (in)equality and logical operators which are the conditions for the join in postfix notation
 * @param new_table - name of the theta_join table
 */
void check_constraints(AK_block *tbl1_temp_block, AK_block *tbl2_temp_block, int tbl1_num_att, int tbl2_num_att, AK_list *constraints, char *new_table) {
    dbg_messg(HIGH, REL_OP, "\n COPYING THETA JOIN");

    int tbl1_att, tbl2_att, tbl1_row, tbl2_row;
    int address, size, type, column;
    int true = 1, false = 0, result;
    int found;
    char data[MAX_VARCHAR_LENGTH];

    list *row_root = (list *) malloc(sizeof (list));
    list *row_root_insert = (list *) malloc(sizeof (list));
    AK_list *temp = (AK_list *) malloc(sizeof (AK_list));
    AK_list_elem list_elem, a, b;
    AK_header *t_header = (AK_header *) AK_get_header(new_table);

    InitializeList(row_root);
	InitializeList(row_root_insert);

    for (tbl1_row = 0; tbl1_row < DATA_BLOCK_SIZE; tbl1_row += tbl1_num_att){

    	if (tbl1_temp_block->tuple_dict[tbl1_row].type == FREE_INT)
			break;

    	for (tbl2_row = 0; tbl2_row < DATA_BLOCK_SIZE; tbl2_row += tbl2_num_att){

    		if (tbl2_temp_block->tuple_dict[tbl2_row].type == FREE_INT)
				break;
    		//@TODO theta join i selekcija imaju iste uvjete pa se provjera ograničenja može apstrahirati
    		list_elem = (AK_list_elem) FirstL(constraints);
    		while (list_elem) {

				switch(list_elem->type){

					case TYPE_ATTRIBS:

						found = 0;

						for (tbl1_att = 0; tbl1_att < tbl1_num_att; tbl1_att++){
							if (strcmp(list_elem->data, tbl1_temp_block->header[tbl1_att].att_name) == 0) {
								found = 1;
								address = tbl1_temp_block->tuple_dict[tbl1_row + tbl1_att].address;
								size = tbl1_temp_block->tuple_dict[tbl1_row + tbl1_att].size;
								type = tbl1_temp_block->tuple_dict[tbl1_row + tbl1_att].type;
								memset(data, 0, MAX_VARCHAR_LENGTH);
								memcpy(data, &(tbl1_temp_block->data[address]), size);
								InsertAtEndL(type, data, size, temp);
								break;
							}
						}

						if (!found) {

							for (tbl2_att = 0; tbl2_att < tbl2_num_att; tbl2_att++){
								if (strcmp(list_elem->data, tbl2_temp_block->header[tbl2_att].att_name) == 0) {
									found = 1;
									address = tbl2_temp_block->tuple_dict[tbl2_row + tbl2_att].address;
									size = tbl2_temp_block->tuple_dict[tbl2_row + tbl2_att].size;
									type = tbl2_temp_block->tuple_dict[tbl2_row + tbl2_att].type;
									memset(data, 0, MAX_VARCHAR_LENGTH);
									memcpy(data, &(tbl2_temp_block->data[address]), size);
									InsertAtEndL(type, data, size, temp);
									break;
								}
							}
						}

						if (!found)
							return;

						break;

					case TYPE_OPERATOR:

						b = (AK_list_elem) EndL(temp);
						a = (AK_list_elem) PreviousL(b, temp);

						if (strcmp(list_elem->data, "=") == 0) {

							if (memcmp(a->data, b->data, a->size) == 0)
								InsertAtEndL(TYPE_INT, &true, sizeof (int), temp);
							else
								InsertAtEndL(TYPE_INT, &false, sizeof (int), temp);

						} else if (strcmp(list_elem->data, "<>") == 0) {

							if (memcmp(a->data, b->data, a->size) != 0)
								InsertAtEndL(TYPE_INT, &true, sizeof (int), temp);
							else
								InsertAtEndL(TYPE_INT, &false, sizeof (int), temp);

						} else if (strcmp(list_elem->data, "OR") == 0) {

							int val_a, val_b;
							memcpy(&val_a, a->data, sizeof (int));
							memcpy(&val_b, b->data, sizeof (int));

							if (val_a || val_b)
								InsertAtEndL(TYPE_INT, &true, sizeof (int), temp);
							else
								InsertAtEndL(TYPE_INT, &false, sizeof (int), temp);

						} else if (strcmp(list_elem->data, "AND") == 0) {

							int val_a, val_b;
							memcpy(&val_a, a->data, sizeof (int));
							memcpy(&val_b, b->data, sizeof (int));

							if (val_a && val_b)
								InsertAtEndL(TYPE_INT, &true, sizeof (int), temp);
							else
								InsertAtEndL(TYPE_INT, &false, sizeof (int), temp);

						} else {

							int rs;
							rs = inequality_check(b, list_elem->data, a->data, b->data);
							InsertAtEndL(TYPE_INT, &rs, sizeof (int), temp);
						}

						DeleteL(a, temp);
						DeleteL(b, temp);
						break;

					default:
						InsertAtEndL(list_elem->type, list_elem->data, list_elem->size, temp);
				}

				list_elem = list_elem->next;
			}

    		memcpy(&result, ((AK_list_elem) FirstL(temp))->data, sizeof (int));
    		DeleteAllL(temp);

    		if (result){

    			column = 0;

    			for (tbl1_att = 0; tbl1_att < tbl1_num_att; tbl1_att++){
					address = tbl1_temp_block->tuple_dict[tbl1_row + tbl1_att].address;
					size = tbl1_temp_block->tuple_dict[tbl1_row + tbl1_att].size;
					type = tbl1_temp_block->tuple_dict[tbl1_row + tbl1_att].type;
					memset(data, 0, MAX_VARCHAR_LENGTH);
					memcpy(data, &(tbl1_temp_block->data[address]), size);
					InsertNewElement(type, data, new_table, t_header[column].att_name, row_root);
					column++;
    			}

    			for (tbl2_att = 0; tbl2_att < tbl2_num_att; tbl2_att++){
					address = tbl2_temp_block->tuple_dict[tbl2_row + tbl2_att].address;
					size = tbl2_temp_block->tuple_dict[tbl2_row + tbl2_att].size;
					type = tbl2_temp_block->tuple_dict[tbl2_row + tbl2_att].type;
					memset(data, 0, MAX_VARCHAR_LENGTH);
					memcpy(data, &(tbl2_temp_block->data[address]), size);
					InsertNewElement(type, data, new_table, t_header[column].att_name, row_root);
					column++;
				}

    			insert_row(row_root);
				DeleteAllElements(row_root);
    		}
    	}
    }

    free(row_root);
    free(row_root_insert);
}

/**
 * @brief Function for creating a theta join betwen two tables on specified conditions
 * @author Tomislav Mikulček
 * @param srcTable1 - name of the first table to join
 * @param srcTable2 - name of the second table to join
 * @param constraints - list of attributes, (in)equality and logical operators which are the conditions for the join in postfix notation
 * @param dstTable - name of the theta join table
 * @return int - if successful returns EXIT_SUCCESS and EXIT_ERROR otherwise
 */
int AK_theta_join(char *srcTable1, char * srcTable2, char * dstTable, AK_list *constraints) {

	table_addresses *src_addr1 = (table_addresses *) get_table_addresses(srcTable1);
    table_addresses *src_addr2 = (table_addresses *) get_table_addresses(srcTable2);

    int startAddress1 = src_addr1->address_from[0];
    int startAddress2 = src_addr2->address_from[0];
    int tbl1_num_att = AK_num_attr(srcTable1);
    int tbl2_num_att = AK_num_attr(srcTable2);

    if ((startAddress1 != 0) && (startAddress2 != 0)) {
        if (create_theta_join_header(srcTable1, srcTable2, dstTable) == EXIT_ERROR)
        	return EXIT_ERROR;

        dbg_messg(LOW, REL_OP, "\nTABLE %s CREATED from %s and %s\n", dstTable, srcTable1, srcTable2);
		dbg_messg(MIDDLE, REL_OP, "\nAK_theta_join: start copying data\n");

        AK_mem_block *tbl1_temp_block, *tbl2_temp_block;

        int i, j, k, l, m;
        i = j = k = l = m = 0;

        //for each extent in table1 that contains blocks needed for join
        for (i = 0; (i < src_addr1->address_from[i]) != 0; i++) {
            startAddress1 = src_addr1->address_from[i];

            if (startAddress1 != 0) {
                dbg_messg(MIDDLE, REL_OP, "\nTheta join: copying extent of table 1: %d\n", i);

                //for each block in table1 extent
                for (j = startAddress1; j < src_addr1->address_to[i]; j++) {
                    dbg_messg(MIDDLE, REL_OP, "Theta join: copying block of table 1: %d\n", j);

                    tbl1_temp_block = (AK_mem_block *) AK_get_block(j);

                    //if there is data in the block
                    if (tbl1_temp_block->block->free_space != 0) {

                        //for each extent in table2 that contains blocks needed for join
                        for (k = 0; (k < src_addr2->address_from[k]) != 0; k++) {
                            startAddress2 = src_addr2->address_from[k];

                            if (startAddress2 != 0) {
                                dbg_messg(MIDDLE, REL_OP, "Theta join: copying extent of table 2: %d\n", k);

                                //for each block in table2 extent
                                for (l = startAddress2; l < src_addr2->address_to[k]; l++) {
                                    dbg_messg(MIDDLE, REL_OP, "Theta join: copying block of table 2: %d\n", l);

                                    tbl2_temp_block = (AK_mem_block *) AK_get_block(l);

                                    //if there is data in the block
                                    if (tbl2_temp_block->block->free_space != 0) {

                                    		check_constraints(tbl1_temp_block->block, tbl2_temp_block->block, tbl1_num_att, tbl2_num_att, constraints, dstTable);
                                    }
                                }
                            } else break;
                        }
                    }
                }
            } else break;
        }

        free(src_addr1);
        free(src_addr2);

		dbg_messg(LOW, REL_OP, "THETA_JOIN_SUCCESS\n\n");

        return EXIT_SUCCESS;
    } else {

        dbg_messg(LOW, REL_OP, "\n AK_theta_join: Table/s doesn't exist!");

        free(src_addr1);
        free(src_addr2);

        return EXIT_ERROR;
    }
}

void op_theta_join_test() {
    printf("\n********** THETA JOIN TEST **********\n\n");

    AK_list *constraints = (AK_list *) malloc(sizeof (AK_list));
    InitL(constraints);

    //test where no column names overlap
    printf("SELECT * FROM department, professor WHERE manager = lastname;\n");
    InsertAtEndL(TYPE_ATTRIBS, "manager", sizeof ("manager"), constraints);
    InsertAtEndL(TYPE_ATTRIBS, "lastname", sizeof ("lastname"), constraints);
    InsertAtEndL(TYPE_OPERATOR, "=", sizeof ("="), constraints);

    AK_theta_join("department", "professor", "theta_join_test", constraints);
    AK_print_table("theta_join_test");

    DeleteAllL(constraints);

    //test where overlaping columns are not a part of the constraints
    printf("SELECT * FROM student, professor2 WHERE id_prof = mbr;\n");
    InsertAtEndL(TYPE_ATTRIBS, "id_prof", sizeof ("id_prof"), constraints);
    InsertAtEndL(TYPE_ATTRIBS, "mbr", sizeof ("mbr"), constraints);
    InsertAtEndL(TYPE_OPERATOR, "=", sizeof ("="), constraints);

    AK_theta_join("student", "professor2", "theta_join_test2", constraints);
    AK_print_table("theta_join_test2");

    DeleteAllL(constraints);
    free(constraints);
}
