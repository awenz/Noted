#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define BUFSIZE 4096


int getline(char *line,int max){
    if (fgets(line, max, stdin) == NULL)
        return 0;
    else
        return strlen(line);
}

void print_usage(void){
    printf("Usage: noted -[options] <text> \n options:\n  -n new note\n  -l list notes\n");
}

void create_table(sqlite3 *handle){
    char create_table[100] = "CREATE TABLE IF NOT EXISTS notes (nnmuber INTEGER PRIMARY KEY ,note TEXT NOT NULL)";
    int retval = sqlite3_exec(handle,create_table,0,0,0);
    if(retval){
        fputs("Table creation failed! \n",stderr);
        exit(EXIT_FAILURE);
    }
}

void create_note(char *note,char *tmp, sqlite3 *handle, sqlite3_stmt *stmt){
    int retval = sqlite3_open("sampledb.sqlite",&handle);
    create_table(handle);
    int i;
    if(retval){
        fputs("Database connection failed\n",stderr);
        exit(EXIT_FAILURE);
    }
    fputs("Database connection successful! \n",stdout);
    char *query_start="INSERT INTO notes VALUES(NULL,'";
    /*char *query_mid="','";*/
    char *query_end="')";
    tmp = (char *)malloc(strlen(query_start) + strlen(note) + strlen(query_end));
    strcpy(tmp, query_start);
    strcat(tmp, note);
    strcat(tmp, query_end);
    retval = sqlite3_exec(handle,tmp,0,0,0);
    if(retval){
        fputs("Note creation failed!\n",stderr);
        exit(EXIT_FAILURE);
    }
}

void list_notes(sqlite3 *handle,sqlite3_stmt *stmt){
    char *select = "SELECT * from notes";
    int cols,col;
    int retval = sqlite3_open("sampledb.sqlite",&handle);
    if(retval){
        fputs("Database Connection failed\n",stdout);
        exit(EXIT_FAILURE);
    }
    retval = sqlite3_prepare_v2(handle,select,-1,&stmt,0);
    if(retval){
        fputs("Selecting Data from DB failed\n",stdout);
        exit(EXIT_FAILURE);
    }
    cols = sqlite3_column_count(stmt);
    while(1)
    {
        retval = sqlite3_step(stmt);
        
        if(retval == SQLITE_ROW)
        {
            
            for(col=0 ; col<cols;col++)
            {
                const char *val = (const char*)sqlite3_column_text(stmt,col);
                printf("%s = %s\t",sqlite3_column_name(stmt,col),val);
            }
            printf("\n");
        }
        else if(retval == SQLITE_DONE)
        {
            printf("All rows fetched\n");
            break;
        }
        else
        {
            printf("Some error encountered\n");
        }
    }
}

int main(int argc, char** argv)
{
    sqlite3 *handle;
    sqlite3_stmt *stmt;
    int retval;
    char *tmp;
    char c;
    char line[BUFSIZE];
    if( argc == 1 ){
        print_usage();
        exit(EXIT_FAILURE);
    }

    while (--argc > 0 && (*++argv)[0] == '-')
        while (c = *++argv[0])
            switch(c) {
                case 'n':
                    getline(line,BUFSIZE);
                    create_note(line,tmp,handle,stmt); 
                    break;
                case 'd':
                    break;
                case 'l':
                    list_notes(handle,stmt);
                    break;
                default:
                    print_usage();
                    break;
            }
    
    sqlite3_close(handle);
    return 0;
}

