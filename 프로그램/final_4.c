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
char *Encrypt(char *password);		//패스워드 암호화
int Validify(char *input, char *password);		//암호화된 패스워드 확인
void Password_Input(char *password);//패스워드 보안 입력 기능
int  SearchStr(char *str1, char *str2);//문자열 찾기 함수
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

StudentList *SLhead, *SLtail, *currentuser; //CurrentUser 전역으로 이동. 대출 등 기타기능 구현을 편리하게
Book *Bhead, *Btail;

int main()
{
	char select;
	char id[100], password[100], name[100], book_search[100];				//회원가입 scanf용 변수
	char ch;
	char bookdb[100],  userdb_id[100], userdb_password[100], userdb_name[100];
	Book *searchedbook;
	ReservedList *trash;
	FILE *fp1, *fp2;
	SLhead=(StudentList*)malloc(sizeof(StudentList));	//학생 리스트 헤드-테일 메모리 할당
	SLtail=(StudentList*)malloc(sizeof(StudentList));
	Bhead=(Book*)malloc(sizeof(Book));					//책 리스트 헤드-테일 메모리 할당
	Btail=(Book*)malloc(sizeof(Book));
	SLhead->next=SLtail;
	SLtail->next=NULL;
	Bhead->next=Btail;
	Btail->next=NULL;									//헤드-테일 연결


	MakeAdmin(); //admin 계정 생성(아이디: admin, 비밀번호: password)

	
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
		printf("|        도서관 프로그램        |\n");
		printf("---------------------------------\n");
		printf("| 1: 로그인 2: 회원가입 3: 종료 |\n");
		printf("---------------------------------\n\n");
		printf("선택해주세요");
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
			printf("<회원가입>\n");
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
			printf("축하합니다! 가위 바위 보 지옥을 빠져나오셨습니다!\n\n");
			return 0; break; //quit
		default:
			system("cls");
			printf("* 잘못 선택하셨습니다. 다시 선택해주세요\n\n");
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
			printf("|         1 : 회원 검색                    |\n"); //여기서 대출자, 예약자 출력 적용
			printf("|         2 : 도서 추가                    |\n");
			printf("|         3 : 도서 삭제                    |\n");
			printf("|         4 : 로그아웃                     |\n");
			printf("|         5 : 프로그램 종료                |\n");
			printf("|                                          |\n");
			printf("--------------------------------------------\n\n");
			printf("선택 : ");
			select=getch();
			switch(select)
			{
			case '1': printf("검색할 회원의 이름의 일부 혹은 전체를 입력하세요: ");
				gets(input);
				Search_Account(input);
				break;
			case '2': printf("추가할 도서의 제목을 입력하세요: ");
				gets(input);
				if(Access_book(input)!=NULL){
					printf("이미 해당 도서가 존재합니다.\n\n아무 키나 누르시면 메인 메뉴로 돌아갑니다.\n");
					getch();
					system("cls");
					break;
				}
				Addbook(input);
				printf("도서 등록에 성공하였습니다. 아무 키나 누르시면 메인 메뉴로 돌아갑니다. ");
				getch();
				system("cls");
				break;
			case '3':printf("삭제할 도서의 제목을 입력하세요: ");
				gets(input);
				if(Access_book(input)==NULL){
					printf("삭제할 도서가 없습니다.\n\n아무 키나 누르시면 메인 메뉴로 돌아갑니다.\n");
					getch();
					system("cls");
					break;
				}
				Deletebook(input);
				printf("도서 삭제에 성공하였습니다. 아무 키나 누르시면 메인 메뉴로 돌아갑니다. ");
				getch();
				system("cls");
				break;
			case '4':
				system("cls");
				goto logout;
			case '5':
				Gameexecute();
				printf("축하합니다! 가위 바위 보 지옥을 빠져나오셨습니다!\n\n");
				return 0;
			}
		}
	}
	else
	{
		while(1){

			printf("----------------- M E N U ------------------\n");
			printf("|                                          |\n");
			printf("|         1 : 도서 대출                    |\n");
			printf("|         2 : 도서 검색                    |\n");
			printf("|         3 : 도서 반납                    |\n");
			printf("|         4 : 도서 예약                    |\n");
			printf("|         5 : 도서 예약 취소               |\n");
			printf("|         6 : 대출 도서 목록               |\n");
			printf("|         7 : 예약 도서 목록               |\n");
			printf("|         8 : 로그아웃                     |\n");
			printf("|         0 : 프로그램 종료                |\n");
			printf("|                                          |\n");
			printf("--------------------------------------------\n\n");
			printf("선택 : ");
			select=getch();
			switch (select)
			{
			case '1' :
				system("cls");
				printf("\n--------------------------------------------\n");
				printf("|  [도서 대출]                              |\n");
				printf("|  대출을 원하는 서적의 도서명을 입력하세요 |\n");
				printf("--------------------------------------------\n\n");
				printf("도서명 : ");
				scanf("%[^\n]", book_search);
				searchedbook=Access_book(book_search);
				if(searchedbook == NULL)
				{
					printf("* 찾으시는 도서가 없습니다. 아무 키나 누르시면 메인 메뉴로 이동합니다.\n");
					getch();
					system("cls");
				}
				else
				{
					if(Reserve_Check(book_search) == 0 && searchedbook->current == 0 ){
						printf("\n대출 가능합니다. 대출하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
							printf("\n회원님이 대출중인 도서입니다. 아무 키나 누르면 메인 메뉴로 돌아갑니다. ");
							getch();
							system("cls");
							break;
						}
						else if(searchedbook->current==0 && searchedbook->head->next->std_num==currentuser->std_num){
							printf("회원님이 예약했던 도서로, 예약 0순위입니다.\n");
							printf("아무 키나 누르면 대출 후 메인 메뉴로 돌아갑니다.");
							getch();
							Borrow_Book(book_search);
							trash=searchedbook->head->next;
							searchedbook->head->next=searchedbook->head->next->next;
							free(trash);
							break;
						}
						else if(Is_Reserved_By_User(book_search)>0){
							printf("\n이미 예약 명단에 올라가 있습니다. 예약 %d순위입니다.\n", Is_Reserved_By_User(book_search));
							printf("아무 키나 누르면 메인 메뉴로 돌아갑니다. ");
							getch();
							system("cls");
							break;
						}
						else if(searchedbook->current != 0){
							printf("\n이미 대출된 도서입니다. 예약자는 %d명입니다. ", Reserve_Check(book_search));
						}
						
						else
						{
							printf("\n대출중은 아니나, 예약자가 %d명 존재합니다. ", Reserve_Check(book_search));
						}
						printf("예약하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
				printf("|  [도서 검색]                              |\n");
				printf("|  서적의 도서명 일부를 입력하세요          |\n");
				printf("--------------------------------------------\n\n");
				printf("도서명 : ");
				scanf("%[^\n]", book_search);
				Book_Search_Solution(book_search);
				break;
			case '3' :
				system("cls");
				Print_Borrowed_Book(currentuser);								//반납을 용이하게 하기 위해 대출 도서 목록을 보여줍니다
				printf("\n--------------------------------------------\n");
				printf("|  [도서 반납]                              |\n");
				printf("|  반납을 원하는 서적의 도서명을 입력하세요 |\n");
				printf("--------------------------------------------\n\n");
				printf("도서명 : ");
				scanf("%[^\n]", book_search);
				searchedbook=Access_book(book_search);
				if(searchedbook == NULL)
				{
					printf("* 찾으시는 도서가 없습니다. 아무 키나 누르시면 메인 메뉴로 이동합니다.\n");
					getch();
					system("cls");
				}
				else
				{
					if(Reserve_Check(book_search) == 0 && searchedbook->current == 0 ){
						printf("회원님이 대출하지 않은 도서입니다\n");
						printf("\n대출 가능합니다. 대출하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
							printf("\n회원님이 대출중인 도서입니다. 반납하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
							printf("회원님이 예약했던 도서로, 예약 0순위입니다.\n");
							printf("예약을 취소하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
							printf("대출하신 책은 아니지만 예약 명단에 올라가 있습니다. 예약 %d순위입니다.\n", Is_Reserved_By_User(book_search));
							printf("예약을 취소하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
							printf("회원님이 대출/예약한 도서가 아닙니다. 아무 키나 누르시면 메인 메뉴로 이동합니다.\n");
							getch();
							system("cls");
						}
						else
						{
							printf("회원님이 대출/예약한 도서가 아닙니다. 아무 키나 누르시면 메인 메뉴로 이동합니다.\n");
							getch();
							system("cls");
						}
					}
				}
				break;
			case '4' :
				system("cls");
				printf("\n--------------------------------------------\n");
				printf("|  [도서 예약]                              |\n");
				printf("|  예약을 원하는 서적의 도서명을 입력하세요 |\n");
				printf("--------------------------------------------\n\n");
				printf("도서명 : ");
				scanf("%[^\n]", book_search);
				searchedbook=Access_book(book_search);
				if(searchedbook == NULL){
					printf("* 찾으시는 도서가 없습니다. 아무 키나 누르시면 메인 메뉴로 이동합니다.\n");
					getch();
					system("cls");
				}
				else if(searchedbook->current==currentuser->std_num){
					printf("\n회원님이 대출중인 도서입니다. 아무 키나 누르면 메인 메뉴로 돌아갑니다. ");
					getch();
					system("cls");
					break;
				}
				else if(Is_Reserved_By_User(book_search)>0){
					printf("\n이미 예약 명단에 올라가 있습니다. 예약 %d순위입니다.\n", Is_Reserved_By_User(book_search));
					printf("아무 키나 누르면 메인 메뉴로 돌아갑니다. ");
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
				printf("|  [도서 예약 취소]                              |\n");
				printf("|  예약 취소를 원하는 서적의 도서명을 입력하세요 |\n");
				printf("  --------------------------------------------\n\n");
				printf("도서명 : ");
				scanf("%[^\n]", book_search);
				searchedbook=Access_book(book_search);
				if(searchedbook == NULL){
					printf("* 찾으시는 도서가 없습니다. 아무 키나 누르시면 메인 메뉴로 이동합니다.\n");
					getch();
					system("cls");
				}
				else if(searchedbook->current==currentuser->std_num){
					printf("\n회원님이 이미 대출중인 도서입니다. 반납하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
					printf("\n예약 명단에 올라가 있습니다. 예약 %d순위입니다.\n", Is_Reserved_By_User(book_search));
					printf("예약을 취소하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
					printf("\n예약하지 않은 도서입니다.\n");
					printf("아무 키나 누르면 메인 메뉴로 돌아갑니다. ");
					getch();
					system("cls");
					break;
				}
				break;
			case '6':
				Print_Borrowed_Book(currentuser);
				printf("----------------------\n");
				printf("메인 메뉴로 돌아가려면 아무 키나 누르십시오. ");
				getch();
				system("cls");
				break;
			case '7':
				Print_Reserved_Book(currentuser);
				printf("메인 메뉴로 돌아가려면 아무 키나 누르십시오. ");
				getch();
				system("cls");
				break;
			case '8':
				system("cls");
				printf("성공적으로 로그아웃 되었습니다!\n\n");
				goto logout;
			case '0':
				Gameexecute();
				printf("축하합니다! 가위 바위 보 지옥을 빠져나오셨습니다!\n\n");
				return 0;
			default:
				system("cls");
				printf("\n* 잘못 선택하셨습니다. 다시 선택해주세요.\n\n");
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
		printf("* 아이디 또는 비밀번호를 다시 확인하세요\n");
		printf("등록되지 않은 아이디이거나, 아이디 또는 비밀번호를 잘못 입력하셨습니다.\n\n");
		return NULL;
	}
	system("cls");
	printf("\n로그인 성공!\n");
	printf("[%s]님 환영합니다.\n\n", Search_id(id)->name);
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
		printf("* 이미 존재하는 아이디입니다.\n");
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
	printf("* 회원가입이 완료되었습니다.\n\n");
}

void Create_Account_s(char *id, char *password, char *name)
{
	StudentList *create=(StudentList*)malloc(sizeof(StudentList));
	if(Search_id(id)!=NULL){
		system("cls");
		printf("* 이미 존재하는 아이디입니다.\n");
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
	printf("* 회원가입이 완료되었습니다.\n\n");
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

	b->current=currentuser->std_num;		//책의 대출자 변수에 유저 번호 삽입(대출프로세스)
	p=currentuser->head;					//하위 부분은 현재 유저의 정보에 빌린 책 삽입
	while(p->next != currentuser->tail){
		p=p->next;
	}
	node=(StudentBookLog*)malloc(sizeof(StudentBookLog));
	p->next = node;
	node->next = currentuser->tail;
	node->bookid = b->bookid;
	system("cls");
	printf("* '%s' 대출 완료되었습니다.\n\n", bookname);

}

void Reserve_Book(char *bookname)
{
	ReservedList *p2, *node2;
	Book* b = Access_book(bookname);
	node2=(ReservedList*)malloc(sizeof(ReservedList));

	if(b==NULL)
		return;

	//하위 부분은 책 db에 예약 유저 삽입
	p2= b->head;
	while(p2->next != b->tail)
	{
		p2=p2->next;
	}
	p2->next = node2;
	node2->next = b->tail;
	node2->std_num = currentuser->std_num;
	system("cls");
	printf("* '%s' 예약 완료되었습니다.\n\n", bookname);
	//이미 대출자가 있으면 예약으로 뜨게
	//currentuser가 있는지 확인하고 이미 예약 또는 대출한 도서입니다.
	//예약자의 경우 책 리스트 하단에만 추가되고 학생 리스트 하단에는 추가되지 않게 -> 상위 문제 모두 해결됨. 이건 예약 리스트에 추가하기만 하는 함수임.
}

void Print_Borrowed_Book(StudentList *user)
{
	int i=1;
	StudentBookLog *p;
	if(user->head->next==user->tail)
	{
		printf("\n* 대출한 도서가 없습니다. 메인 메뉴로 돌아가려면 아무 키나 누르십시오. ");
		getch();
		system("cls");
		return;
	}
	p=user->head->next;
	printf("대출한 도서 목록\n");
	printf("----------------------\n");
	while(p!=user->tail)
	{
		printf("| %d | %s\n", i, Search_bookid(p->bookid)->bookname);
		p=p->next;
		i++;
	}
}

/*
1. 대출자 있는지 확인하는 함수
2. 대출자 출력하는 함수
3. 대출자 및 예약자 출력하는 함수
4. 현재 사용자 예약에 추가하는 함수
5. 예약 취소하는 함수
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

while(p->next != Access_book(bookname)->tail) 작성중이던 함수
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

int Validify(char *input, char *password)	//input: 확인할 암호 password: 실제 암호(암호화된 값)
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
		printf("검색 결과가 없습니다. 아무 키나 누르시면 메인 화면으로 이동합니다.");
		getch();
		system("cls");
		return;
	}
	system("cls");
	printf("검색 결과가 %d건 있습니다.\n\n", i);
	j=i;
	for(i=0; i<j; ++i){
		printf("[%d]: %s\n", i+1, list[i]->bookname);
	}
	printf("메인 화면으로 가려면 0을, 대출하려면 도서 번호를, 다음 목록은 9를 눌러주세요.\n");
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
			printf("\n회원님이 대출중인 도서입니다. 아무 키나 누르면 메인 메뉴로 돌아갑니다. ");
			getch();
			system("cls");
			return;
		}
		else if(list[i]->current==0 && list[i]->head->next->std_num==currentuser->std_num){
			printf("회원님이 예약했던 도서로, 예약 0순위입니다.\n");
			Borrow_Book(list[i]->bookname);
			return;
		}
		else if(Is_Reserved_By_User(list[i]->bookname)>0){
			printf("\n이미 예약 명단에 올라가 있습니다. 예약 %d순위입니다.\n", Is_Reserved_By_User(list[i]->bookname));
			printf("아무 키나 누르면 메인 메뉴로 돌아갑니다. ");
			getch();
			system("cls");
			return;
		}
		else if(list[i]->current != 0){
			printf("\n이미 대출된 도서입니다. 예약자는 %d명입니다. ", Reserve_Check(list[i]->bookname));
		}

		else
		{
			printf("\n대출중은 아니나, 예약자가 %d명 존재합니다. ", Reserve_Check(list[i]->bookname));
		}
		printf("예약하시겠습니까? (Y: 예, N: 아니오)\n\n");
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
	create=(Book*)malloc(sizeof(Book));			//새로운 책 메모리 할당, 그리고 책 리스트에 연결시킨다
	p->next=create;
	create->next=Btail;
	strcpy(create->bookname, bookname);
	create->current=0;
	create->bookid=bcount++;

	ip = (int*)malloc(sizeof(int)*10);

	create->head=(ReservedList*)malloc(sizeof(ReservedList));	//책 구조체 안에 생기는 새로운 연결리스트인, 예약자 현황 리스트 메모리 할당 및 초기화(연결)
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
	create=(Book*)malloc(sizeof(Book));			//새로운 책 메모리 할당, 그리고 책 리스트에 연결시킨다
	p->next=create;
	create->next=Btail;

	while(bookname[i]!=10){
		create->bookname[i]=bookname[i];			//책 구조체 안에 필요한 제목, 대출자 초기화, 일련번호 부여
		++i;
	}
	create->bookname[i]=0;
	create->current=0;
	create->bookid=bcount++;

	ip = (int*)malloc(sizeof(int)*10);

	create->head=(ReservedList*)malloc(sizeof(ReservedList));	//책 구조체 안에 생기는 새로운 연결리스트인, 예약자 현황 리스트 메모리 할당 및 초기화(연결)
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
			p->next=p->next->next;						//노드 삭제(포인터 연결 수정)

			if(trash->current!=0){										//학생 구조체에서 대출한 책 목록 삭제(malloc free)
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
			while(pp!=trash->tail)										//현재 책 구조체에서 예약 목록 삭제(malloc free)
			{
				pptrash=pp;
				pp=pp->next;
				free(pptrash);
			}
			free(pp);

			free(trash);												//현재 책 구조체의 메모리 삭제(malloc free)
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
		strcpy(result, "가위");
	if(g2==2)
		strcpy(result, "바위");
	if(g2==3)
		strcpy(result, "보");
	printf("컴퓨터는 [%s]를 냈습니다.\n\n", result);

	if(g1==g2)
		printf("비겼습니다. 다시 해주세요.\n");
	else if(g1==1&&g2==2)
		printf("졌습니다. 다시 해주세요.\n");
	else if(g1==1&&g2==3)
		printf("이겼습니다! %d번만 더 이기면 프로그램이 종료됩니다 :)\n", --(*counter));
	else if(g1==2&&g2==1)
		printf("이겼습니다! %d번만 더 이기면 프로그램이 종료됩니다 :)\n", --(*counter));
	else if(g1==2&&g2==3)
		printf("졌습니다. 다시 해주세요.\n");
	else if(g1==3&&g2==1)
		printf("졌습니다. 다시 해주세요.\n");
	else if(g1==3&&g2==2)
		printf("이겼습니다! %d번만 더 이기면 프로그램이 종료됩니다 :)\n", --(*counter));
	printf("\n아무 키나 누르면 넘어갑니다...");
	getch();
	system("cls");
}

void Gameexecute(void)
{
	int g1, g2, counter=3;
	char game;
	printf("프로그램을 종료하시려면 컴퓨터와의 가위 바위 보를 이기셔야 합니다 :P\n");
	while(counter!=0)
	{
		printf("1:가위, 2:바위, 3:보 // 입력하세요: ");
		game=getch();
		switch (game)
		{
		case '1': printf("[가위]를 내셨습니다.\n");
			printf("\n컴퓨터가 생각중입니다. 사용자의 입력을 훔쳐보지는 않아요 :D\n\n");
			g1=1;
			g2=rand()%3+1;
			Sleep(3000);
			Game(g1, g2, &counter);
			break;
		case '2': printf("[바위]를 내셨습니다.\n");
			printf("\n컴퓨터가 생각중입니다. 사용자의 입력을 훔쳐보지는 않아요 :D\n\n");
			g1=2;
			g2=rand()%3+1;
			Sleep(3000);
			Game(g1, g2, &counter);
			break;
		case '3': printf("[보]를 내셨습니다.\n");
			printf("\n컴퓨터가 생각중입니다. 사용자의 입력을 훔쳐보지는 않아요 :D\n\n");
			g1=3;
			g2=rand()%3+1;
			Sleep(3000);
			Game(g1, g2, &counter);
			break;
		default: printf("잘못된 입력입니다.\n");
			break;
		}
	}

}

void Intro(void)
{
	int i, color;
	char print[200]="                 가상 도서관에 오신 것을 진심으로 환영합니다.\n\n";
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
		printf("검색 결과가 없습니다. 아무 키나 누르시면 메인 화면으로 이동합니다.");
		getch();
		system("cls");
		return;
	}
	system("cls");
	printf("검색 결과가 %d건 있습니다.\n\n", i);
	j=i;
	for(i=0; i<j; ++i){
		printf("[%d]: %s(id: %s)\n", i+1, list[i]->name, list[i]->id);
	}
	printf("메인 화면으로 가려면 0을, 회원 정보는 해당 번호를, 다음 목록은 9를 눌러주세요.\n");
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
		printf("회원 이름: %s\n", list[i]->name);
		printf("회원 id: %s\n", list[i]->id);

		if(list[i]->head->next==list[i]->tail)
		{
			printf("\n* 대출한 도서가 없습니다. 메인 메뉴로 돌아가려면 아무 키나 누르십시오. ");
			getch();
			system("cls");
			return;
		}
		p=list[i]->head->next;
		printf("대출한 도서 목록\n");
		printf("----------------------\n");
		while(p!=list[i]->tail)
		{
			printf("| %d | %s\n", i, Search_bookid(p->bookid)->bookname);
			p=p->next;
			t++;
		}
		printf("----------------------\n");
		printf("메인 메뉴로 돌아가려면 아무 키나 누르십시오. ");
		getch();
		system("cls");

	}
}

void Return_Book(char *bookname)
{
		StudentBookLog *p, *temp;
		Book* b = Access_book(bookname);
		char ch;
		b->current=0;		//책의 대출자 변수에 0으로 바꾼다.
		p=currentuser->head;					//하위 부분은 현재 유저의 정보에서 빌린 책 제거
		while(p->next != currentuser->tail){
			if(b->bookid == p->next->bookid)
				break;
			p=p->next;
		}
		temp=p->next;
		p->next=p->next->next;
		free(temp);
		system("cls");
		printf("* '%s' 반납 완료되었습니다.\n\n", bookname);

		//도서반납함수. 해야 할 것: 유저 정보에서 StudentBookLog에서 대출도서정보de-link하고, 반드시 free 해줄것.
		//free 방법: 임시 StudentBookLog 포인터를 하나 더 만들어서 없앨 노드를 가리키게 한 다음 de-link 해줌. 그리고 함수 끝나기 전에 free(포인터)해줌.
		//왜 그런지 알지? 임시 포인터 안 만들면, de-link 하고 나면 더 이상 그걸 참조할 방법 없으니까.
		//또 해야 할 것: 책에서 current(사용자 번호 저장하는 int 변수)=0 해줘야 한다. 그게 아무도 없다는 뜻.
}

void Print_Reserved_Book(StudentList *user)			//예약 도서 목록 출력
{
	int i=1, list=0;
	ReservedList *p;
	Book* b = Bhead;
	
	p=Bhead->next->head;
	printf("\n예약한 도서 목록\n");
	printf("----------------------\n");
	while(b != Btail)			//Btail에 도달 시 탐색 중단
	{
		p=b->head;
		while(p != b->tail)	//책에서 currentuser의 학번이 있는지 탐색
		{
			list++;
			if(currentuser->std_num == p->std_num)
			{
				printf("| %d | %s (예약 %d순위)\n", i, b->bookname, list-1);
				i++;
			}
			p=p->next;
		}
		list=0;
		b=b->next;	//한 권 탐색이 끝나면 다음 책으로 넘어감
	}
}

void Reserve_Book_Cancel(char *bookname)		//예약 취소
{
	//여기 참고용 소스는 예약 소스임.
	ReservedList *p, *temp;
	Book* b = Access_book(bookname);

	//하위 부분은 책 db에서 예약 유저 제외
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
	printf("* '%s' 예약 취소 완료되었습니다.\n\n", bookname);
}