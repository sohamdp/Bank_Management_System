#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>        
#include<sys/socket.h>
struct normaluser
{
	char name[50];
	int userid;
	char password[50];
	int account_no;
	float balance;
	int status;
};

struct jointuser
{
	char parent_name[50];
	char child_name[50];
	int userid;
	char password[50];
	int account_no;
	float balance;
	int status;
};
struct admin
{
	char name[50];
	int userid;
	char password[50];
};
void normal_user();
void joint_user();
void admin();
void new_account();
int get_new_normal_userid();
int existing_account();
void check_jointuser_credential(int,int);
void check_normaluser_credential(int,int);
void check_admin_credential(int,int);
void Deposit(int,int,int);
void withdraw(int,int,int);
void Balance_Enquiry(int,int,int);
void password_change(int,int,int);
void View_normaluser_details(int,int,int);
void View_jointuser_details(int,int,int);
void admin_menu(int);
void new_account()
{
	printf("\n\n");
	printf("\t\t\t\t***NEW ACCOUNT CREATION***\n");
	int choice,choice1;
	do
	{
		printf("1. Normal User\n2. Joint account user\n3. Admin \n\n");
		printf("Enter your choice:");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				normal_user();
				break;
			case 2:
				joint_user();
				break;
			case 3:
				admin();
				break;
		}
		printf("Do you want to create one more account type 1 for yes and 0 for no):");
		scanf("%d",&choice1);
	}while(choice1);
			
}
int get_new_normal_userid()
{
	struct normaluser record;
	int fd=open("normal_user.txt",O_RDONLY,0744);
	if(fd==-1)
	{
		printf("Error while opening the normal_user file\n");
	}
	else
	{
	    lseek(fd,-sizeof(record),SEEK_END);
	    read(fd,&record,sizeof(record));
	}
	return record.userid+1;
}
int get_new_joint_userid()
{
	struct normaluser record;
	int fd=open("joint_user.txt",O_RDONLY,0744);
	if(fd==-1)
	{
		printf("Error while opening the normal_user file\n");
	}
	else
	{
	    lseek(fd,-sizeof(record),SEEK_END);
	    read(fd,&record,sizeof(record));
	}
	return record.userid+1;
}
int get_new_admin_userid()
{
	struct admin record;
	int fd=open("admin.txt",O_RDONLY,0744);
	if(fd==-1)
	{
		printf("Error while opening the normal_user file\n");
	}
	else
	{
	    lseek(fd,-sizeof(record),SEEK_END);
	    read(fd,&record,sizeof(record));
	}
	return record.userid+1;
}
void normal_user()
{
	int fd=open("normal_user.txt",O_CREAT|O_RDWR,0744);
	if(fd==-1)
	{
		printf("Error while opening the normal_user file\n");
	}
	else
	{

		struct normaluser newuser;
		int size=lseek(fd,0,SEEK_END);
		int temp;
		printf("size of users:%d \n",size);
		if(size==0)//no user until
		{
			newuser.userid=1000;
			printf("userid:%d\n",newuser.userid);
		}
		else
		{
			newuser.userid=get_new_normal_userid();
			printf("userid:%d\n",newuser.userid);
		}
		printf("Please Enter the name of User:");
		scanf(" %[^\n]",newuser.name);
		printf("please enter the password(upto 15 characters):");
		scanf(" %[^\n]",newuser.password);
		newuser.account_no=(newuser.userid-1000)+100000;
		newuser.balance=0;
		newuser.status=1;
		write(fd,&newuser,sizeof(newuser));
		printf("***Normaluser Account Created Successfully***\n");
		close(fd);
	}
}

