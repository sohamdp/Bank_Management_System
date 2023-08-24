#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include<fcntl.h>
#include<string.h>
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
int check_normaluser_credential(struct normaluser curruser)
{
	int result;
	struct normaluser data;
	int id=curruser.userid-1000;//record number
	int fd=open("normal_user.txt",O_RDONLY,0744);
	if(fd==-1) printf("Error\n");
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);// make change here by me try something new  
		lock.l_len=sizeof(curruser); //size of record to lock
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);// lock the current required record
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&data,sizeof(data));
		if(curruser.userid==data.userid && !strcmp(curruser.password,data.password) && data.status==1)//temparary change
		{
			result=1;
			printf("normalclient connected with User ID:%d\n",curruser.userid);

		}
		else
		{
			result=0;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
	}
	return result;
}
int check_jointuser_credential(struct jointuser curruser)
{
	int result;
	struct jointuser data;
	int id=curruser.userid-1000;//record number
	int fd=open("joint_user.txt",O_RDONLY,0744);
	if(fd==-1) printf("Error\n");
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);// make change here by me try something new  
		lock.l_len=sizeof(curruser); //size of record to lock
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);// lock the current required record
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&data,sizeof(data));
		if(curruser.userid==data.userid && !strcmp(curruser.password,data.password)&& data.status==1)//temparary change
		{
			result=1;
			printf("jointclient connected with User ID:%d\n",curruser.userid);
		}
		else
		{
			result=0;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
	}
	return result;
}
int check_admin_credential(struct admin curruser)
{
	int result;
	struct admin data;
	int id=curruser.userid-1000;//record number
	int fd=open("admin.txt",O_RDONLY,0744);
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);// make change here by me try something new  
		lock.l_len=sizeof(curruser); //size of record to lock
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);// lock the current required record
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&data,sizeof(data));
		if(curruser.userid==data.userid && !strcmp(curruser.password,data.password))
		{
			result=1;
			printf("admin connected with User ID:%d\n",curruser.userid);
		}
		else
		{
			result=0;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
	}
	return result;
}

int deposit(int choice,int newsocketdescripter,int userid)
{
	int id=userid-1000;
	int result;
	float amount;
	if(choice==1)//normal_user
	{
		int fd=open("normal_user.txt",O_RDWR,0744);
		if(fd!=-1)
		{
			struct flock lock;
			struct normaluser curruser;
			lock.l_type=F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(id)*sizeof(curruser);
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			read(newsocketdescripter,&amount,sizeof(amount));
			lseek(fd,(id)*sizeof(curruser),SEEK_SET);
			read(fd,&curruser,sizeof(curruser));
			if(curruser.status==1)
			{
				curruser.balance=curruser.balance+amount;
				lseek(fd,sizeof(curruser)*(-1),SEEK_CUR);
				write(fd,&curruser,sizeof(curruser));
				result=1;
			}
			else
			{
				result=0;
				printf("Account with that ID not exist please contact your nearest branch\n");
			}
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
			write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
			close(fd);
		}
		
	}
	else if(choice==2)//jointuser
	{
		
		int fd=open("joint_user.txt",O_RDWR,0744);
		if(fd!=-1)
		{

			struct flock lock;
			struct jointuser curruser;
			int size=id*sizeof(curruser);
			lock.l_type=F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=size;
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			read(newsocketdescripter,&amount,sizeof(amount));
			lseek(fd,size,SEEK_SET);
			read(fd,&curruser,sizeof(curruser));
			if(curruser.status==1)
			{
				curruser.balance=curruser.balance+amount;
				lseek(fd,sizeof(curruser)*(-1),SEEK_CUR);
				write(fd,&curruser,sizeof(curruser));
				result=1;
			}
			else
			{
				result=0;
				printf("Your Account is INACTIVE please contact your nearest branch\n");
			}
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
			write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
			close(fd);
		}
	}

	return result;
}

