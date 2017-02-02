#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

int count=1, bcount=1;

typedef struct ReservedList{
	int std_num;
	struct ReservedList *next;
}ReservedList;

typedef struct Book{
	char bookname[100];
	int bookid;
	ReservedList *head, *tail;
	int current;
	struct Book *next;
}Book;

typedef struct StudentBookLog{
	int bookid;
	struct StudentBookLog *next;
}StudentBookLog;

typedef struct StudentList{
	char id[100];
	char name[100];
	int std_num;
	char password[100];
	StudentBookLog *head, *tail; 
	struct StudentList *next;
}StudentList;


StudentList *Login_Check(char *id, char *password);
StudentList *Search_id(char *id);
StudentList *Search_last();
StudentList *Search_std_num(int std_num);
void Borrow_Book(char *bookname);
void Reserve_Book(char *bookname);
Book *Access_book(char *bookname);
Book *Search_bookid(int bookid);
void Create_Account(char *id, char *password, char *name);
void Create_Account_s(char *id, char *password, char *name);
void Print_Borrowed_Book(StudentList *user);
int Reserve_Check(char *bookname);
char *Encrypt(char *password);		//�н����� ��ȣȭ
int Validify(char *input, char *password);		//��ȣȭ�� �н����� Ȯ��
void Password_Input(char *password);//�н����� ���� �Է� ���
int  SearchStr(char *str1, char *str2);//���ڿ� ã�� �Լ�
void Book_Search_Solution(char *keyword);
int Is_Reserved_By_User(char *bookname);
void MakeAdmin(void);
void Addbook(char *bookname);
void Addbook_s(char *bookname);
void Deletebook(char *bookname);
void ChangeLineCh(char *input);
void Game(int g1, int g2, int *counter);
void Gameexecute(void);
void Intro(void);
void Intro_Color(char *sys);
void Search_Account(char *search);

//Final
void Print_Reserved_Book(StudentList *user);
void Reserve_Book_Cancel(char *bookname);
void Return_Book(char *bookname);

StudentList *SLhead, *SLtail, *currentuser; //CurrentUser �������� �̵�. ���� �� ��Ÿ��� ������ ���ϰ�
Book *Bhead, *Btail;