void joint_user()
{
	int fd=open("joint_user.txt",O_CREAT|O_RDWR,0744);
	if(fd==-1)
	{
		printf("Error while opening the normal_user file\n");
	}
	else
	{

		struct jointuser newuser;
		int size=lseek(fd,0,SEEK_END);
		int temp;
		printf("size of users:%d \n",size);
		if(size==0)//no user until
		{
			newuser.userid=1000;
			printf("userid:%d\n",newuser.userid);
		}
		else
		{
			newuser.userid=get_new_joint_userid();
			printf("userid:%d\n",newuser.userid);
		}
		printf("Please Enter the Parent_User name:");
		scanf(" %[^\n]",newuser.parent_name);
		printf("Please Enter the Child_User name:");
		scanf(" %[^\n]",newuser.child_name);
		printf("please enter the password(upto 15 characters):");
		scanf(" %[^\n]",newuser.password);
		newuser.status=1;
		newuser.account_no=(newuser.userid-1000)+100000;
		newuser.balance=0;
		write(fd,&newuser,sizeof(newuser));
		printf("***Jointuser Account Created Successfully***\n");
		close(fd);
	}
}

void admin()
{
	int fd=open("admin.txt",O_CREAT|O_RDWR,0744);
	if(fd==-1)
	{
		printf("Error while opening the normal_user file\n");
	}
	else
	{

		struct admin newuser;
		int size=lseek(fd,0,SEEK_END);
		int temp;
		printf("size of users:%d \n",size);
		if(size==0)//no user until
		{
			newuser.userid=1000;
			printf("userid:%d\n",newuser.userid);
		}
		else
		{
			newuser.userid=get_new_admin_userid();
			printf("userid:%d\n",newuser.userid);
		}
		printf("Please Enter the name of User:");
		scanf(" %[^\n]",newuser.name);
		printf("please enter the password(upto 15 characters):");
		scanf(" %[^\n]",newuser.password);
		write(fd,&newuser,sizeof(newuser));
		printf("***Admin Account Created Successfully***\n");
		close(fd);
	}
}

