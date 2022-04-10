#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "IMT2020061_libsys.c"

int main()
{
    printf("\nEnter any of the following choice:\n");
    printf("\n1 - CREATE\n2 - OPEN\n3 - ADD_BOOK\n4 - ADD_STUDENT\n5 - DELETE_BOOK\n6 - DELETE_STUDENT\n7 - ISSUE_BOOK \n8 - SEARCH_BOOK_BY_ISBN\n9 - SEARCH_BOOK_BY_TITLE\n10 - SEARCH_STUDENT_BY_ROLL_NO\n11 - SEARCH_STUDENT_BY_NAME\n12 - Exit\n\n");
    while (1)
    {
        int choice;
        scanf("%d", &choice);

        int status;
        struct Student Student_Rec;
        struct Book Book_Rec;
        int isbn_to_search;
        char title[30];
        char name[30];
        int roll_no_to_search;
        int roll_no_to_delete;
        int isbn_to_delete;
        int roll_no, isbn;

        char *stud_repo_name = "stud_demo";
        char *book_repo_name = "book_demo";
        char *issue_repo_name = "issue";

        if (choice == 1)
        {
            status = libsys_create(book_repo_name, stud_repo_name, issue_repo_name);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 2)
        {
            status = libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 3)
        {
            printf("Enter ISBN number: ");
            scanf("%d", &Book_Rec.isbn);

            printf("Enter Book Title: ");
            scanf("%s", Book_Rec.title);

            printf("Enter Author: ");
            scanf("%s", Book_Rec.author);

            printf("Enter Price: ");
            scanf("%f", &Book_Rec.price);

            status = put_book_by_isbn(Book_Rec.isbn, &Book_Rec);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 4)
        {
            printf("Enter Roll_No: ");
            scanf("%d", &Student_Rec.rollno);

            printf("Enter name: ");
            scanf("%s", Student_Rec.name);

            printf("Enter address: ");
            scanf("%s", Student_Rec.address);

            printf("Enter cgpa: ");
            scanf("%f", &Student_Rec.cgpa);

            status = put_student_by_rollno(Student_Rec.rollno, &Student_Rec);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 5)
        {
            printf("Enter isbn number to be deleted: ");
            scanf("%d", &isbn_to_delete);

            status = delete_book_by_isbn(isbn_to_delete);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 6)
        {
            printf("Enter Roll number to be deleted: ");
            scanf("%d", &roll_no_to_delete);

            status = delete_student_by_rollno(roll_no_to_delete);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 7)
        {
            printf("Enter ISBN number: ");
            scanf("%d", &isbn);

            printf("Enter roll_number: ");
            scanf("%d", &roll_no);

            status = issue(roll_no, isbn);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 8)
        {

            printf("Enter ISBN number to be searched: ");
            scanf("%d", &isbn_to_search);

            status = get_book_by_isbn(isbn_to_search, &Book_Rec);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 9)
        {
            printf("Enter title to be searched: ");
            scanf("%s", title);

            status = get_book_by_title(title, &Book_Rec);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 10)
        {
            printf("Enter Roll number to be searched: ");
            scanf("%d", &roll_no_to_search);

            status = get_student_by_rollno(roll_no_to_search, &Student_Rec);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 11)
        {
            printf("Enter Name of the student: ");
            scanf("%s", name);

            status = get_student_by_name(name, &Student_Rec);
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
        }
        else if (choice == 12)
        {
            status = libsys_close();
            if (status == LIB_SUCCESS)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAILED\n");
            }
            exit(1);
        }
        else
        {
            break;
        }

        if(choice != 12)    printf("\nEnter next choice:\n");
    }
}