int main()
{
	char select;
	char id[100], password[100], name[100], book_search[100];				//ȸ������ scanf�� ����
	char ch;
	char bookdb[100],  userdb_id[100], userdb_password[100], userdb_name[100];
	Book *searchedbook;
	ReservedList *trash;
	FILE *fp1, *fp2;
	SLhead=(StudentList*)malloc(sizeof(StudentList));	//�л� ����Ʈ ���-���� �޸� �Ҵ�
	SLtail=(StudentList*)malloc(sizeof(StudentList));
	Bhead=(Book*)malloc(sizeof(Book));					//å ����Ʈ ���-���� �޸� �Ҵ�
	Btail=(Book*)malloc(sizeof(Book));
	SLhead->next=SLtail;
	SLtail->next=NULL;
	Bhead->next=Btail;
	Btail->next=NULL;									//���-���� ����


	MakeAdmin(); //admin ���� ����(���̵�: admin, ��й�ȣ: password)

	
	fp1 = fopen("BookDB.txt", "r");
	while(feof(fp1)==0){
		fgets(bookdb, 100*sizeof(char), fp1);
		Addbook_s(bookdb);
	}
	fclose(fp1);
	fp2 = fopen("UserDB.txt", "r");
	while(feof(fp2)==0){
		fgets(userdb_id, 100*sizeof(char), fp2);
		fgets(userdb_password, 100*sizeof(char), fp2);
		fgets(userdb_name, 100*sizeof(char), fp2);
		ChangeLineCh(userdb_id);
		ChangeLineCh(userdb_password);
		ChangeLineCh(userdb_name);
		Create_Account_s(userdb_id, userdb_password, userdb_name);
	}
	fclose(fp2);
	system("cls");

	srand(time(0));
	Intro();
	while(1)
	{
		int validity;
	logout:
		printf("\n");
		printf("---------------------------------\n");
		printf("|        ������ ���α׷�        |\n");
		printf("---------------------------------\n");
		printf("| 1: �α��� 2: ȸ������ 3: ���� |\n");
		printf("---------------------------------\n\n");
		printf("�������ּ���");
		printf("\n--------------------------------------------\n");
		validity=0;
		select=getch();
		switch (select)
		{
		case '1':
			printf("Input ID: ");
			scanf("%s", id);
			printf("Input password: ");
			Password_Input(password);
			currentuser=Login_Check(id, password);
			if(currentuser!=NULL){
				validity=1;
				break;}
			else
				break;
		case '2':
			printf("<ȸ������>\n");
			printf("Input new ID: ");
			scanf("%s", id);
			printf("Input new password: ");
			Password_Input(password);
			Encrypt(password);
			printf("Input your name: ");
			scanf("%s", name);
			Create_Account(id, password, name);
			break;
		case '3':
			Gameexecute();
			printf("�����մϴ�! ���� ���� �� ������ ���������̽��ϴ�!\n\n");
			return 0; break; //quit
		default:
			system("cls");
			printf("* �߸� �����ϼ̽��ϴ�. �ٽ� �������ּ���\n\n");
			break;
		} 
		if(validity==1)
			break;
		fflush(stdin);
	}

	if(strcmp(id, "admin")==0)
	{
		while(1){
			char input[100];
			printf("----------------- M E N U ------------------\n");
			printf("|                                          |\n");
			printf("|         1 : ȸ�� �˻�                    |\n"); //���⼭ ������, ������ ��� ����
			printf("|         2 : ���� �߰�                    |\n");
			printf("|         3 : ���� ����                    |\n");
			printf("|         4 : �α׾ƿ�                     |\n");
			printf("|         5 : ���α׷� ����                |\n");
			printf("|                                          |\n");
			printf("--------------------------------------------\n\n");
			printf("���� : ");
			select=getch();
			switch(select)
			{
			case '1': printf("�˻��� ȸ���� �̸��� �Ϻ� Ȥ�� ��ü�� �Է��ϼ���: ");
				gets(input);
				Search_Account(input);
				break;
			case '2': printf("�߰��� ������ ������ �Է��ϼ���: ");
				gets(input);
				if(Access_book(input)!=NULL){
					printf("�̹� �ش� ������ �����մϴ�.\n\n�ƹ� Ű�� �����ø� ���� �޴��� ���ư��ϴ�.\n");
					getch();
					system("cls");
					break;
				}
				Addbook(input);
				printf("���� ��Ͽ� �����Ͽ����ϴ�. �ƹ� Ű�� �����ø� ���� �޴��� ���ư��ϴ�. ");
				getch();
				system("cls");
				break;
			case '3':printf("������ ������ ������ �Է��ϼ���: ");
				gets(input);
				if(Access_book(input)==NULL){
					printf("������ ������ �����ϴ�.\n\n�ƹ� Ű�� �����ø� ���� �޴��� ���ư��ϴ�.\n");
					getch();
					system("cls");
					break;
				}
				Deletebook(input);
				printf("���� ������ �����Ͽ����ϴ�. �ƹ� Ű�� �����ø� ���� �޴��� ���ư��ϴ�. ");
				getch();
				system("cls");
				break;
			case '4':
				system("cls");
				goto logout;
			case '5':
				Gameexecute();
				printf("�����մϴ�! ���� ���� �� ������ ���������̽��ϴ�!\n\n");
				return 0;
			}
		}
	}
	else
	{
		while(1){

			printf("----------------- M E N U ------------------\n");
			printf("|                                          |\n");
			printf("|         1 : ���� ����                    |\n");
			printf("|         2 : ���� �˻�                    |\n");
			printf("|         3 : ���� �ݳ�                    |\n");
			printf("|         4 : ���� ����                    |\n");
			printf("|         5 : ���� ���� ���               |\n");
			printf("|         6 : ���� ���� ���               |\n");
			printf("|         7 : ���� ���� ���               |\n");
			printf("|         8 : �α׾ƿ�                     |\n");
			printf("|         0 : ���α׷� ����                |\n");
			printf("|                                          |\n");
			printf("--------------------------------------------\n\n");
			printf("���� : ");
			select=getch();
			switch (select)
			{
			case '1' :
				system("cls");
				printf("\n--------------------------------------------\n");
				printf("|  [���� ����]                              |\n");
				printf("|  ������ ���ϴ� ������ �������� �Է��ϼ��� |\n");
				printf("--------------------------------------------\n\n");
				printf("������ : ");
				scanf("%[^\n]", book_search);
				searchedbook=Access_book(book_search);
				if(searchedbook == NULL)
				{
					printf("* ã���ô� ������ �����ϴ�. �ƹ� Ű�� �����ø� ���� �޴��� �̵��մϴ�.\n");
					getch();
					system("cls");
				}
				else
				{
					if(Reserve_Check(book_search) == 0 && searchedbook->current == 0 ){
						printf("\n���� �����մϴ�. �����Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
						ch='a';
						while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
							ch=getch();
						}
						if(ch=='N'||ch=='n'){
							system("cls");
							break;
						}
						else{
							Borrow_Book(book_search);
						}
					}

					else{
						if(searchedbook->current==currentuser->std_num){
							printf("\nȸ������ �������� �����Դϴ�. �ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�. ");
							getch();
							system("cls");
							break;
						}
						else if(searchedbook->current==0 && searchedbook->head->next->std_num==currentuser->std_num){
							printf("ȸ������ �����ߴ� ������, ���� 0�����Դϴ�.\n");
							printf("�ƹ� Ű�� ������ ���� �� ���� �޴��� ���ư��ϴ�.");
							getch();
							Borrow_Book(book_search);
							trash=searchedbook->head->next;
							searchedbook->head->next=searchedbook->head->next->next;
							free(trash);
							break;
						}
						else if(Is_Reserved_By_User(book_search)>0){
							printf("\n�̹� ���� ��ܿ� �ö� �ֽ��ϴ�. ���� %d�����Դϴ�.\n", Is_Reserved_By_User(book_search));
							printf("�ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�. ");
							getch();
							system("cls");
							break;
						}
						else if(searchedbook->current != 0){
							printf("\n�̹� ����� �����Դϴ�. �����ڴ� %d���Դϴ�. ", Reserve_Check(book_search));
						}
						
						else
						{
							printf("\n�������� �ƴϳ�, �����ڰ� %d�� �����մϴ�. ", Reserve_Check(book_search));
						}
						printf("�����Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
						ch='a';
						while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
							ch=getch();
						}
						if(ch=='N'||ch=='n')
							break;
						else{
							Reserve_Book(book_search);
						}
					}
				}
				break;
			case '2' :
				system("cls");
				printf("\n--------------------------------------------\n");
				printf("|  [���� �˻�]                              |\n");
				printf("|  ������ ������ �Ϻθ� �Է��ϼ���          |\n");
				printf("--------------------------------------------\n\n");
				printf("������ : ");
				scanf("%[^\n]", book_search);
				Book_Search_Solution(book_search);
				break;
			case '3' :
				system("cls");
				Print_Borrowed_Book(currentuser);								//�ݳ��� �����ϰ� �ϱ� ���� ���� ���� ����� �����ݴϴ�
				printf("\n--------------------------------------------\n");
				printf("|  [���� �ݳ�]                              |\n");
				printf("|  �ݳ��� ���ϴ� ������ �������� �Է��ϼ��� |\n");
				printf("--------------------------------------------\n\n");
				printf("������ : ");
				scanf("%[^\n]", book_search);
				searchedbook=Access_book(book_search);
				if(searchedbook == NULL)
				{
					printf("* ã���ô� ������ �����ϴ�. �ƹ� Ű�� �����ø� ���� �޴��� �̵��մϴ�.\n");
					getch();
					system("cls");
				}
				else
				{
					if(Reserve_Check(book_search) == 0 && searchedbook->current == 0 ){
						printf("ȸ������ �������� ���� �����Դϴ�\n");
						printf("\n���� �����մϴ�. �����Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
						ch='a';
						while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
							ch=getch();
						}
						if(ch=='N'||ch=='n'){
							system("cls");
							break;
						}
						else{
							Borrow_Book(book_search);
						}
					}

					else{
						if(searchedbook->current==currentuser->std_num){
							printf("\nȸ������ �������� �����Դϴ�. �ݳ��Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
							ch='a';
							while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
								ch=getch();
							}
							if(ch=='N'||ch=='n'){
								system("cls");
								break;
							}
							else{
								Return_Book(book_search);
							}
						}
						else if(searchedbook->current==0 && searchedbook->head->next->std_num==currentuser->std_num){
							printf("ȸ������ �����ߴ� ������, ���� 0�����Դϴ�.\n");
							printf("������ ����Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
							ch='a';
							while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
								ch=getch();
							}
							if(ch=='N'||ch=='n'){
								system("cls");
								break;
							}
							else{
								Reserve_Book_Cancel(book_search);
							}
						}
						else if(Is_Reserved_By_User(book_search)>0){
							printf("�����Ͻ� å�� �ƴ����� ���� ��ܿ� �ö� �ֽ��ϴ�. ���� %d�����Դϴ�.\n", Is_Reserved_By_User(book_search));
							printf("������ ����Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
							ch='a';
							while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
								ch=getch();
							}
							if(ch=='N'||ch=='n'){
								system("cls");
								break;
							}
							else{
								Reserve_Book_Cancel(book_search);
							}
						}
						else if(searchedbook->current != 0){
							printf("ȸ������ ����/������ ������ �ƴմϴ�. �ƹ� Ű�� �����ø� ���� �޴��� �̵��մϴ�.\n");
							getch();
							system("cls");
						}
						else
						{
							printf("ȸ������ ����/������ ������ �ƴմϴ�. �ƹ� Ű�� �����ø� ���� �޴��� �̵��մϴ�.\n");
							getch();
							system("cls");
						}
					}
				}
				break;
			case '4' :
				system("cls");
				printf("\n--------------------------------------------\n");
				printf("|  [���� ����]                              |\n");
				printf("|  ������ ���ϴ� ������ �������� �Է��ϼ��� |\n");
				printf("--------------------------------------------\n\n");
				printf("������ : ");
				scanf("%[^\n]", book_search);
				searchedbook=Access_book(book_search);
				if(searchedbook == NULL){
					printf("* ã���ô� ������ �����ϴ�. �ƹ� Ű�� �����ø� ���� �޴��� �̵��մϴ�.\n");
					getch();
					system("cls");
				}
				else if(searchedbook->current==currentuser->std_num){
					printf("\nȸ������ �������� �����Դϴ�. �ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�. ");
					getch();
					system("cls");
					break;
				}
				else if(Is_Reserved_By_User(book_search)>0){
					printf("\n�̹� ���� ��ܿ� �ö� �ֽ��ϴ�. ���� %d�����Դϴ�.\n", Is_Reserved_By_User(book_search));
					printf("�ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�. ");
					getch();
					system("cls");
					break;
				}
				else
				{
					Reserve_Book(book_search);
				}
				break;
			case '5':
				system("cls");
				Print_Reserved_Book(currentuser);
				printf("\n  --------------------------------------------\n");
				printf("|  [���� ���� ���]                              |\n");
				printf("|  ���� ��Ҹ� ���ϴ� ������ �������� �Է��ϼ��� |\n");
				printf("  --------------------------------------------\n\n");
				printf("������ : ");
				scanf("%[^\n]", book_search);
				searchedbook=Access_book(book_search);
				if(searchedbook == NULL){
					printf("* ã���ô� ������ �����ϴ�. �ƹ� Ű�� �����ø� ���� �޴��� �̵��մϴ�.\n");
					getch();
					system("cls");
				}
				else if(searchedbook->current==currentuser->std_num){
					printf("\nȸ������ �̹� �������� �����Դϴ�. �ݳ��Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
					ch='a';
					while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
						ch=getch();
					}
					if(ch=='N'||ch=='n'){
						system("cls");
						break;
					}
					else{
						Return_Book(book_search);
					}
					break;
				}
				else if(Is_Reserved_By_User(book_search)>0){
					printf("\n���� ��ܿ� �ö� �ֽ��ϴ�. ���� %d�����Դϴ�.\n", Is_Reserved_By_User(book_search));
					printf("������ ����Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
					ch='a';
					while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
						ch=getch();
					}
					if(ch=='N'||ch=='n'){
						system("cls");
						break;
					}
					else{
						Reserve_Book_Cancel(book_search);
					}
					break;
				}
				else
				{
					printf("\n�������� ���� �����Դϴ�.\n");
					printf("�ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�. ");
					getch();
					system("cls");
					break;
				}
				break;
			case '6':
				Print_Borrowed_Book(currentuser);
				printf("----------------------\n");
				printf("���� �޴��� ���ư����� �ƹ� Ű�� �����ʽÿ�. ");
				getch();
				system("cls");
				break;
			case '7':
				Print_Reserved_Book(currentuser);
				printf("���� �޴��� ���ư����� �ƹ� Ű�� �����ʽÿ�. ");
				getch();
				system("cls");
				break;
			case '8':
				system("cls");
				printf("���������� �α׾ƿ� �Ǿ����ϴ�!\n\n");
				goto logout;
			case '0':
				Gameexecute();
				printf("�����մϴ�! ���� ���� �� ������ ���������̽��ϴ�!\n\n");
				return 0;
			default:
				system("cls");
				printf("\n* �߸� �����ϼ̽��ϴ�. �ٽ� �������ּ���.\n\n");
				break;
			}
			fflush(stdin);
		}
	} 
	system("pause");
	return 0;   
}