void check_normaluser_credential(int socketdescripter,int choice)
{
	struct normaluser curruser;
	printf("User ID:");
    scanf("%d",&curruser.userid);
    printf("Password:");
    scanf(" %[^\n]",curruser.password);
    // send credentials to server for checking ...if valid user or not
    write(socketdescripter,&choice,sizeof(int));
    write(socketdescripter,&curruser,sizeof(curruser));
    int result;
    read(socketdescripter,&result,sizeof(result));
    printf("result:%d\n",result);
    if(result==0)
    {
    	printf("****Invalid Login Credentials OR your account is deactivated****\n\n");// temparay change
    	printf("****Again select your choice and continue****\n\n");
    	existing_account(socketdescripter);
    }
    else
    {
    	//printf("****Successfully logged in!!!!!*****\n\n ");
    	//perform_task(socketdescripter);
    }
}
void check_jointuser_credential(int socketdescripter,int choice)
{
	struct jointuser curruser;
	printf("User ID:");
    scanf("%d",&curruser.userid);
    printf("Password:");
    scanf(" %[^\n]",curruser.password);
    // send credentials to server for checking ...if valid user or not
    write(socketdescripter,&choice,sizeof(int));
    write(socketdescripter,&curruser,sizeof(curruser));
    int result;
    read(socketdescripter,&result,sizeof(result));
    printf("result:%d\n",result);
    if(result==0)
    {
    	printf("****Invalid Login Credentials OR your account is deactivated****\n\n");
    	printf("****Again select your choice and continue****\n\n");
    	existing_account(socketdescripter);
    }
    else
    {
    	//printf("****Successfully logged in!!!!!*****\n\n ");
    	//perform_task(socketdescripter);
    }
}
void check_admin_credential(int socketdescripter,int choice)
{
	struct admin curruser;
	printf("User ID:");
    scanf("%d",&curruser.userid);
    printf("Password:");
    scanf(" %[^\n]",curruser.password);
    // send credentials to server for checking ...if valid user or not
    write(socketdescripter,&choice,sizeof(int));
    write(socketdescripter,&curruser,sizeof(curruser));
    int result;
    read(socketdescripter,&result,sizeof(result));
    printf("result:%d\n",result);
    if(result==0)
    {
    	printf("****Invalid Login Credentials****\n\n");
    	printf("****Again select your choice and continue****\n\n");
    	existing_account(socketdescripter);
    }
    else
    {
    	//printf("****Successfully logged in!!!!!*****\n\n ");
    	//perform_task(socketdescripter);
    }
}
int existing_account(int socketdescripter)// return result=choice
{
	printf("1.Normal_user\n2.joint_user\n3.Admin\n\n");
	int choice,result;
	printf("Enter your choice:");
	scanf("%d",&choice);
	printf("my choice:%d\n",choice);
	switch(choice)
	{
		case 1:
			check_normaluser_credential(socketdescripter,choice);
			break;
		case 2:
			check_jointuser_credential(socketdescripter,choice);
			break;
		case 3:
			printf("adminnn");
			check_admin_credential(socketdescripter,choice);
			break;
		case 4:
			 printf("Invalid option please select the valid option\n");
			 existing_account(socketdescripter);
	}
	return choice;
}
void normal_joint_menu(int socketdescripter,int option)
{
	while(1)
	{
	    printf("1.Deposit\n2.Withdraw\n3.Balance Enquiry\n4.Password Change\n5.View details\n6.Exit\n\n");
	    int choice,ch;
	    printf("Enter your choice:");
	    scanf("%d",&choice);
	   switch(choice)
	    {
	    	case 1:
	    		printf("option:%d\n",option);
	    		Deposit(socketdescripter,choice,option);
	    		break;
	   		case 2:
	   			printf("option:%d\n",option);
	    		withdraw(socketdescripter,choice,option);
	    		break;
	    	case 3:
	    		printf("option:%d\n",option);
	    		Balance_Enquiry(socketdescripter,choice,option);
	    		break;
	    	case 4:
	    		printf("option:%d\n",option);
	    		password_change(socketdescripter,choice,option);
	    		break;
	    	case 5:
	    		printf("option:%d\n",option);
	    		if(option==1)//normaluser
	    		{
	    			View_normaluser_details(socketdescripter,choice,option);
	    		}
	    		else if(option==2)//jointuser
	    		{
	    			View_jointuser_details(socketdescripter,choice,option);
	    		}
	    		
	    		break;
	    	case 6:
	    		write(socketdescripter,&choice,sizeof(choice));
	    		printf("\n\n****Thank you for visiting us****\n\n");
	    		read(socketdescripter,&ch,sizeof(ch));
	    		if(ch==1)
	    		{
	    			printf("normalclient disconnected from server");
	    		}
	    		else if(ch==2)
	    		{
	    			printf("jointclient disconnected from server");
	    		}
	    		exit(0);

	    }
	}

}

void Deposit(int socketdescripter,int choice,int option)
{
	
	//send data to server
	write(socketdescripter,&choice,sizeof(choice));
	float amount;
	printf("Total amount to Deposit:");
	scanf("%f",&amount);
	while(amount<=0)
	{
		printf("PLease Enter the correct amount:");
		scanf("%f",&amount);
	}
	write(socketdescripter,&amount,sizeof(amount));
	//read from server
	int result;
	read(socketdescripter,&result,sizeof(result));
	if(result==1)
	{
		printf("****Your Amount Deposited Successfully****\n\n");
	}
	else
	{
		printf("****Error occured while Depositing Money****\n");
	}
}