int withdraw(int choice,int newsocketdescripter,int userid)
{
	int id=userid-1000;
	int result;
	float amount;
	if(choice==1)//normal_user
	{
		
		int fd=open("normal_user.txt",O_RDWR,0744);
		if(fd!=-1)
		{
			struct flock lock;
			struct normaluser curruser;
			lock.l_type=F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(id)*sizeof(curruser);
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			read(newsocketdescripter,&amount,sizeof(amount));
			lseek(fd,(id)*sizeof(curruser),SEEK_SET);
			read(fd,&curruser,sizeof(curruser));
			if(curruser.status==1 && curruser.balance>=amount)
			{
				curruser.balance=curruser.balance-amount;
				lseek(fd,sizeof(curruser)*(-1),SEEK_CUR);
				write(fd,&curruser,sizeof(curruser));
				result=1;
			}
			else
			{
				result=0;
				if(curruser.balance<amount)
				{
					printf("Insufficient balance\n");
				}
				else if(curruser.status==1)
				{
					printf("Your Account is INACTIVE please contact your nearest branch\n");
				}
				
			}
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
			write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
			close(fd);
		}
		
	}
	else if(choice==2)//jointuser
	{
		
		int fd=open("joint_user.txt",O_RDWR,0744);
		if(fd!=-1)
		{
			struct jointuser curruser;
			int size=id*sizeof(curruser);
			struct flock lock;
			lock.l_type=F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=size;
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			read(newsocketdescripter,&amount,sizeof(amount));
			lseek(fd,size,SEEK_SET);
			read(fd,&curruser,sizeof(curruser));
			if(curruser.status==1 && curruser.balance>=amount)
			{
				curruser.balance=curruser.balance-amount;
				lseek(fd,sizeof(curruser)*(-1),SEEK_CUR);
				write(fd,&curruser,sizeof(curruser));
				result=1;
			}
			else
			{
				result=0;
				printf("Your Account is INACTIVE please contact your nearest branch\n");
			}
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
			write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
			close(fd);
		}
	}

	return result;
}

int Balance_Enquiry(int choice,int userid)
{
	int id=userid-1000;
	int amount;
	if(choice==1)//normal_user
	{
		struct normaluser curruser;
		int fd=open("normal_user.txt",O_RDONLY,0744);
		if(fd!=1)
		{
			struct flock lock;
			lock.l_type=F_RDLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(id)*sizeof(curruser);
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			lseek(fd,(id)*sizeof(curruser),SEEK_SET);
			read(fd,&curruser,sizeof(curruser));
			if(curruser.status==1)
			{
				amount=curruser.balance;	
			}
			else
			{
				printf("Your Account is INACTIVE please contact your nearest branch\n");
				amount=0;
			}
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
			write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
			close(fd);
		}
		
	}
	else if(choice==2)//jointuser
	{
		struct jointuser curruser;
		int size=id*sizeof(curruser);
		int fd=open("joint_user.txt",O_RDONLY,0744);
		if(fd!=-1)
		{
			struct flock lock;
			lock.l_type=F_RDLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=size;
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			lseek(fd,size,SEEK_SET);
			read(fd,&curruser,sizeof(curruser));
			if(curruser.status==1)
			{
				amount=curruser.balance;	
			}
			else
			{
				printf("Your Account is INACTIVE please contact your nearest branch\n");
				amount=0;
			}
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
			write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
			close(fd);
		}
	}

	return amount;
}