StudentList *Login_Check(char *id, char *password)
{
	if(Search_id(id)==NULL || Validify(password, Search_id(id)->password)==0){
		system("cls");
		printf("* ���̵� �Ǵ� ��й�ȣ�� �ٽ� Ȯ���ϼ���\n");
		printf("��ϵ��� ���� ���̵��̰ų�, ���̵� �Ǵ� ��й�ȣ�� �߸� �Է��ϼ̽��ϴ�.\n\n");
		return NULL;
	}
	system("cls");
	printf("\n�α��� ����!\n");
	printf("[%s]�� ȯ���մϴ�.\n\n", Search_id(id)->name);
	return Search_id(id);
}

StudentList *Search_id(char *id)
{
	StudentList *p;
	if(SLhead->next==SLtail)
		return NULL;
	for(p=SLhead->next; p!=SLtail; p=p->next)
	{
		if(strcmp(p->id, id)==0)
			return p;
	}
	return NULL;
}

StudentList *Search_last()
{
	StudentList *p=SLhead;
	while(p->next!=SLtail)
	{
		p=p->next;
	}
	return p;
}

StudentList *Search_std_num(int std_num)
{
	StudentList *p;
	if(SLhead->next==SLtail)
		return NULL;
	for(p=SLhead->next; p!=SLtail; p=p->next)
	{
		if(p->std_num == std_num)
			return p;
	}
	return NULL;
}