void withdraw(int socketdescripter,int choice,int option)
{
	//send data to server
	write(socketdescripter,&choice,sizeof(choice));
	float amount;
	printf("Total amount to withdraw:");
	scanf("%f",&amount);
	while(amount<=0)
	{
		printf("PLease Enter the correct amount:");
		scanf("%f",&amount);
	}
	write(socketdescripter,&amount,sizeof(amount));
	//read from server
	int result;
	read(socketdescripter,&result,sizeof(result));
	if(result==1)
	{
		printf("****Your Amount withdraw Successfully****\n\n");
	}
	else
	{
		printf("****Error occured while withdrawing Money****\n");
	}
}
void Balance_Enquiry(int socketdescripter,int choice,int option)
{
	float amount;
	//to server 
	write(socketdescripter,&choice,sizeof(int));

	//from server
	read(socketdescripter,&amount,sizeof(amount));
	printf("Available balance in account:%f\n",amount);
}
void password_change(int socketdescripter,int choice,int option)
{
	int result;
	//send choice to server
	write(socketdescripter,&choice,sizeof(choice));
	char pass[20];
	printf("Enter new Password:");
	scanf("%s",pass);
	write(socketdescripter,&pass,sizeof(pass));
	//from server
	read(socketdescripter,&result,sizeof(result));
	if(result==1)
	{
		printf("****password change Successfully****\n");
	}
	else
	{
		printf("****Error occured while changing password****\n");
	}
	//normal_joint_menu(socketdescripter,option);
}
void View_normaluser_details(int socketdescripter,int choice,int option)
{
	int id;
	// send choice server
	write(socketdescripter,&choice,sizeof(choice));// 5 will send
	write(socketdescripter,&option,sizeof(option));
	printf("Enter Userid to view account details:");
	scanf("%d",&id);
	write(socketdescripter,&id,sizeof(id));
	//read data from server
	printf("\t\t*****ALL Account Details*****\t\t\n\n");
	struct normaluser info;
	read(socketdescripter,&info,sizeof(info));
	printf("\t\tAccount holder name:%s\n",info.name);
	printf("\t\tuserid:%d\n",info.userid);
	printf("\t\tpassword:%s\n",info.password);
	printf("\t\taccount_no:%d\n",info.account_no);
	printf("\t\tAccount balance:%f\n",info.balance);
	printf("\t\tAccount status:%d\n",info.status);
	//normal_joint_menu(socketdescripter,option);
}
void View_jointuser_details(int socketdescripter,int choice,int option)
{
	int id;
	// send choice server
	write(socketdescripter,&choice,sizeof(choice));// 5 will send
	write(socketdescripter,&option,sizeof(option));
	printf("Enter Userid to view account details:");
	scanf("%d",&id);
	write(socketdescripter,&id,sizeof(id));
	//read data from server
	printf("\t\t*****ALL Account Details*****\t\t\n\n");
	struct jointuser info;
	read(socketdescripter,&info,sizeof(info));
	printf("\t\tAccount holder name of Parent:%s\n",info.parent_name);
	printf("\t\tAccount holder name of child:%s\n",info.child_name);
	printf("\t\tuserid:%d\n",info.userid);
	printf("\t\tpassword:%s\n",info.password);
	printf("\t\taccount_no:%d\n",info.account_no);
	printf("\t\tAccount balance:%f\n",info.balance);
	printf("\t\tAccount status:%d\n",info.status);

	//normal_joint_menu(socketdescripter,option);
}
void Add_normaluser(int socketdescripter)
{

		struct normaluser newuser;
		printf("Please Enter the name of User:");
		scanf("%[^\n]",newuser.name);
		strcpy(newuser.password,newuser.name);
		newuser.balance=0;
		write(socketdescripter,&newuser,sizeof(newuser));
		//result from server
		int result;
		read(socketdescripter,&result,sizeof(result));
		if(result==1)
		{
			printf("****Successfully Add new Account****\n");
		}
		else
		{
			printf("****Error while adding new Account****\n");
		}
		admin_menu(socketdescripter);
		
		
}
void Add_jointuser(int socketdescripter)
{

		struct jointuser newuser;
		printf("Please Enter the name of the Parent User:");
		scanf(" %[^\n]",newuser.parent_name);
		printf("Please Enter the name of the child User:");
		scanf(" %[^\n]",newuser.child_name);
		strcpy(newuser.password,newuser.parent_name);
		newuser.balance=0;
		write(socketdescripter,&newuser,sizeof(newuser));
		//result from server
		int result;
		read(socketdescripter,&result,sizeof(result));
		if(result==1)
		{
			printf("****Successfully Add new Account****\n");
		}
		else
		{
			printf("****Error while adding new Account****\n");
		}
		admin_menu(socketdescripter);		
}
void Addaccount(int socketdescripter,int choice)
{
	//send choice to server
	write(socketdescripter,&choice,sizeof(choice));
	int option;
	printf("Enter the type of Account 1 for normaluser 2 jointuser:");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
			write(socketdescripter,&option,sizeof(option));
			Add_normaluser(socketdescripter);
			break;
		case 2:
			write(socketdescripter,&option,sizeof(option));
			Add_jointuser(socketdescripter);
			break;
	}
	admin_menu(socketdescripter);
}
void delete_normaluser(int socketdescripter)
{
	int id;
	printf("Enter Userid to delete:");
	scanf("%d",&id);
	//send to server
	write(socketdescripter,&id,sizeof(id));
	//from server
	int result;
	read(socketdescripter,&result,sizeof(result));
	if(result==1)
	{
		printf("****Successfully Deleted Account****\n");
	}
	else
	{
		printf("****Error while Deleting Account****\n");
	}
	admin_menu(socketdescripter);

}
void delete_jointuser(int socketdescripter)
{
	int id;
	printf("Enter Userid to delete:");
	scanf("%d",&id);
	//send to server
	write(socketdescripter,&id,sizeof(id));
	//from server
	int result;
	read(socketdescripter,&result,sizeof(result));
	if(result==1)
	{
		printf("****Successfully Deleted Account****\n");
	}
	else
	{
		printf("****Error while Deleteing Account****\n");
	}
	admin_menu(socketdescripter);

}
void DeleteAccount(int socketdescripter,int choice)
{
	//send choice to server
	write(socketdescripter,&choice,sizeof(choice));
	int option;
	printf("Enter the type of Account to Delete 1 for normaluser 2 jointuser:");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
			write(socketdescripter,&option,sizeof(option));
			delete_normaluser(socketdescripter);
			break;
		case 2:
			write(socketdescripter,&option,sizeof(option));
			delete_jointuser(socketdescripter);
			break;

	}
	admin_menu(socketdescripter);
}


