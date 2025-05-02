/**************************************************
 * Title: DatabaseSystem - Project 2 Example
 * Summary: blah blah
 *  |Date              |Author             |Version
    |2024-06-07        |Seoyoung Shin       |1.0.0
**************************************************/
#include <stdio.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost"; // change if necessary
const char* user = "root";      // change if necessary
const char* pw = "okaydbgn030@";       // change if necessary

#define MAX_LEN			13000

int main(void) {
    MYSQL* connection = NULL;
    MYSQL conn;
    MYSQL_RES* sql_result;
    MYSQL_ROW sql_row;
    FILE* fp = fopen("CRUD.txt", "rt");    // open CRUD file.
    char line[MAX_LEN];

    if (mysql_init(&conn) == NULL)
        printf("mysql_init() error!");

    connection = mysql_real_connect(&conn, host, user, pw, NULL, 3306, (const char*)NULL, 0);   // the first NULL can be replaced to an existing db instance.
    if (connection == NULL) {
        printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
        return 1;
    }
    else {
        printf("Connection Succeed\n\n");

        while (fgets(line, sizeof(line), fp) != NULL) {
            if (!strcmp(line, "$$$\n"))      // read lines from CRUD file, '$$$' separates CREATE / DELETE parts.
                break;
            mysql_query(connection, line);
        }

        if (mysql_select_db(&conn, "project")) {
            printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
            return 1;
        }

        printf("----------SELECT QUERY TYPES----------\n");
        printf("\n");
        printf("    1. TYPE 1\n");
        printf("    2. TYPE 2\n");
        printf("    3. TYPE 3\n");
        printf("    4. TYPE 4\n");
        printf("    5. TYPE 5\n");
        printf("    6. TYPE 6\n");
        printf("    7. TYPE 7\n");
        printf("    0. QUIT\n");

        int selected_num, state = 0;;
        const char* query;

        while (true) {
            scanf("%d", &selected_num);
            if (!selected_num) break;
            else if (selected_num == 1) {
                query = "select * from location";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                }
                printf("[ SELECT * FROM location ]\n");
                while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                    printf(" %s  %s  %s  %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);

            }
            mysql_free_result(sql_result);
        }

        // comment out if you want to persist example db instance.
        while (fgets(line, sizeof(line), fp) != NULL)
            mysql_query(connection, line);			// these are DELETEs & DROPs.

        mysql_close(connection);
    }

    return 0;
}