int password_change(int choice,int userid,int newsocketdescripter)
{
	int id=userid-1000;
	int result;
	char newpass[20];
	if(choice==1)//normaluser
	{
		struct normaluser curruser;
		int fd=open("normal_user.txt",O_RDWR,0744);
		if(fd!=-1)
		{
			struct flock lock;
			lock.l_type=F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(id)*sizeof(curruser);
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();

			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			read(newsocketdescripter,&newpass,sizeof(newpass));
			lseek(fd,(id)*sizeof(curruser),SEEK_SET);
			read(fd,&curruser,sizeof(curruser));
			if(curruser.status==1)
			{
				strcpy(curruser.password,newpass);
				lseek(fd,(-1)*sizeof(curruser),SEEK_CUR);
				write(fd,&curruser,sizeof(curruser));
				result=1;
			}
			else
			{
				result=0;
				printf("Your Account is INACTIVE please contact your nearest branch\n");

			}
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
			write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
			close(fd);
		}
	}
	else if(choice==2)//jointuser
	{
		struct jointuser curruser;
		int fd=open("joint_user.txt",O_RDWR,0744);
		if(fd!=-1)
		{
			struct flock lock;
			lock.l_type=F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(id)*sizeof(curruser);
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();

			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			read(newsocketdescripter,&newpass,sizeof(newpass));
			lseek(fd,(id)*sizeof(curruser),SEEK_SET);
			read(fd,&curruser,sizeof(curruser));
			if(curruser.status==1)
			{
				strcpy(curruser.password,newpass);
				lseek(fd,(-1)*sizeof(curruser),SEEK_CUR);
				write(fd,&curruser,sizeof(curruser));
				result=1;
			}
			else
			{
				result=0;
				printf("Your Account is INACTIVE please contact your nearest branch\n");

			}
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
			write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
			close(fd);
		}
	}
	return result;
}

struct normaluser View_normaluser_details(int choice,int userid)
{
	int id=userid-1000;
	struct normaluser curruser;
	int fd=open("normal_user.txt",O_RDONLY,0744);
	if(fd!=-1)
	{

		struct flock lock;
		lock.l_type=F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);
		lock.l_len=sizeof(curruser);
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);
		write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&curruser,sizeof(curruser));
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);
		
	}
	return curruser;
		
}
struct jointuser View_jointuser_details(int choice,int userid)
{
	int id=userid-1000;
	struct jointuser curruser;
	int fd=open("joint_user.txt",O_RDONLY,0744);
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);
		lock.l_len=sizeof(curruser);
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);
		write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&curruser,sizeof(curruser));
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);
	
    }
    return curruser;
}

int Add_normaluser(int newsocketdescripter,struct normaluser newuser)
{
	int result=0;
	struct normaluser curruser;
	int fd=open("normal_user.txt",O_CREAT|O_RDWR,0744);
	if(fd!=-1)
	{
		int size=lseek(fd,0,SEEK_END);
		struct flock lock;
		lock.l_type=F_WRLCK;
		lock.l_whence=SEEK_END;
		if(size==0)
		{
			lock.l_start=0;
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();

			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			read(fd,&curruser,sizeof(curruser));
			newuser.userid=curruser.userid+1;
			newuser.account_no=curruser.account_no+1;
			newuser.status=1;
			result=1;
		}
		else
		{
			lock.l_start=(-1)*sizeof(curruser);
		    lock.l_len=sizeof(curruser);
		    lock.l_pid=getpid();

		    fcntl(fd,F_SETLKW,&lock);
		    write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			lseek(fd,(-1)*sizeof(curruser),SEEK_END);
			read(fd,&curruser,sizeof(curruser));
			newuser.userid=curruser.userid+1;
			newuser.account_no=curruser.account_no+1;
			newuser.status=1;
			result=1;
		}
		write(fd,&newuser,sizeof(newuser));
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);	
	}	
	return result;
}

int Add_jointuser(int newsocketdescripter,struct jointuser newuser)
{
	int result=0;
	struct jointuser curruser;
	int fd=open("joint_user.txt",O_CREAT|O_RDWR,0744);
	if(fd!=-1)
	{
		int size=lseek(fd,0,SEEK_END);
		struct flock lock;
		lock.l_type=F_WRLCK;
		lock.l_whence=SEEK_END;
		if(size==0)
		{
			lock.l_start=0;
			lock.l_len=sizeof(curruser);
			lock.l_pid=getpid();

			fcntl(fd,F_SETLKW,&lock);
			write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			read(fd,&curruser,sizeof(curruser));
			newuser.userid=curruser.userid+1;
			newuser.account_no=curruser.account_no+1;
			newuser.status=1;
			result=1;

		}
		else
		{
			lock.l_start=(-1)*sizeof(curruser);
		    lock.l_len=sizeof(curruser);
		    lock.l_pid=getpid();

		    fcntl(fd,F_SETLKW,&lock);
		    write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
			lseek(fd,(-1)*sizeof(curruser),SEEK_END);
			read(fd,&curruser,sizeof(curruser));
			newuser.userid=curruser.userid+1;
			newuser.account_no=curruser.account_no+1;
			newuser.status=1;
			result=1;
		}
		write(fd,&newuser,sizeof(newuser));
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);	
	}	
	return result;
}