void Create_Account(char *id, char *password, char *name)
{
	StudentList *create=(StudentList*)malloc(sizeof(StudentList));
	FILE *fp2;
	if(Search_id(id)!=NULL){
		system("cls");
		printf("* �̹� �����ϴ� ���̵��Դϴ�.\n");
		free(create);
		return;
	}
	Search_last()->next=create;
	create->next=SLtail;

	strcpy(create->id, id);
	strcpy(create->password, password);
	strcpy(create->name, name);

	create->std_num=count++;
	create->head=(StudentBookLog*)malloc(sizeof(StudentBookLog));
	create->tail=(StudentBookLog*)malloc(sizeof(StudentBookLog));
	create->head->next=create->tail;
	create->tail->next=NULL;

	
	fp2=fopen("UserDB.txt", "a+");
	fprintf(fp2, "%c", 10);
	fputs(id, fp2);
	fprintf(fp2, "%c", 10);
	fputs(password, fp2);
	fprintf(fp2, "%c", 10);
	fputs(name, fp2);
	fclose(fp2);

	system("cls");
	printf("--------------------------------------------\n");
	printf("* ȸ�������� �Ϸ�Ǿ����ϴ�.\n\n");
}

void Create_Account_s(char *id, char *password, char *name)
{
	StudentList *create=(StudentList*)malloc(sizeof(StudentList));
	if(Search_id(id)!=NULL){
		system("cls");
		printf("* �̹� �����ϴ� ���̵��Դϴ�.\n");
		free(create);
		return;
	}
	Search_last()->next=create;
	create->next=SLtail;

	strcpy(create->id, id);
	strcpy(create->password, password);
	strcpy(create->name, name);

	create->std_num=count++;
	create->head=(StudentBookLog*)malloc(sizeof(StudentBookLog));
	create->tail=(StudentBookLog*)malloc(sizeof(StudentBookLog));
	create->head->next=create->tail;
	create->tail->next=NULL;

	system("cls");
	printf("--------------------------------------------\n");
	printf("* ȸ�������� �Ϸ�Ǿ����ϴ�.\n\n");
}

Book *Access_book(char *bookname)
{
	Book *p;
	if(Bhead->next==Btail)
		return NULL;
	for(p=Bhead->next; p!=Btail; p=p->next)
	{
		if(strcmp(p->bookname, bookname)==0)
			return p;
	}
	return NULL;
}
Book *Search_bookid(int bookid)
{
	Book *p;
	if(Bhead->next==Btail)
		return NULL;
	for(p=Bhead->next; p!=Btail; p=p->next)
	{
		if(p->bookid==bookid)
			return p;
	}
	return NULL;
}