void modify_normaluser(int socketdescripter)
{
	struct normaluser changeuser;
	printf("Enter Userid to Modify:");
	scanf("%d",&changeuser.userid);
	printf("Enter New name of Account holder:");
	scanf(" %[^\n]",changeuser.name);
	//send to server
	write(socketdescripter,&changeuser,sizeof(changeuser));
	//from server
	int result;
	read(socketdescripter,&result,sizeof(result));
	if(result==1)
	{
		printf("****Successfully Modify Account****\n");
	}
	else
	{
		printf("****Error while Modifying Account****\n");
	}
	admin_menu(socketdescripter);

}

void modify_jointuser(int socketdescripter)
{
	struct jointuser changeuser;
	int id;
	printf("Enter Userid to Modify:");
	scanf("%d",&changeuser.userid);
	printf("Enter New name of parent Account holder:");
	scanf(" %[^\n]",changeuser.parent_name);
	printf("Enter New name of child Account holder:");
	scanf(" %[^\n]",changeuser.child_name);
	//send to server
	write(socketdescripter,&changeuser,sizeof(changeuser));
	//from server
	int result;
	read(socketdescripter,&result,sizeof(result));
	if(result==1)
	{
		printf("****Successfully Deleted Account****\n");
	}
	else
	{
		printf("****Error while Deleting Account****\n");
	}
	admin_menu(socketdescripter);

}
void ModifyAccount(int socketdescripter,int choice)
{
	//send choice to server
	write(socketdescripter,&choice,sizeof(choice));
	int option;
	printf("Enter the type of Account to Modify 1 for normaluser 2 jointuser:");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
			write(socketdescripter,&option,sizeof(option));
			modify_normaluser(socketdescripter);
			break;
		case 2:
			write(socketdescripter,&option,sizeof(option));
			modify_jointuser(socketdescripter);
			break;

	}
	admin_menu(socketdescripter);
}