int Delete_normaluser(int newsocketdescripter,int userid)
{
	int result=0;
	int id=userid-1000;//record number
	struct normaluser curruser;
	int fd=open("normal_user.txt",O_RDWR,0744);
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);
		lock.l_len=sizeof(curruser);
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);
		write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&curruser,sizeof(curruser));
		if(curruser.status==1)
		{
			curruser.status=0;
			result=1;
		}
		lseek(fd,(-1)*sizeof(curruser),SEEK_CUR);
		write(fd,&curruser,sizeof(curruser));
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);

	}
	return result;

}
int Delete_jointuser(int newsocketdescripter,int userid)
{
	int result=0;
	int id=userid-1000;//record number
	struct jointuser curruser;
	int fd=open("joint_user.txt",O_RDWR,0744);
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);
		lock.l_len=sizeof(curruser);
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);
		write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&curruser,sizeof(curruser));
		if(curruser.status==1)
		{
			curruser.status=0;
			result=1;
		}
		lseek(fd,(-1)*sizeof(curruser),SEEK_CUR);
		write(fd,&curruser,sizeof(curruser));
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);

	}
	return result;

}

int Modify_normaluser(int newsocketdescripter,struct normaluser modifyuser)
{
	int result=0;
	int id=modifyuser.userid-1000;//record number
	struct normaluser curruser;
	int fd=open("normal_user.txt",O_RDWR,0744);
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);
		lock.l_len=sizeof(curruser);
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);
		write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&curruser,sizeof(curruser));
		if(curruser.status==1 && curruser.userid==modifyuser.userid)
		{
			strcpy(curruser.name,modifyuser.name);
			lseek(fd,(-1)*sizeof(curruser),SEEK_CUR);
			write(fd,&curruser,sizeof(curruser));
			result=1;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);

	}
	return result;
}

int Modify_jointuser(int newsocketdescripter,struct jointuser modifyuser)
{
	int result=0;
	int id=modifyuser.userid-1000;//record number
	struct jointuser curruser;
	int fd=open("joint_user.txt",O_RDWR,0744);
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);
		lock.l_len=sizeof(curruser);
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);
		write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&curruser,sizeof(curruser));
		if(curruser.status==1 && curruser.userid==modifyuser.userid)
		{
			strcpy(curruser.parent_name,modifyuser.parent_name);
			strcpy(curruser.child_name,modifyuser.child_name);
			lseek(fd,(-1)*sizeof(curruser),SEEK_CUR);
			write(fd,&curruser,sizeof(curruser));
			result=1;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);

	}
	return result;
}


void Search_normaluser(int newsocketdescripter,int userid)
{
	struct normaluser curruser;
	int id=userid-1000;
	int result;
	int fd=open("normal_user.txt",O_RDONLY,0744);
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);
		lock.l_len=sizeof(curruser);
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);
		write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&curruser,sizeof(curruser));
		if(curruser.status==1 && curruser.userid==userid)
		{
			result=1;
			write(newsocketdescripter,&result,sizeof(result));
			write(newsocketdescripter,&curruser,sizeof(curruser));
		}
		else if(curruser.status==0 || curruser.userid!=userid)
		{
			result=0;
			write(newsocketdescripter,&result,sizeof(result));
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);

	}
}