void Borrow_Book(char *bookname)
{
	StudentBookLog *p, *node;
	Book* b = Access_book(bookname);
	char ch;

	b->current=currentuser->std_num;		//å�� ������ ������ ���� ��ȣ ����(�������μ���)
	p=currentuser->head;					//���� �κ��� ���� ������ ������ ���� å ����
	while(p->next != currentuser->tail){
		p=p->next;
	}
	node=(StudentBookLog*)malloc(sizeof(StudentBookLog));
	p->next = node;
	node->next = currentuser->tail;
	node->bookid = b->bookid;
	system("cls");
	printf("* '%s' ���� �Ϸ�Ǿ����ϴ�.\n\n", bookname);

}

void Reserve_Book(char *bookname)
{
	ReservedList *p2, *node2;
	Book* b = Access_book(bookname);
	node2=(ReservedList*)malloc(sizeof(ReservedList));

	if(b==NULL)
		return;

	//���� �κ��� å db�� ���� ���� ����
	p2= b->head;
	while(p2->next != b->tail)
	{
		p2=p2->next;
	}
	p2->next = node2;
	node2->next = b->tail;
	node2->std_num = currentuser->std_num;
	system("cls");
	printf("* '%s' ���� �Ϸ�Ǿ����ϴ�.\n\n", bookname);
	//�̹� �����ڰ� ������ �������� �߰�
	//currentuser�� �ִ��� Ȯ���ϰ� �̹� ���� �Ǵ� ������ �����Դϴ�.
	//�������� ��� å ����Ʈ �ϴܿ��� �߰��ǰ� �л� ����Ʈ �ϴܿ��� �߰����� �ʰ� -> ���� ���� ��� �ذ��. �̰� ���� ����Ʈ�� �߰��ϱ⸸ �ϴ� �Լ���.
}

void Print_Borrowed_Book(StudentList *user)
{
	int i=1;
	StudentBookLog *p;
	if(user->head->next==user->tail)
	{
		printf("\n* ������ ������ �����ϴ�. ���� �޴��� ���ư����� �ƹ� Ű�� �����ʽÿ�. ");
		getch();
		system("cls");
		return;
	}
	p=user->head->next;
	printf("������ ���� ���\n");
	printf("----------------------\n");
	while(p!=user->tail)
	{
		printf("| %d | %s\n", i, Search_bookid(p->bookid)->bookname);
		p=p->next;
		i++;
	}
}

/*
1. ������ �ִ��� Ȯ���ϴ� �Լ�
2. ������ ����ϴ� �Լ�
3. ������ �� ������ ����ϴ� �Լ�
4. ���� ����� ���࿡ �߰��ϴ� �Լ�
5. ���� ����ϴ� �Լ�
*/

int Reserve_Check(char *bookname)
{

	int i=0;
	ReservedList *p = Access_book(bookname)->head;

	while(p->next != Access_book(bookname)->tail)
	{
		i++;
		p=p->next;
	}
	return i;
}

/*
void Print_Borrow_Student(char *bookname)
{
ReservedList *p = Access_book(bookname)->head;

while(p->next != Access_book(bookname)->tail) �ۼ����̴� �Լ�
{
p->std_num
*/

char *Encrypt(char *password)
{
	int i;
	char key=15;
	for(i=0; i<strlen(password); ++i)
	{
		password[i]=password[i]^key;
	}
	return password;
}

int Validify(char *input, char *password)	//input: Ȯ���� ��ȣ password: ���� ��ȣ(��ȣȭ�� ��)
{
	if(strcmp(Encrypt(input), password)==0)
		return 1;
	else
		return 0;
}

void Password_Input(char *password)
{
	int i=0;
	char temp;
	fflush(stdin);
	for(i=0; i<100 ; ++i)
	{
		temp=getch();
		if(temp==13)
			break;
		printf("*");
		password[i]=temp;
	}
	password[i]='\0';
	printf("\n");
}

int  SearchStr(char *str1, char *str2)
{
	int cnt, i, j, n;
	char *s1;

	cnt = 0;     // variable that searches for matching string count
	n = strlen(str1);
	if ( n==0 || strlen(str2) < n ) return 0;   // if str2 is shorter than str1, return 0

	for ( i = 0; str2[i+n-1]; i++){ //loop stops when str2[1+n-1] is 0, which means the sentence ended. The reason for '1+n-1' is to optimize the algorithm. 

		for ( j = 0; j < n; j++){ //loop until str1 ends
			if ( str1[j]!=str2[i+j] ){
				break; //break if character doesn't match
			}
		}

		if ( j==n ){
			cnt++; //if the loop didn't 'break', the string segment matches. Increase count.
		}
	}
	return cnt;
}

