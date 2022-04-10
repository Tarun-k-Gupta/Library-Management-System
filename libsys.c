#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "libsys.h"

struct LibsysInfo repo_handle;

//--------------------------------create()---------------------------------
int libsys_create(char *repo_name1, char *repo_name2, char *repo_name3)
{

	char bookRepo[strlen(repo_name1) + 5];
	strcpy(bookRepo, repo_name1);
	strcat(bookRepo, ".dat");
	FILE *fp1 = fopen(bookRepo, "wb");

	char studRepo[strlen(repo_name2) + 5];
	strcpy(studRepo, repo_name2);
	strcat(studRepo, ".dat");
	FILE *fp2 = fopen(studRepo, "wb");

	char issueRepo[strlen(repo_name3) + 5];
	strcpy(issueRepo, repo_name3);
	strcat(issueRepo, ".dat");
	FILE *fp3 = fopen(issueRepo, "wb");

	char indRepo[strlen(repo_name1) + 5];
	strcpy(indRepo, repo_name1);
	strcat(indRepo, ".ndx");
	FILE *fp4 = fopen(indRepo, "wb");

	char indRepo_stud[strlen(repo_name2) + 5];
	strcpy(indRepo_stud, repo_name2);
	strcat(indRepo_stud, ".ndx");
	FILE *fp5 = fopen(indRepo_stud, "wb");

	int x = 0;

	if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL || fp5 == NULL)
		return LIB_FILE_ERROR;

	else if (fwrite(&x, sizeof(int), 1, fp4) != 1 || fwrite(&x, sizeof(int), 1, fp5) != 1)
	{
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
		fclose(fp4);
		fclose(fp5);
		return LIB_FILE_ERROR;
	}
	else
	{
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
		fclose(fp4);
		fclose(fp5);
		return LIB_SUCCESS;
	}
}

//--------------------------------open()---------------------------------

int libsys_open(char *book_name, char *stud_name, char *issue_name)
{
	int s1 = booksys_open(book_name);
	int s2 = studsys_open(stud_name);
	int s3 = issuesys_open(issue_name);

	int x;
	char indRepo[strlen(book_name) + 5];
	strcpy(indRepo, book_name);
	strcat(indRepo, ".ndx");
	repo_handle.libsys_ndx_fp = fopen(indRepo, "rb+");
	fread(&x, sizeof(int), 1, repo_handle.libsys_ndx_fp);
	repo_handle.index_count = x;

	fread(repo_handle.index_entries, sizeof(struct LIB_NdxInfo), repo_handle.index_count, repo_handle.libsys_ndx_fp);

	fclose(repo_handle.libsys_ndx_fp);

	char indRepo_stud[strlen(stud_name) + 5];
	strcpy(indRepo_stud, stud_name);
	strcat(indRepo_stud, ".ndx");
	repo_handle.libsys_ndx_fp_stud = fopen(indRepo_stud, "rb+");
	fread(&x, sizeof(int), 1, repo_handle.libsys_ndx_fp_stud);
	repo_handle.index_count_stud = x;

	fread(repo_handle.index_entries_stud, sizeof(struct LIB_NdxInfo), repo_handle.index_count_stud, repo_handle.libsys_ndx_fp_stud);

	fclose(repo_handle.libsys_ndx_fp_stud);

	if (s1 == LIB_SUCCESS && s2 == LIB_SUCCESS && s3 == LIB_SUCCESS)
		return LIB_SUCCESS;
	else
		return LIB_FILE_ERROR;
}
int booksys_open(char *repo_name)
{

	char bookRepo[strlen(repo_name) + 5];
	strcpy(bookRepo, repo_name);
	strcat(bookRepo, ".dat");
	repo_handle.booksys_data_fp = fopen(bookRepo, "rb+");
	if (repo_handle.booksys_data_fp == NULL)
		return LIB_FILE_ERROR;
	strcpy(repo_handle.booksys_name, repo_name);
	repo_handle.book_repo_status = LIB_REPO_OPEN;
	return LIB_SUCCESS;
}