void Search_jointuser(int newsocketdescripter,int userid)
{
	struct jointuser curruser;
	int id=userid-1000;
	int result;
	int fd=open("joint_user.txt",O_RDONLY,0744);
	if(fd!=-1)
	{
		struct flock lock;
		lock.l_type=F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(curruser);
		lock.l_len=sizeof(curruser);
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);
		write(1,"\n****Inside the Critical Section****\n",sizeof("\n****Inside the Critical Section****\n"));
		lseek(fd,(id)*sizeof(curruser),SEEK_SET);
		read(fd,&curruser,sizeof(curruser));
		if(curruser.status==1 && curruser.userid==userid)
		{
			result=1;
			write(newsocketdescripter,&result,sizeof(result));
			write(newsocketdescripter,&curruser,sizeof(curruser));
		}
		else if(curruser.status==0 || curruser.userid!=userid)
		{
			result=0;
			write(newsocketdescripter,&result,sizeof(result));
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(1,"\n****Out off Critical Section****\n",sizeof("\n****Out off Critical Section****\n"));
		close(fd);

	}
}
void server_task(int newsocketdescripter)
{
	//read the choice send by client
	int choice;
	int result,id;
	while(1)
	{
		read(newsocketdescripter,&choice,sizeof(choice));
		if(choice==1)//normal user
		{
			struct normaluser curruser;
			read(newsocketdescripter,&curruser,sizeof(curruser));
			id=curruser.userid;
			//printf("normalclient connected with normalUser ID:%d\n",id);
			result=check_normaluser_credential(curruser);
			write(newsocketdescripter,&result,sizeof(result));
		}
		else if(choice==2)//joint user
		{
			struct jointuser curruser;
			read(newsocketdescripter,&curruser,sizeof(curruser));
			id=curruser.userid;
			//printf("jointclient connected with jointUser ID:%d\n",id);
			result=check_jointuser_credential(curruser);
			write(newsocketdescripter,&result,sizeof(result));
		}
		else if(choice==3)//admin
		{
			struct admin curruser;
			read(newsocketdescripter,&curruser,sizeof(curruser));
			id=curruser.userid;
			//printf("Admin connected with admin ID:%d\n",id);
			result=check_admin_credential(curruser);
			write(newsocketdescripter,&result,sizeof(result));
		}

		if(result)//if credentials are correct then break out of loop
		{
			break;
		}
	}
	
	while(1)
	{
		if(choice==1 || choice==2)
		{
			int select_choice;// select options from 6
		    read(newsocketdescripter,&select_choice,sizeof(select_choice));
			if(select_choice==1)//deposit money used for both normal and joint user
			{
				// float amount;
				// read(newsocketdescripter,&amount,sizeof(float));
				int result=deposit(choice,newsocketdescripter,id);
				write(newsocketdescripter,&result,sizeof(result));
			}
			else if(select_choice==2)//withdraw money
			{
				// float amount;
				// read(newsocketdescripter,&amount,sizeof(float));
				int result=withdraw(choice,newsocketdescripter,id);
				write(newsocketdescripter,&result,sizeof(result));
			}
			else if(select_choice==3)//balance
			{
				float amount;
				amount=Balance_Enquiry(choice,id);
				write(newsocketdescripter,&amount,sizeof(float));
			}
			else if(select_choice==4)// password change
			{
				int result;
				// char newpass[20];
				// read(newsocketdescripter,&newpass,sizeof(newpass));
				result=password_change(choice,id,newsocketdescripter);
				write(newsocketdescripter,&result,sizeof(result));
			}
			else if(select_choice==5)//veiw details
			{
				int ch;
				read(newsocketdescripter,&ch,sizeof(ch));
				if(ch==1)//normaluser
				{
					struct normaluser result;
					int userid;
					read(newsocketdescripter,&userid,sizeof(userid));
					result=View_normaluser_details(choice,userid);
					write(newsocketdescripter,&result,sizeof(result));	
				}
				else if(ch==2)//jointuser
				{
					struct jointuser result;
					int userid;
					read(newsocketdescripter,&userid,sizeof(userid));
					result=View_jointuser_details(choice,userid);
					write(newsocketdescripter,&result,sizeof(result));	
				}

			}
			else if(select_choice==6)
			{
				//write(newsocketdescripter,&choice,sizeof(choice));
				break;
			}

		}
		else if(choice==3)
		{
			int select_choice;//admin side
			read(newsocketdescripter,&select_choice,sizeof(int));
		    printf("select_choice:%d\n",select_choice);
		    if(select_choice==1)//addaccount
		    {
		    	int choice;
		    	read(newsocketdescripter,&choice,sizeof(choice));
		    	if(choice==1)//normal user
		    	{
		    		int result;
		    		struct normaluser newuser;
		    		read(newsocketdescripter,&newuser,sizeof(newuser));
		    		result=Add_normaluser(newsocketdescripter,newuser);
		    		write(newsocketdescripter,&result,sizeof(result));
		    	}
		    	else if(choice==2)//jointuser
		    	{
		    		int result;
		    		struct jointuser newuser;
		    		read(newsocketdescripter,&newuser,sizeof(newuser));
		    		result=Add_jointuser(newsocketdescripter,newuser);
		    		write(newsocketdescripter,&result,sizeof(result));
		    	}
		    	
		    }
		    else if(select_choice==2)//delete account
		    {
		    	int choice;
		    	read(newsocketdescripter,&choice,sizeof(choice));
		    	if(choice==1)//normal user
		    	{
		    		int result,userid;
		   			read(newsocketdescripter,&userid,sizeof(userid));
		    		result=Delete_normaluser(newsocketdescripter,userid);
		    		write(newsocketdescripter,&result,sizeof(result));
		    	}
		    	else if(choice==2)//jointuser
		    	{
		    		int result,userid;
		   			read(newsocketdescripter,&userid,sizeof(userid));
		    		result=Delete_jointuser(newsocketdescripter,userid);
		    		write(newsocketdescripter,&result,sizeof(result));
		    	}
		    }
		    else if(select_choice==3)//modify account
		    {
		    	int choice;
		    	read(newsocketdescripter,&choice,sizeof(choice));
		    	if(choice==1)//normal user
		    	{
		    		int result;
		    		struct normaluser modifyuser;
		   		read(newsocketdescripter,&modifyuser,sizeof(modifyuser));
		    		result=Modify_normaluser(newsocketdescripter,modifyuser);
		    		write(newsocketdescripter,&result,sizeof(result));
		    	}
		    	else if(choice==2)//jointuser
		    	{
		    		int result;
		    		struct jointuser modifyuser;
		   		read(newsocketdescripter,&modifyuser,sizeof(modifyuser));
		    		result=Modify_jointuser(newsocketdescripter,modifyuser);
		    		write(newsocketdescripter,&result,sizeof(result));
		    	}
		    }
		    else if(select_choice==4)
		    {
		    	int choice;
		    	read(newsocketdescripter,&choice,sizeof(choice));
		    	if(choice==1)//normal user
		    	{
		    		int result,userid;
		   			read(newsocketdescripter,&userid,sizeof(userid));
		    		Search_normaluser(newsocketdescripter,userid);
		    	}
		    	else if(choice==2)//jointuser
		    	{
		    		int result,userid;
		   			read(newsocketdescripter,&userid,sizeof(userid));
		    		Search_jointuser(newsocketdescripter,userid);
		    	}
		    }
		    else if(select_choice==5)
		    {
		    	break;
		    }

		}
	    
	}

	close(newsocketdescripter);
	printf("\n\t\t****Seasson End****\t\t\n");
}


void main()
{
	struct sockaddr_in server,client; //store the information of server
	int socketdescripter,clientlen;
	//socket descripter
	socketdescripter=socket(AF_INET,SOCK_STREAM,0);
	if (socketdescripter < 0)
        printf("Error in server creating\n");
    else
        printf("Server Created\n");
	if(socketdescripter!=1)
	{
		server.sin_family=AF_INET;
		server.sin_addr.s_addr=INADDR_ANY;// client on same machine
		server.sin_port=htons(8888);
		bind(socketdescripter,(struct sockaddr *)&server,sizeof(server));
		listen(socketdescripter,10);// atmost 10 users serverd at atime
		printf("****Waiting for Client to connect****\n\n");
		while(1)
		{

			clientlen=sizeof(client);
			int newsocketdescripter=accept(socketdescripter,(struct sockaddr *)&client,&clientlen);
			if(fork()==0)
			{
				//child
				close(socketdescripter);
				server_task(newsocketdescripter);
				exit(0);

			}
			else
			{
				//parent
				close(newsocketdescripter);
			}
		}
	}
}








