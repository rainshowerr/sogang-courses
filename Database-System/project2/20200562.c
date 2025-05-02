/**************************************************
 * Title: DatabaseSystem - Project 2 Example
 * Summary: -_- commands out of sync -_-
 *  |Date              |Author             |Version
    |2024-06-07        |Seoyoung Shin       |1.0.0
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost"; // change if necessary
const char* user = "root";      // change if necessary
const char* pw = "abcd1234";       // change if necessary

#define MAX_LEN			13000

char* strjoin(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1);

    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

char* random_id_generator(MYSQL* connection) {
    MYSQL_RES* sql_result;
    MYSQL_ROW sql_row;
    int state, cnt = 0;

    srand((unsigned int)time(NULL));
    char* random_id = (char*)malloc((11) * sizeof(char));
    char query[500];

    do {
        // random number 생성
        for (int i = 0; i < 10; i++)
            random_id[i] = rand() % 10 + '0';
        random_id[10] = '\0';

        // 기존 id와 중복되는지 확인
        snprintf(query, sizeof(query), "SELECT COUNT(*) FROM property WHERE prop_id='%s'", random_id);
        state = mysql_query(connection, query);
        if (state == 0) {
            sql_result = mysql_store_result(connection);
            sql_row = mysql_fetch_row(sql_result);
            cnt += atoi(sql_row[0]);
            mysql_free_result(sql_result);
        }
        snprintf(query, sizeof(query), "SELECT COUNT(*) FROM agent WHERE agent_id='%s'", random_id);
        state = mysql_query(connection, query);
        if (state == 0) {
            sql_result = mysql_store_result(connection);
            sql_row = mysql_fetch_row(sql_result);
            cnt += atoi(sql_row[0]);
            mysql_free_result(sql_result);
        }
        snprintf(query, sizeof(query), "SELECT COUNT(*) FROM seller WHERE seller_id='%s'", random_id);
        state = mysql_query(connection, query);
        if (state == 0) {
            sql_result = mysql_store_result(connection);
            sql_row = mysql_fetch_row(sql_result);
            cnt += atoi(sql_row[0]);
            mysql_free_result(sql_result);
        }
        snprintf(query, sizeof(query), "SELECT COUNT(*) FROM buyer WHERE buyer_id='%s'", random_id);
        state = mysql_query(connection, query);
        if (state == 0) {
            sql_result = mysql_store_result(connection);
            sql_row = mysql_fetch_row(sql_result);
            cnt += atoi(sql_row[0]);
            mysql_free_result(sql_result);
        }
        snprintf(query, sizeof(query), "SELECT COUNT(*) FROM transaction WHERE trans_id='%s'", random_id);
        state = mysql_query(connection, query);
        if (state == 0) {
            sql_result = mysql_store_result(connection);
            sql_row = mysql_fetch_row(sql_result);
            cnt += atoi(sql_row[0]);
            mysql_free_result(sql_result);
        }
    } while (cnt != 0);

    return random_id;
}

int main(void) {
    MYSQL* connection = NULL;
    MYSQL conn;
    MYSQL_RES* sql_result;
    MYSQL_ROW sql_row;
    FILE* fp = fopen("CRUD.txt", "rt");    // open CRUD file.
    int choice, state = 0;;
    char* query;
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
            state = mysql_query(connection, line);
        }

        if (mysql_select_db(&conn, "project")) {
            printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
            return 1;
        }

        while (true) {
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
            scanf("%d", &choice);
            if (!choice) break;
            else if (choice == 1) {
                query = "select * from property where sale_status='TRUE' and loc_name='Mapo';";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    printf("** Fine the address of homes for sale in the district Mapo **\n");
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                        printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
                    mysql_free_result(sql_result);
                }
                printf("\n");

                while (true) {
                    printf("---------- Subtypes in TYPE 1 ----------\n");
                    printf("    1. TYPE 1-1\n");
                    scanf("%d", &choice);
                    if (choice == 0)
                        break;
                    else if (choice == 1) {
                        query = "select * from property where sale_status='TRUE' and listing_price >= 100000000 and listing_price <= 150000000;";
                        state = mysql_query(connection, query);
                        if (state == 0) {
                            sql_result = mysql_store_result(connection);
                            printf("** Find the costing between \1,000,000 and \1,500,000,000 **\n");
                            while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                                printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
                            mysql_free_result(sql_result);
                        }
                    }
                    printf("\n");
                }
            }
            else if (choice == 2) {
                query = "select * from property natural join location where sale_status='TRUE' and district_num = '8';";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    printf("** Find the address of homes for sale in the 8th school district. **\n");
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                        printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
                    mysql_free_result(sql_result);
                }
                printf("\n");

                while (true) {
                    printf("---------- Subtypes in TYPE 2 ----------\n");
                    printf("    1. TYPE 2-1\n");
                    scanf("%d", &choice);
                    if (choice == 0)
                        break;
                    else if (choice == 1) {
                        query = "select * from property natural join location where sale_status='TRUE' and district_num = '8' and num_of_bedrooms >= 4 and num_of_bathrooms = 2;";
                        state = mysql_query(connection, query);
                        if (state == 0) {
                            sql_result = mysql_store_result(connection);
                            printf("** Then find properties with 4 or more bedrooms and 2 bathrooms. **\n");
                            while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                                printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
                            mysql_free_result(sql_result);
                        }
                    }
                    printf("\n");
                }
            }
            else if (choice == 3) {
                query = "SELECT a.agent_id, a.agent_name, SUM(t.sale_price) AS total_sales_price \
                         FROM agent a JOIN transaction t ON a.agent_id = t.selling_agent_id \
                         WHERE YEAR(t.sale_end_date) = 2022 \
                         GROUP BY a.agent_id, a.agent_name \
                         ORDER BY total_sales_price DESC LIMIT 1;";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    printf("** Find the name of the agent who has sold the most properties in the year 2022 by total won value. **\n");
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                        printf("%s %s %s\n", sql_row[0], sql_row[1], sql_row[2]);
                    mysql_free_result(sql_result);
                }
                printf("\n");
                while (true) {
                    printf("---------- Subtypes in TYPE 3 ----------\n");
                    printf("    1. TYPE 3-1\n");
                    printf("    2. TYPE 3-2\n");
                    scanf("%d", &choice);
                    if (choice == 0)
                        break;
                    else if (choice == 1) {
                        printf("** Then find the top k agents in the year 2023 by total won value. **\n");
                        printf("Which K ? : ");
                        int k; scanf("%d", &k); char k_str[10];
                        sprintf(k_str, "%d;", k);

                        query = strjoin("SELECT a.agent_id, a.agent_name, SUM(t.sale_price) AS total_sales_price \
                                    FROM agent a JOIN transaction t ON a.agent_id = t.selling_agent_id \
                                    WHERE YEAR(t.sale_end_date) = 2023 \
                                    GROUP BY a.agent_id, a.agent_name \
                                    ORDER BY total_sales_price DESC LIMIT ", k_str);
                        state = mysql_query(connection, query);
                        free(query);
                        if (state == 0) {
                            sql_result = mysql_store_result(connection);
                            printf("** Then find the top k agents in the year 2023 by total won value. **\n");
                            while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                                printf("%s %s %s\n", sql_row[0], sql_row[1], sql_row[2]);
                            mysql_free_result(sql_result);
                        }
                        printf("\n");
                    }
                    else if (choice == 2) {
                        state = mysql_query(connection, "SELECT COUNT(DISTINCT selling_agent_id) FROM transaction WHERE YEAR(sale_end_date) = 2021; ");
                        if (state == 0) {
                            sql_result = mysql_store_result(connection);
                            sql_row = mysql_fetch_row(sql_result);
                            int k = (int)(atoi(sql_row[0]) * 0.1);
                            mysql_free_result(sql_result);
                            char k_str[10];
                            sprintf(k_str, "%d;", k);

                            query = strjoin("SELECT a.agent_id, a.agent_name, SUM(t.sale_price) AS total_sales_price \
                                            FROM agent a JOIN transaction t ON a.agent_id = t.selling_agent_id \
                                            WHERE YEAR(t.sale_end_date) = 2021 \
                                            GROUP BY a.agent_id, a.agent_name \
                                            ORDER BY total_sales_price ASC LIMIT ", k_str);
                            state = mysql_query(connection, query);
                            free(query);
                            if (state == 0) {
                                sql_result = mysql_store_result(connection);
                                printf("** And then find the bottom 10 percent agents in the year 2021 by total won value. **\n");
                                while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                                    printf("%s %s %s\n", sql_row[0], sql_row[1], sql_row[2]);
                                mysql_free_result(sql_result);
                            }
                        }
                        printf("\n");
                    }
                }
            }
            else if (choice == 4) {
                query = "SELECT a.agent_id, a.agent_name, AVG(t.sale_price) AS avg_sale_price, AVG(DATEDIFF(t.sale_end_date, p.sale_start_date)) AS avg_sale_duration \
                        FROM agent a JOIN transaction t ON a.agent_id = t.selling_agent_id JOIN property p ON t.prop_id = p.prop_id \
                        WHERE YEAR(t.sale_end_date) = 2022 \
                        GROUP BY a.agent_id, a.agent_name \
                        ORDER BY avg_sale_price DESC;";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    printf("** For each agent, compute the average selling price of properties sold in 2022, and the average time the  property was on the market. **\n");
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                        printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
                    mysql_free_result(sql_result);
                }
                printf("\n");

                while (true) {
                    printf("---------- Subtypes in TYPE 4 ----------\n");
                    printf("    1. TYPE 4-1\n");
                    printf("    2. TYPE 4-2\n");
                    scanf("%d", &choice);
                    if (choice == 0)
                        break;
                    else if (choice == 1) {
                        query = "SELECT a.agent_id, a.agent_name, MAX(t.sale_price) AS max_sale_price \
                                FROM agent a JOIN transaction t ON a.agent_id = t.selling_agent_id \
                                WHERE YEAR(t.sale_end_date) = 2023 \
                                GROUP BY a.agent_id, a.agent_name \
                                ORDER BY max_sale_price DESC;";
                        state = mysql_query(connection, query);
                        if (state == 0) {
                            sql_result = mysql_store_result(connection);
                            printf("** Then compute the maximum selling price of properties sold in 2023 for each agent. **\n");
                            while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                                printf("%s %s %s\n", sql_row[0], sql_row[1], sql_row[2]);
                            mysql_free_result(sql_result);
                        }
                        printf("\n");
                    }
                    else if (choice == 2) {
                        query = "SELECT a.agent_id, a.agent_name, MAX(DATEDIFF(t.sale_end_date, p.sale_start_date)) AS max_days_on_market \
                                FROM agent a JOIN transaction t ON a.agent_id = t.selling_agent_id JOIN property p ON t.prop_id = p.prop_id \
                                GROUP BY a.agent_id, a.agent_name \
                                ORDER BY max_days_on_market ASC;";
                        state = mysql_query(connection, query);
                        if (state == 0) {
                            sql_result = mysql_store_result(connection);
                            printf("** And then compute the longest time the property was on the market for each agent. **\n");
                            while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                                printf("%s %s %s\n", sql_row[0], sql_row[1], sql_row[2]);
                            mysql_free_result(sql_result);
                        }
                        printf("\n");
                    }
                }
            }
            else if (choice == 5) {
                printf("** Show photos of the most expensive studio, one - bedroom, \
multi - bedroom apartment(s), and detached house(s), respectively, from the database. **\n");
                query = "SELECT p.prop_id, p.prop_type, p.listing_price, \
                        ip.in_photo_data, ep.ex_photo_data, fp.fl_photo_data  \
                        FROM property p \
                        LEFT OUTER JOIN in_photo ip ON p.prop_id = ip.prop_id \
                        LEFT OUTER JOIN ex_photo ep ON p.prop_id = ep.prop_id \
                        LEFT OUTER JOIN fl_photo fp ON p.prop_id = fp.prop_id \
                        WHERE p.prop_type = 'studio'\
                        ORDER BY p.listing_price DESC \
                        LIMIT 1;";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                        printf("%s %s %s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
                    mysql_free_result(sql_result);
                }

                query = "SELECT p.prop_id, p.prop_type, p.listing_price, \
                        ip.in_photo_data, ep.ex_photo_data, fp.fl_photo_data  \
                        FROM property p \
                        LEFT OUTER JOIN in_photo ip ON p.prop_id = ip.prop_id \
                        LEFT OUTER JOIN ex_photo ep ON p.prop_id = ep.prop_id \
                        LEFT OUTER JOIN fl_photo fp ON p.prop_id = fp.prop_id \
                        WHERE p.prop_type = 'one-bedroom apartments'\
                        ORDER BY p.listing_price DESC \
                        LIMIT 1;";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                        printf("%s %s %s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
                    mysql_free_result(sql_result);
                }

                query = "SELECT p.prop_id, p.prop_type, p.listing_price, \
                        ip.in_photo_data, ep.ex_photo_data, fp.fl_photo_data  \
                        FROM property p \
                        LEFT OUTER JOIN in_photo ip ON p.prop_id = ip.prop_id \
                        LEFT OUTER JOIN ex_photo ep ON p.prop_id = ep.prop_id \
                        LEFT OUTER JOIN fl_photo fp ON p.prop_id = fp.prop_id \
                        WHERE p.prop_type = 'multi-bedroom apartments'\
                        ORDER BY p.listing_price DESC \
                        LIMIT 1;";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                        printf("%s %s %s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
                    mysql_free_result(sql_result);
                }

                query = "SELECT p.prop_id, p.prop_type, p.listing_price, \
                        ip.in_photo_data, ep.ex_photo_data, fp.fl_photo_data  \
                        FROM property p \
                        LEFT OUTER JOIN in_photo ip ON p.prop_id = ip.prop_id \
                        LEFT OUTER JOIN ex_photo ep ON p.prop_id = ep.prop_id \
                        LEFT OUTER JOIN fl_photo fp ON p.prop_id = fp.prop_id \
                        WHERE p.prop_type = 'detached houses'\
                        ORDER BY p.listing_price DESC \
                        LIMIT 1;";
                state = mysql_query(connection, query);
                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
                        printf("%s %s %s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
                    mysql_free_result(sql_result);
                }
                printf("\n");
            }
            else if (choice == 6) {
                char* trans_id, prop_id[20], seller_id[20], buyer_id[20], selling_agent_id[20], buyer_agent_id[20], sale_end_date[20];
                trans_id = random_id_generator(connection);
                printf("** Record the sale of a property that had been listed as being available. **\n");
                printf("ex. id = 1234567890, date = yyyy-mm-dd.\n");
                query = (char*)malloc(sizeof(char) * 500);
                printf("1. property id : ");
                scanf("%s", prop_id);
                snprintf(query, 500, "SELECT COUNT(*) FROM property WHERE prop_id='%s' AND sale_status = 'TRUE';", prop_id);
                state = mysql_query(connection, query);
                sql_result = mysql_store_result(connection);
                sql_row = mysql_fetch_row(sql_result);
                // listing된 부동산이 아닐 경우 다시 입력받기
                while (sql_row && atoi(sql_row[0]) == 0) {
                    mysql_free_result(sql_result);
                    printf("Invalid property ID. Try again. : ");
                    scanf("%s", prop_id);
                    snprintf(query, 500, "SELECT COUNT(*) FROM property WHERE prop_id='%s' AND sale_status = 'TRUE';", prop_id);
                    state = mysql_query(connection, query);
                    sql_result = mysql_store_result(connection);
                    sql_row = mysql_fetch_row(sql_result);
                }
                printf("2. seller id : ");
                scanf("%s", seller_id);
                printf("3. buyer id : ");
                scanf("%s", buyer_id);
                printf("4. selling agent id : ");
                scanf("%s", selling_agent_id);
                printf("5. buyer's agent id (If not, please enter 'n') : ");
                scanf("%s", buyer_agent_id);
                printf("6. date : ");
                scanf("%s", sale_end_date);
                mysql_query(connection, "START TRANSACTION;");
                // transaction 테이블에 추가
                snprintf(query, 500, "INSERT INTO transaction VALUES ('%s', '%s', '%s', '%s', (SELECT listing_price FROM property WHERE prop_id='%s'), '%s', '%s');", \
                    trans_id, seller_id, buyer_id, prop_id, prop_id, sale_end_date, selling_agent_id);
                free(trans_id);
                state = mysql_query(connection, query);
                if (state != 0) {
                    mysql_query(connection, "ROLLBACK;");
                    free(query);
                    printf("seller, buyer, selling_agent, buyer's agent 중 등록되어 있지 않은 고객이 있어 메인으로 돌아갑니다. 등록 후 다시 시도해주세요.\n");
                    continue;
                }
                // 구매자측 중개사가 존재한다면 buyer_agent 테이블에 추가
                if (strcmp(buyer_agent_id, "n") && strcmp(buyer_agent_id, "N")) {
                    snprintf(query, 500, "INSERT INTO buyer_agent VALUES ('%s', '%s');", trans_id, buyer_agent_id);
                    state = mysql_query(connection, query);
                    if (state != 0) {
                        mysql_query(connection, "ROLLBACK;");
                        free(query);
                        printf("agent 테이블에 해당 agent가 존재하지 않아 메인으로 돌아갑니다. agent를 먼저 추가해주세요.\n");
                        continue;
                    }
                }
                // property 테이블의 sale_status를 false(판매완료)로 업데이트
                snprintf(query, 500, "UPDATE property SET sale_status = 'FALSE' WHERE prop_id = '%s';", prop_id); // update 쿼리는 result를 free해줄 필요가 없다.
                state = mysql_query(connection, query);
                if (state != 0) {
                    mysql_query(connection, "ROLLBACK;");
                    free(query);
                    printf("property 테이블의 sale_status를 변경하는 도중 오류가 발생해 메인으로 돌아갑니다.\n");
                    continue;
                }
                state = mysql_query(connection, "COMMIT;");
                printf("Successfully updated.\n\n");
                free(query);
            }
            else if (choice == 7) {
                char agent_name[21], agent_phone_number[21], agent_address[101];
                query = (char*)malloc(sizeof(char) * 500);

                printf("Agent name : ");
                scanf("%s", agent_name);
                printf("Agent phone number : ");
                scanf("%s", agent_phone_number);
                printf("Agent address : ");
                scanf("%s", agent_address);

                snprintf(query, 500, "SELECT COUNT(*) FROM agent WHERE agent_phone_number='%s'", agent_phone_number);
                state = mysql_query(connection, query);

                if (state == 0) {
                    sql_result = mysql_store_result(connection);
                    sql_row = mysql_fetch_row(sql_result);
                    if (sql_row && atoi(sql_row[0]) == 0) {
                        snprintf(query, 500, "INSERT INTO agent VALUES ('%s', '%s', '%s', '%s');", \
                            random_id_generator(connection), agent_name, agent_phone_number, agent_address);
                        state = mysql_query(connection, query);
                        if (state == 0)
                            printf("Agent가 성공적으로 추가되었습니다.\n");
                        else
                            printf("Agent 추가를 실패했습니다.\n");
                    }
                    else
                        printf("이미 가입된 agent입니다.\n");
                    mysql_free_result(sql_result);
                }
                free(query);
                printf("\n");
            }
        }

        // comment out if you want to persist example db instance.
        while (fgets(line, sizeof(line), fp) != NULL)
            mysql_query(connection, line);			// these are DELETEs & DROPs.

        mysql_close(connection);

        return 0;
    }
}