int issuesys_open(char *repo_name)
{

	char issueRepo[strlen(repo_name) + 5];
	strcpy(issueRepo, repo_name);
	strcat(issueRepo, ".dat");
	repo_handle.issuesys_data_fp = fopen(issueRepo, "rb+");
	if (repo_handle.issuesys_data_fp == NULL)
		return LIB_FILE_ERROR;

	strcpy(repo_handle.issuesys_name, repo_name);
	repo_handle.issue_repo_status = LIB_REPO_OPEN;
	return LIB_SUCCESS;
}

int studsys_open(char *repo_name)
{

	char studRepo[strlen(repo_name) + 5];
	strcpy(studRepo, repo_name);
	strcat(studRepo, ".dat");
	repo_handle.studsys_data_fp = fopen(studRepo, "rb+");
	if (repo_handle.studsys_data_fp == NULL)
		return LIB_FILE_ERROR;
	strcpy(repo_handle.studsys_name, repo_name);
	repo_handle.stud_repo_status = LIB_REPO_OPEN;
	return LIB_SUCCESS;
}

int get_book_by_isbn(int key, struct Book *rec)
{
	if (repo_handle.book_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;

	for (int i = 0; i < repo_handle.index_count; i++)
	{
		if (repo_handle.index_entries[i].key == key)
		{
			if (repo_handle.index_entries[i].flag == RECORD_PRESENT)
			{
				fseek(repo_handle.booksys_data_fp, repo_handle.index_entries[i].offset, SEEK_SET);
				fread(&key, sizeof(int), 1, repo_handle.booksys_data_fp);
				fread(rec, sizeof(struct Book), 1, repo_handle.booksys_data_fp);

				printf("Book Details:\n");
				printf("ISBN - %d\n", rec->isbn);
				printf("Title - %s\n", rec->title);
				printf("Author - %s\n", rec->author);
				printf("Price - %f\n", rec->price);

				fseek(repo_handle.issuesys_data_fp, 0, SEEK_SET);
				int key_to_check, roll_number_issued_to, flag = 0;

				struct Issue *issue_check = (struct Issue *)malloc(sizeof(struct Issue));
				struct Student *stud_check = (struct Student *)malloc(sizeof(struct Student));
				while (fread(issue_check, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp))
				{
					if (key == issue_check->isbn)
					{
						flag = 1;
						break;
					}
				}

				if (flag == 1)
				{
					printf("Issue Status - ISSUED\n");
					get_student_by_rollno(issue_check->rollno, stud_check);
					printf("Student Details Holding that book: \n");
					printf("Roll Number - %d\n", stud_check->rollno);
					printf("Name - %s\n", stud_check->name);
					printf("Address - %s\n", stud_check->address);
					printf("CGPA - %f\n", stud_check->cgpa);
				}

				else
					printf("Issue Status - NOT ISSUED\n");

				return LIB_SUCCESS;
			}
			else
			{
				return LIB_REC_NOT_FOUND;
			}
		}
	}
	return LIB_REC_NOT_FOUND;
}
int put_book_by_isbn(int key, struct Book *rec)
{
	if (repo_handle.book_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;

	for (int i = 0; i < repo_handle.index_count; i++)
	{
		if (repo_handle.index_entries[i].key == key)
		{
			if (repo_handle.index_entries[i].flag == RECORD_PRESENT)
			{
				return LIB_ADD_FAILED;
			}
			else
			{
				repo_handle.index_entries[i].flag = RECORD_PRESENT;
				int off_set = repo_handle.index_entries[i].offset + sizeof(int);
				fseek(repo_handle.booksys_data_fp, off_set, SEEK_SET);
				int a = fwrite(rec, sizeof(struct Book), 1, repo_handle.booksys_data_fp);
				if (a != 1)
					return LIB_ADD_FAILED;
				return LIB_SUCCESS;
			}
		}
	}

	fseek(repo_handle.booksys_data_fp, 0, SEEK_END);
	int x = (int)ftell(repo_handle.booksys_data_fp);
	struct LIB_NdxInfo temp;
	temp.offset = x;
	temp.key = key;
	temp.flag = RECORD_PRESENT;

	repo_handle.index_entries[repo_handle.index_count] = temp;
	repo_handle.index_count++;
	fseek(repo_handle.booksys_data_fp, 0, SEEK_END);
	int a = fwrite(&key, sizeof(int), 1, repo_handle.booksys_data_fp);
	int b = fwrite(rec, sizeof(struct Book), 1, repo_handle.booksys_data_fp);
	if (a == 1 && b == 1)
		return LIB_SUCCESS;
	else
		return LIB_ADD_FAILED;
}

int delete_book_by_isbn(int key)
{
	if (repo_handle.book_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;

	//checking if book is issued or not
	fseek(repo_handle.issuesys_data_fp, 0, SEEK_SET);
    int key_to_check;

    struct Issue issue_rec;
    while(fread(&issue_rec, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp))
    {
        if(key == issue_rec.isbn)
        {
			printf("Book can't be deleted as it is issued\n");
			return LIB_REC_NOT_FOUND;
        }
    }

	for (int i = 0; i < repo_handle.index_count; i++)
	{
		if (repo_handle.index_entries[i].key == key)
		{
			if (repo_handle.index_entries[i].flag == RECORD_PRESENT)
			{
				repo_handle.index_entries[i].flag = RECORD_DELETED;
				return LIB_SUCCESS;
			}
			else
			{
				return LIB_REC_NOT_FOUND;
			}
		}
	}
	return LIB_REC_NOT_FOUND;
}

int put_student_by_rollno(int rollno_to_write, struct Student *rec)
{
	if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;
	fseek(repo_handle.studsys_data_fp, 0, SEEK_END);

	for (int i = 0; i < repo_handle.index_count_stud; i++)
	{
		if (repo_handle.index_entries_stud[i].key == rollno_to_write)
		{
			if (repo_handle.index_entries_stud[i].flag == RECORD_PRESENT)
			{
				return LIB_ADD_FAILED;
			}
			else
			{
				repo_handle.index_entries_stud[i].flag = RECORD_PRESENT;
				int off_set = repo_handle.index_entries_stud[i].offset + sizeof(int);
				fseek(repo_handle.studsys_data_fp, off_set, SEEK_SET);
				int a = fwrite(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp);
				if (a != 1)
					return LIB_ADD_FAILED;
				return LIB_SUCCESS;
			}
		}
	}

	fseek(repo_handle.studsys_data_fp, 0, SEEK_END);
	int x = (int)ftell(repo_handle.studsys_data_fp);
	struct LIB_NdxInfo temp;
	temp.offset = x;
	temp.key = rollno_to_write;
	temp.flag = RECORD_PRESENT;

	repo_handle.index_entries_stud[repo_handle.index_count_stud] = temp;
	repo_handle.index_count_stud++;
	fseek(repo_handle.studsys_data_fp, 0, SEEK_END);
	int a = fwrite(&rollno_to_write, sizeof(int), 1, repo_handle.studsys_data_fp);
	int b = fwrite(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp);
	if (a != 1 || b != 1)
		return LIB_ADD_FAILED;
	return LIB_SUCCESS;
}

int get_student_by_rollno(int rollno_to_read, struct Student *rec)
{
	if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;

	for (int i = 0; i < repo_handle.index_count_stud; i++)
	{
		if (repo_handle.index_entries_stud[i].key == rollno_to_read)
		{
			if (repo_handle.index_entries_stud[i].flag == RECORD_PRESENT)
			{
				fseek(repo_handle.studsys_data_fp, repo_handle.index_entries_stud[i].offset, SEEK_SET);
				fread(&rollno_to_read, sizeof(int), 1, repo_handle.studsys_data_fp);
				fread(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp);
				return LIB_SUCCESS;
			}
			else
			{
				return LIB_REC_NOT_FOUND;
			}
		}
	}
	return LIB_REC_NOT_FOUND;
}

int delete_student_by_rollno(int rollno)
{
	if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;

	for (int i = 0; i < repo_handle.index_count_stud; i++)
	{
		if (repo_handle.index_entries_stud[i].key == rollno)
		{
			if (repo_handle.index_entries_stud[i].flag == RECORD_PRESENT)
			{
				repo_handle.index_entries_stud[i].flag = RECORD_DELETED;
				return LIB_SUCCESS;
			}
			else
			{
				return LIB_REC_NOT_FOUND;
			}
		}
	}
	return LIB_REC_NOT_FOUND;
}

//--------------------------------issue()---------------------------------
int issue(int rollno, int isbn)
{
	if (repo_handle.book_repo_status != LIB_REPO_OPEN || repo_handle.stud_repo_status != LIB_REPO_OPEN || repo_handle.issue_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;

	struct Student s;
	struct Book b;

	int s1 = get_book_by_isbn(isbn, &b);
	int s2 = get_student_by_rollno(rollno, &s);
	if (s1 != LIB_SUCCESS || s2 != LIB_SUCCESS)
		return LIB_REC_NOT_FOUND;

	struct Issue i;
	i.isbn = isbn;
	i.rollno = rollno;
	fseek(repo_handle.issuesys_data_fp, 0, SEEK_END);
	int a = fwrite(&i, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
	if (a == 1)
		return LIB_SUCCESS;
	return LIB_ADD_FAILED;
}

int libsys_close()
{
	int s1 = booksys_close();
	int s2 = studsys_close();
	int s3 = issuesys_close();

	char indexFile[strlen(repo_handle.booksys_name) + 5];
	strcpy(indexFile, repo_handle.booksys_name);
	strcat(indexFile, ".ndx");
	repo_handle.libsys_ndx_fp = fopen(indexFile, "wb");
	fseek(repo_handle.libsys_ndx_fp, 0, SEEK_SET);
	if (fwrite(&repo_handle.index_count, sizeof(int), 1, repo_handle.libsys_ndx_fp) != 1)
	{
		return LIB_NDX_SAVE_FAILED;
	}
	int a = fwrite(repo_handle.index_entries, sizeof(struct LIB_NdxInfo), repo_handle.index_count, repo_handle.libsys_ndx_fp);
	if (a != repo_handle.index_count)
		return LIB_NDX_SAVE_FAILED;
	fclose(repo_handle.libsys_ndx_fp);

	char indRepo_stud[strlen(repo_handle.studsys_name) + 5];
	strcpy(indRepo_stud, repo_handle.studsys_name);
	strcat(indRepo_stud, ".ndx");
	repo_handle.libsys_ndx_fp_stud = fopen(indRepo_stud, "wb");
	fseek(repo_handle.libsys_ndx_fp_stud, 0, SEEK_SET);
	if (fwrite(&repo_handle.index_count_stud, sizeof(int), 1, repo_handle.libsys_ndx_fp_stud) != 1)
	{
		return LIB_NDX_SAVE_FAILED;
	}
	int b = fwrite(repo_handle.index_entries_stud, sizeof(struct LIB_NdxInfo), repo_handle.index_count_stud, repo_handle.libsys_ndx_fp_stud);
	if (b != repo_handle.index_count_stud)
		return LIB_NDX_SAVE_FAILED;
	fclose(repo_handle.libsys_ndx_fp_stud);

	if (s1 != LIB_SUCCESS || s2 != LIB_SUCCESS || s3 != LIB_SUCCESS)
	{
		return s1;
	}
	return LIB_SUCCESS;
}
int booksys_close()
{
	if (repo_handle.book_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;
	fclose(repo_handle.booksys_data_fp);
	strcpy(repo_handle.booksys_name, "");
	repo_handle.book_repo_status = LIB_REPO_CLOSED;
	return LIB_SUCCESS;
}
int studsys_close()
{
	if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;
	fclose(repo_handle.studsys_data_fp);
	strcpy(repo_handle.studsys_name, "");
	repo_handle.stud_repo_status = LIB_REPO_CLOSED;
	return LIB_SUCCESS;
}
int issuesys_close()
{
	if (repo_handle.issue_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;
	fclose(repo_handle.issuesys_data_fp);
	strcpy(repo_handle.issuesys_name, "");
	repo_handle.issue_repo_status = LIB_REPO_CLOSED;
	return LIB_SUCCESS;
}

int get_book_by_title(char *title, struct Book *rec)
{
	if (repo_handle.book_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;

	int key;
	struct Book recBook;
	for (int i = 0; i < repo_handle.index_count; i++)
	{
		if (repo_handle.index_entries[i].flag == RECORD_PRESENT)
		{
			fseek(repo_handle.booksys_data_fp, repo_handle.index_entries[i].offset, SEEK_SET);
			fread(&key, sizeof(int), 1, repo_handle.booksys_data_fp);
			fread(&recBook, sizeof(struct Book), 1, repo_handle.booksys_data_fp);

			if (!strcmp(recBook.title, title))
			{
				rec->price = recBook.price;
				strcpy(rec->title, recBook.title);
				strcpy(rec->author, recBook.author);
				rec->isbn = recBook.isbn;

				printf("Book Details:\n");
				printf("ISBN - %d\n", recBook.isbn);
				printf("Title - %s\n", recBook.title);
				printf("Author - %s\n", recBook.author);
				printf("Price - %f\n", recBook.price);

				fseek(repo_handle.issuesys_data_fp, 0, SEEK_SET);
				int key_to_check, roll_number_issued_to, signal = 0;

				struct Issue *issue_check = (struct Issue *)malloc(sizeof(struct Issue));
				struct Student *stud_check = (struct Student *)malloc(sizeof(struct Student));
				while (fread(issue_check, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp))
				{
					if (key == issue_check->isbn)
					{
						signal = 1;
						break;
					}
				}

				if (signal == 1)
				{
					printf("Issue Status - ISSUED\n");
					get_student_by_rollno(issue_check->rollno, stud_check);
					printf("Student Details Holding that book: \n");
					printf("Roll Number - %d\n", stud_check->rollno);
					printf("Name - %s\n", stud_check->name);
					printf("Address - %s\n", stud_check->address);
					printf("CGPA - %f\n", stud_check->cgpa);
				}

				else
					printf("Issue Status - Not Issued\n");

				return BOOK_SUCCESS;
			}
		}
	}

	return LIB_REC_NOT_FOUND;
}

int get_student_by_name(char *name, struct Student *rec)
{
	if (repo_handle.book_repo_status != LIB_REPO_OPEN)
		return LIB_REPO_CLOSED;

	int key;
	struct Student rec_stud;
	for (int i = 0; i < repo_handle.index_count_stud; i++)
	{
		if (repo_handle.index_entries_stud[i].flag == RECORD_PRESENT)
		{
			fseek(repo_handle.studsys_data_fp, repo_handle.index_entries_stud[i].offset, SEEK_SET);
			fread(&key, sizeof(int), 1, repo_handle.studsys_data_fp);
			fread(&rec_stud, sizeof(struct Student), 1, repo_handle.studsys_data_fp);

			if (!strcmp(rec_stud.name, name))
			{
				strcpy(rec->address, rec_stud.address);
				rec->cgpa = rec_stud.cgpa;
				strcpy(rec->name, rec_stud.name);
				rec->rollno = rec_stud.rollno;
				return LIB_SUCCESS;
			}
		}
	}

	return LIB_REC_NOT_FOUND;
}