void search_normaluser(int socketdescripter)
{
	int id;
	printf("Enter Userid to search:");
	scanf("%d",&id);
	//send to server
	write(socketdescripter,&id,sizeof(id));
	//from server
	int result;
	read(socketdescripter,&result,sizeof(result));
	if(result==1)
	{
		struct normaluser user;
		read(socketdescripter,&user,sizeof(user));
		printf("Name of Account holder:%s\n",user.name);
		printf("account_no:%d\n",user.account_no);
		printf("Balance:%f\n",user.balance);
		printf("Account status:%d\n",user.status);
	}
	else
	{
		printf("your Acoount is (INACTIVE OR ACCOUNT IS NOT EXIST) please contact your near branch\n");
	}
	admin_menu(socketdescripter);

}

void search_jointuser(int socketdescripter)
{
	int id;
	printf("Enter Userid to search:");
	scanf("%d",&id);
	//send to server
	write(socketdescripter,&id,sizeof(id));
	//from server
	int result;
	read(socketdescripter,&result,sizeof(result));
	struct jointuser user;
	if(result==1)
	{
		read(socketdescripter,&user,sizeof(user));
		printf("Name of Account holder(parent):%s\n",user.parent_name);
		printf("Name of Account holder(child):%s\n",user.child_name);
		printf("account_no:%d\n",user.account_no);
		printf("Balance:%f\n",user.balance);
		printf("Account status:%d\n",user.status);

	}
	else
	{
		printf("your Acoount is (INACTIVE OR ACCOUNT IS NOT EXIST)please contact your near branch\n");	
	}
	
	admin_menu(socketdescripter);

}

void SearchAccount(int socketdescripter,int choice)
{
	//send choice to server
	write(socketdescripter,&choice,sizeof(choice));
	int option;
	printf("Enter the type of Account to search 1 for normaluser 2 jointuser:");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
			write(socketdescripter,&option,sizeof(option));
			search_normaluser(socketdescripter);
			break;
		case 2:
			write(socketdescripter,&option,sizeof(option));
			search_jointuser(socketdescripter);
			break;

	}
	admin_menu(socketdescripter);
}

void admin_menu(int socketdescripter)
{
	printf("1.AddAccount\n2.DeleteAccount\n3.ModifyAccount\n4.SearchAccount\n5.Exit\n\n");
    int choice;
    printf("Enter your choice:");
    scanf("%d",&choice);
    switch(choice)
    {
    	case 1:
    		Addaccount(socketdescripter,choice);	
    		break;
    case 2:
    		DeleteAccount(socketdescripter,choice);
    		break;
    case 3:
    		ModifyAccount(socketdescripter,choice);
    		break;
    case 4:
    		SearchAccount(socketdescripter,choice);
    		break;
    case 5:
    		write(socketdescripter,&choice,sizeof(choice));
    		printf("\n\n****Thank you for visiting us****\n\n");
    		exit(0);
    }

}
void showMenu(int socketdescripter,int choice)
{
	printf("choice in showMenu:%d\n",choice);
    switch(choice)
    {
    	case 1:// normal user
    	normal_joint_menu(socketdescripter,choice);
    	break;
    	case 2://joint user
    	normal_joint_menu(socketdescripter,choice);
    	break;
    	case 3:
    	admin_menu(socketdescripter);
    }
}