void Book_Search_Solution(char *keyword)
{
	Book *p=Bhead->next;
	Book *list[10];
	int i, j;
	char ch;
goback:
	i=0;
	j=0;
	if(Bhead->next==Btail)
		return;
	for(; p!=Btail; p=p->next)
	{
		if(SearchStr(keyword, p->bookname)>0){
			if(i>7)
				break;
			list[i]=p;
			i++;
		}
	}
	if(i==0){
		printf("�˻� ����� �����ϴ�. �ƹ� Ű�� �����ø� ���� ȭ������ �̵��մϴ�.");
		getch();
		system("cls");
		return;
	}
	system("cls");
	printf("�˻� ����� %d�� �ֽ��ϴ�.\n\n", i);
	j=i;
	for(i=0; i<j; ++i){
		printf("[%d]: %s\n", i+1, list[i]->bookname);
	}
	printf("���� ȭ������ ������ 0��, �����Ϸ��� ���� ��ȣ��, ���� ����� 9�� �����ּ���.\n");
	do {
		fflush(stdin);
		ch=getch();
		i=(int)ch-48;
		if(i==9)
			break;
	} while(i>j);
	if(i==0){
		system("cls");
		return;
	}
	if(i==9)
		goto goback;
	--i;
	if(Reserve_Check(list[i]->bookname) == 0 && list[i]->current == 0 ){
			Borrow_Book(list[i]->bookname);
		}
	else
	{
		if(list[i]->current==currentuser->std_num){
			printf("\nȸ������ �������� �����Դϴ�. �ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�. ");
			getch();
			system("cls");
			return;
		}
		else if(list[i]->current==0 && list[i]->head->next->std_num==currentuser->std_num){
			printf("ȸ������ �����ߴ� ������, ���� 0�����Դϴ�.\n");
			Borrow_Book(list[i]->bookname);
			return;
		}
		else if(Is_Reserved_By_User(list[i]->bookname)>0){
			printf("\n�̹� ���� ��ܿ� �ö� �ֽ��ϴ�. ���� %d�����Դϴ�.\n", Is_Reserved_By_User(list[i]->bookname));
			printf("�ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�. ");
			getch();
			system("cls");
			return;
		}
		else if(list[i]->current != 0){
			printf("\n�̹� ����� �����Դϴ�. �����ڴ� %d���Դϴ�. ", Reserve_Check(list[i]->bookname));
		}

		else
		{
			printf("\n�������� �ƴϳ�, �����ڰ� %d�� �����մϴ�. ", Reserve_Check(list[i]->bookname));
		}
		printf("�����Ͻðڽ��ϱ�? (Y: ��, N: �ƴϿ�)\n\n");
		ch='a';
		while(ch!='Y'&&ch!='N'&&ch!='y'&&ch!='n'){
			ch=getch();
		}
		if(ch=='N'||ch=='n')
			return;
		else{
			Reserve_Book(list[i]->bookname);
		}
	}
	return;
}

int Is_Reserved_By_User(char *bookname){
	int count=0;
	ReservedList *p=Access_book(bookname)->head->next;
	Book *book=Access_book(bookname);
	if(p==book->tail)
		return 0;
	while(p!=book->tail){
		if(p->std_num==currentuser->std_num)
			return count+1;
		p=p->next;
		++count;
	}
	return 0;
}

void MakeAdmin(void)
{
	StudentList *create=(StudentList*)malloc(sizeof(StudentList));
	char password[100]="password";
	Search_last()->next=create;
	create->next=SLtail;
	Encrypt(password);
	strcpy(create->id, "admin");
	strcpy(create->password, password);
	strcpy(create->name, "admin");

	create->std_num=count++;
	create->head=(StudentBookLog*)malloc(sizeof(StudentBookLog));
	create->tail=(StudentBookLog*)malloc(sizeof(StudentBookLog));
	create->head->next=create->tail;
	create->tail->next=NULL;
}

void Addbook(char *bookname)
{
	FILE *fp;
	Book *p, *create;
	int * ip;
	int i=0;
	p=Bhead;
	while(p->next!=Btail)
	{
		p=p->next;
	}
	create=(Book*)malloc(sizeof(Book));			//���ο� å �޸� �Ҵ�, �׸��� å ����Ʈ�� �����Ų��
	p->next=create;
	create->next=Btail;
	strcpy(create->bookname, bookname);
	create->current=0;
	create->bookid=bcount++;

	ip = (int*)malloc(sizeof(int)*10);

	create->head=(ReservedList*)malloc(sizeof(ReservedList));	//å ����ü �ȿ� ����� ���ο� ���Ḯ��Ʈ��, ������ ��Ȳ ����Ʈ �޸� �Ҵ� �� �ʱ�ȭ(����)
	create->tail=(ReservedList*)malloc(sizeof(ReservedList));
	create->head->next=create->tail;
	create->tail->next=NULL;

	fp=fopen("BookDB.txt", "a+");
	fprintf(fp, "%c", 10);
	fputs(bookname, fp);
	fclose(fp);
}
void Addbook_s(char *bookname)
{
	Book *p, *create;
	int * ip;
	int i=0;
	p=Bhead;
	while(p->next!=Btail)
	{
		p=p->next;
	}
	create=(Book*)malloc(sizeof(Book));			//���ο� å �޸� �Ҵ�, �׸��� å ����Ʈ�� �����Ų��
	p->next=create;
	create->next=Btail;

	while(bookname[i]!=10){
		create->bookname[i]=bookname[i];			//å ����ü �ȿ� �ʿ��� ����, ������ �ʱ�ȭ, �Ϸù�ȣ �ο�
		++i;
	}
	create->bookname[i]=0;
	create->current=0;
	create->bookid=bcount++;

	ip = (int*)malloc(sizeof(int)*10);

	create->head=(ReservedList*)malloc(sizeof(ReservedList));	//å ����ü �ȿ� ����� ���ο� ���Ḯ��Ʈ��, ������ ��Ȳ ����Ʈ �޸� �Ҵ� �� �ʱ�ȭ(����)
	create->tail=(ReservedList*)malloc(sizeof(ReservedList));
	create->head->next=create->tail;
	create->tail->next=NULL;
}


void Deletebook(char *bookname)
{
	Book *p=Bhead, *trash;
	ReservedList *pp, *pptrash;
	StudentBookLog *ppp, *ppptrash;
	while(p->next!=Btail)
	{
		if(strcmp(p->next->bookname, bookname)==0){
			trash=p->next;
			p->next=p->next->next;						//��� ����(������ ���� ����)

			if(trash->current!=0){										//�л� ����ü���� ������ å ��� ����(malloc free)
				ppp=Search_std_num(trash->current)->head;
				while(ppp->next!=Search_std_num(trash->current)->tail){
					if(ppp->next->bookid==trash->bookid){
						ppptrash=ppp->next;
						ppp->next=ppp->next->next;
						free(ppptrash);
						break;
					}
					ppp=ppp->next;
				}
			}
			
			pp=trash->head->next;
			while(pp!=trash->tail)										//���� å ����ü���� ���� ��� ����(malloc free)
			{
				pptrash=pp;
				pp=pp->next;
				free(pptrash);
			}
			free(pp);

			free(trash);												//���� å ����ü�� �޸� ����(malloc free)
			return;
		}
		p=p->next;
	}
}

void ChangeLineCh(char *input)
{
	int i=0;
	while(input[i]!=10){
		++i;
		if(i>50)
			return;
	}
	input[i]=0;
}

void Game(int g1, int g2, int *counter)
{
	char result[10];
	if(g2==1)
		strcpy(result, "����");
	if(g2==2)
		strcpy(result, "����");
	if(g2==3)
		strcpy(result, "��");
	printf("��ǻ�ʹ� [%s]�� �½��ϴ�.\n\n", result);

	if(g1==g2)
		printf("�����ϴ�. �ٽ� ���ּ���.\n");
	else if(g1==1&&g2==2)
		printf("�����ϴ�. �ٽ� ���ּ���.\n");
	else if(g1==1&&g2==3)
		printf("�̰���ϴ�! %d���� �� �̱�� ���α׷��� ����˴ϴ� :)\n", --(*counter));
	else if(g1==2&&g2==1)
		printf("�̰���ϴ�! %d���� �� �̱�� ���α׷��� ����˴ϴ� :)\n", --(*counter));
	else if(g1==2&&g2==3)
		printf("�����ϴ�. �ٽ� ���ּ���.\n");
	else if(g1==3&&g2==1)
		printf("�����ϴ�. �ٽ� ���ּ���.\n");
	else if(g1==3&&g2==2)
		printf("�̰���ϴ�! %d���� �� �̱�� ���α׷��� ����˴ϴ� :)\n", --(*counter));
	printf("\n�ƹ� Ű�� ������ �Ѿ�ϴ�...");
	getch();
	system("cls");
}

void Gameexecute(void)
{
	int g1, g2, counter=3;
	char game;
	printf("���α׷��� �����Ͻ÷��� ��ǻ�Ϳ��� ���� ���� ���� �̱�ž� �մϴ� :P\n");
	while(counter!=0)
	{
		printf("1:����, 2:����, 3:�� // �Է��ϼ���: ");
		game=getch();
		switch (game)
		{
		case '1': printf("[����]�� ���̽��ϴ�.\n");
			printf("\n��ǻ�Ͱ� �������Դϴ�. ������� �Է��� ���ĺ����� �ʾƿ� :D\n\n");
			g1=1;
			g2=rand()%3+1;
			Sleep(3000);
			Game(g1, g2, &counter);
			break;
		case '2': printf("[����]�� ���̽��ϴ�.\n");
			printf("\n��ǻ�Ͱ� �������Դϴ�. ������� �Է��� ���ĺ����� �ʾƿ� :D\n\n");
			g1=2;
			g2=rand()%3+1;
			Sleep(3000);
			Game(g1, g2, &counter);
			break;
		case '3': printf("[��]�� ���̽��ϴ�.\n");
			printf("\n��ǻ�Ͱ� �������Դϴ�. ������� �Է��� ���ĺ����� �ʾƿ� :D\n\n");
			g1=3;
			g2=rand()%3+1;
			Sleep(3000);
			Game(g1, g2, &counter);
			break;
		default: printf("�߸��� �Է��Դϴ�.\n");
			break;
		}
	}

}

void Intro(void)
{
	int i, color;
	char print[200]="                 ���� �������� ���� ���� �������� ȯ���մϴ�.\n\n";
	char sys[10];

	for(i=2; print[i-2]!=0||print[i-1]!=0; i+=2)
	{
		if(print[i]==32&&print[i]<0)
		{
			printf("%c", print[i]);
			--i;
			continue;
		}
		printf("%c%c", print[i], print[i+1]);

		if(print[i]!=32){
			Intro_Color(sys);
			system(sys);
			Sleep(200);
		}
	}
	Sleep(3000);
	system("color a");

}

void Intro_Color(char *sys)
{
	int i=rand()%15;
	if(i==0)
		strcpy(sys, "color a");
	else if(i==1)
		strcpy(sys, "color 2");
	else if(i==2)
		strcpy(sys, "color c");
	else if(i==3)
		strcpy(sys, "color d");
	else if(i==4)
		strcpy(sys, "color 6");
	else if(i==5)
		strcpy(sys, "color 7");
	else if(i==6)
		strcpy(sys, "color 8");
	else if(i==7)
		strcpy(sys, "color e");
	else if(i==8)
		strcpy(sys, "color c");
	else if(i==10)
		strcpy(sys, "color a");
	else if(i==11)
		strcpy(sys, "color b");
	else if(i==12)
		strcpy(sys, "color d");
	else if(i==13)
		strcpy(sys, "color e");
	else if(i==14)
		strcpy(sys, "color f");
}

void Search_Account(char *search)
{
	StudentList *p=SLhead->next;
	StudentList *list[10];
	int i, j;
	char ch;
initial:
	i=0;
	j=0;
	if(SLhead->next==SLtail)
		return;
	for(; p!=SLtail; p=p->next)
	{
		if(SearchStr(search, p->name)>0){
			if(i>7)
				break;
			list[i]=p;
			i++;
		}
	}
	if(i==0){
		printf("�˻� ����� �����ϴ�. �ƹ� Ű�� �����ø� ���� ȭ������ �̵��մϴ�.");
		getch();
		system("cls");
		return;
	}
	system("cls");
	printf("�˻� ����� %d�� �ֽ��ϴ�.\n\n", i);
	j=i;
	for(i=0; i<j; ++i){
		printf("[%d]: %s(id: %s)\n", i+1, list[i]->name, list[i]->id);
	}
	printf("���� ȭ������ ������ 0��, ȸ�� ������ �ش� ��ȣ��, ���� ����� 9�� �����ּ���.\n");
	do {
		fflush(stdin);
		ch=getch();
		i=(int)ch-48;
		if(i==9)
			break;
	} while(i>j);
	if(i==0){
		system("cls");
		return;
	}
	else if(i==9)
		goto initial;
	else
	{
		int t=1;
		StudentBookLog *p;
		i--;
		printf("\n----------------------\n");
		printf("ȸ�� �̸�: %s\n", list[i]->name);
		printf("ȸ�� id: %s\n", list[i]->id);

		if(list[i]->head->next==list[i]->tail)
		{
			printf("\n* ������ ������ �����ϴ�. ���� �޴��� ���ư����� �ƹ� Ű�� �����ʽÿ�. ");
			getch();
			system("cls");
			return;
		}
		p=list[i]->head->next;
		printf("������ ���� ���\n");
		printf("----------------------\n");
		while(p!=list[i]->tail)
		{
			printf("| %d | %s\n", i, Search_bookid(p->bookid)->bookname);
			p=p->next;
			t++;
		}
		printf("----------------------\n");
		printf("���� �޴��� ���ư����� �ƹ� Ű�� �����ʽÿ�. ");
		getch();
		system("cls");

	}
}

void Return_Book(char *bookname)
{
		StudentBookLog *p, *temp;
		Book* b = Access_book(bookname);
		char ch;
		b->current=0;		//å�� ������ ������ 0���� �ٲ۴�.
		p=currentuser->head;					//���� �κ��� ���� ������ �������� ���� å ����
		while(p->next != currentuser->tail){
			if(b->bookid == p->next->bookid)
				break;
			p=p->next;
		}
		temp=p->next;
		p->next=p->next->next;
		free(temp);
		system("cls");
		printf("* '%s' �ݳ� �Ϸ�Ǿ����ϴ�.\n\n", bookname);

		//�����ݳ��Լ�. �ؾ� �� ��: ���� �������� StudentBookLog���� ���⵵������de-link�ϰ�, �ݵ�� free ���ٰ�.
		//free ���: �ӽ� StudentBookLog �����͸� �ϳ� �� ���� ���� ��带 ����Ű�� �� ���� de-link ����. �׸��� �Լ� ������ ���� free(������)����.
		//�� �׷��� ����? �ӽ� ������ �� �����, de-link �ϰ� ���� �� �̻� �װ� ������ ��� �����ϱ�.
		//�� �ؾ� �� ��: å���� current(����� ��ȣ �����ϴ� int ����)=0 ����� �Ѵ�. �װ� �ƹ��� ���ٴ� ��.
}

void Print_Reserved_Book(StudentList *user)			//���� ���� ��� ���
{
	int i=1, list=0;
	ReservedList *p;
	Book* b = Bhead;
	
	p=Bhead->next->head;
	printf("\n������ ���� ���\n");
	printf("----------------------\n");
	while(b != Btail)			//Btail�� ���� �� Ž�� �ߴ�
	{
		p=b->head;
		while(p != b->tail)	//å���� currentuser�� �й��� �ִ��� Ž��
		{
			list++;
			if(currentuser->std_num == p->std_num)
			{
				printf("| %d | %s (���� %d����)\n", i, b->bookname, list-1);
				i++;
			}
			p=p->next;
		}
		list=0;
		b=b->next;	//�� �� Ž���� ������ ���� å���� �Ѿ
	}
}

void Reserve_Book_Cancel(char *bookname)		//���� ���
{
	//���� ����� �ҽ��� ���� �ҽ���.
	ReservedList *p, *temp;
	Book* b = Access_book(bookname);

	//���� �κ��� å db���� ���� ���� ����
	p= b->head;
	while(p->next != b->tail)
	{
		if(currentuser->std_num == p->next->std_num)
			break;
		p=p->next;
	}
	temp=p->next;
	p->next=p->next->next;
	free(temp);
	system("cls");
	printf("* '%s' ���� ��� �Ϸ�Ǿ����ϴ�.\n\n", bookname);
}